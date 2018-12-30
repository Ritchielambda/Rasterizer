#include"QRender.h"

QRender::QRender(HWND handle, int width,int height)
{
	hwnd = handle;
	WVP = MathInterface::MatrixIdentity();  //
	m_bufferwidth = width;
	m_bufferheight = height;
	
	rasterizer = new Rasterizer(handle, width, height);
	rendermode = fillmode;
	Mat_Screen = MathInterface::MatrixScreenTransform(m_bufferwidth,m_bufferheight);
}
void QRender::SetWVP(const Matrix& mat)
{
	WVP = mat;
}

//void QRender::SetVertexbuffer(int PVertexsize, std::vector<Vertex>* data)
//{
//	if (InVertexbuffer == data) return;
//	//if (Vertexbuffer) delete Vertexbuffer;
//	Vertexsize = PVertexsize;
//	InVertexbuffer = data;
//}
//void QRender::SetIndexbuffer(int PIndexsize,std::vector<int>* data)
//{
//	if (InIndexbuffer == data) return;
//	//if (Indexbuffer) delete Indexbuffer;
//	Indexsize = PIndexsize;
//	InIndexbuffer = data;
//}
void QRender::DrawIndexed()//it's private interface
{
	// test only draw one obj  a time   clear buffer here
	
	//if ((!InVertexbuffer) && (!InIndexbuffer)) return;
	//int temp = Indexsize % 3;
	//if (temp != 0)return;
	//for (int i = 0; i < Indexsize; i += 3)//  do transforming from obj space to screen space
	//{
	//	Vertex v1 = InVertexbuffer[InIndexbuffer[i]];
	//	Vertex v2 = InVertexbuffer[InIndexbuffer[i+1]];
	//	Vertex v3 = InVertexbuffer[InIndexbuffer[i+2]];
	//	v1 *= WVP;
	//	v2 *= WVP;
	//	v3*= WVP;
	//	v1.Divz = 1 / v1.m_Position.w;
	//	v2.Divz = 1 / v2.m_Position.w;
	//	v3.Divz = 1 / v3.m_Position.w;
	//	toCVV(v1);
	//	toCVV(v2);
	//	toCVV(v3);
	//	v1 *= Mat_Screen;
	//	v2 *= Mat_Screen;
	//	v3 *= Mat_Screen;
	//	rasterizer->drawtriangles(v1, v2, v3);
	//}
	//int temp1 = Indexsize % 3;
}


void QRender::Present()
{
	
}
void QRender::toCVV(Vertex& vertex)
{
	vertex.m_Position.x /= vertex.m_Position.w;
	vertex.m_Position.y /= vertex.m_Position.w;
	vertex.m_Position.z /= vertex.m_Position.w;
	vertex.m_Position.w = 1;
}
void QRender::HomoSpaceClipping_Triangles(std::vector<UINT>* const pIB)//
{
	*m_pIB_HomoSpace_Clipped = (*pIB);
	*m_pVB_HomoSpace_Clipped = std::move(*m_pVB_HomoSpace);//std::move创建引用 节省内存？

	UINT i = 0;
	while (i < m_pIB_HomoSpace_Clipped->size() - 3)//<size -3 because it would swap it's value to last pos,then pop it out
	{
		UINT idx1 = m_pIB_HomoSpace_Clipped->at(i);
		UINT idx2 = m_pIB_HomoSpace_Clipped->at(i + 1);
		UINT idx3 = m_pIB_HomoSpace_Clipped->at(i + 2);

		auto const v1 = m_pVB_HomoSpace_Clipped->at(idx1);
		auto const v2 = m_pVB_HomoSpace_Clipped->at(idx2);
		auto const v3 = m_pVB_HomoSpace_Clipped->at(idx3);

		bool b1 = v1.posH.x <= -1.0f || v1.posH.x >= 1.0f ||
			v1.posH.y <= -1.0f || v1.posH.y >= 1.0f ||
			v1.posH.z <= 0.0f || v1.posH.z >= 1.0f;

		bool b2 = v2.posH.x <= -1.0f || v2.posH.x >= 1.0f ||
			v2.posH.y <= -1.0f || v2.posH.y >= 1.0f ||
			v2.posH.z <= 0.0f || v2.posH.z >= 1.0f;

		bool b3 = v3.posH.x <= -1.0f || v3.posH.x >= 1.0f ||
			v3.posH.y <= -1.0f || v3.posH.y >= 1.0f ||
			v3.posH.z <= 0.0f || v3.posH.z >= 1.0f;

		if (b1 && b2 && b3 == true)
		{
			//swap the clipped index to the tail
			UINT rubbishFragmentStartIndex = m_pIB_HomoSpace_Clipped->size() - 3;
			std::swap(m_pIB_HomoSpace_Clipped->at(i), m_pIB_HomoSpace_Clipped->at(rubbishFragmentStartIndex));
			std::swap(m_pIB_HomoSpace_Clipped->at(i + 1), m_pIB_HomoSpace_Clipped->at(rubbishFragmentStartIndex + 1));
			std::swap(m_pIB_HomoSpace_Clipped->at(i + 2), m_pIB_HomoSpace_Clipped->at(rubbishFragmentStartIndex + 2));

			//then pop back the rubbish indices
			for (UINT j = 0; j < 3; j++)m_pIB_HomoSpace_Clipped->pop_back();
		}
		else
		{
			//because when triangle is clipped,another triangle at the tail of list
			//will be swapped with current triangle, thus can't move on to process
			//next triangle
			i += 3;
		}
	}
}

void QRender::DrawTriangles(QRenderdrawcalldata & drawCallData)
{

	//clear the last draw call ruins
	m_pVB_HomoSpace->clear();
	m_pVB_HomoSpace_Clipped->clear();
	m_pIB_HomoSpace_Clipped->clear();
	m_pVB_Rasterized->clear();

	UINT offset = drawCallData.offset;
	UINT vCount = drawCallData.VertexCount;
	auto const pVB = drawCallData.pVertexbuffer;//the data the ptr point to can't be modified
	auto const pIB = drawCallData.pIndexbuffer;

	//reserve space for vector, because they need to push_back later
	m_pVB_HomoSpace->reserve(vCount);
	m_pVB_Rasterized->reserve(m_bufferwidth*m_bufferheight / 3);// /3 is approximate estimation

	//-----------------------VERTEX SHADER---------------------
	for (UINT i = offset; i < (offset + vCount>pVB->size() ? pVB->size() : offset + vCount); ++i)
	{

		Vertex& currVertex = drawCallData.pVertexbuffer->at(offset + i);

		//Use Vertex Shader To Deal with Every vertex
		VertexShader(currVertex);
	}

	//--------------------HOMO SPACE CLIPPING----------------------
	HomoSpaceClipping_Triangles(pIB);

	//----------------------RASTERIZER-----------------------------


}

void QRender::RasterizeTriangles()
{
/************************************************************

todo

************************************************************/


	//rasterizer->drawtriangles()
}

void QRender::SetWordMatrix(Matrix pwordMatrix)
{
	WorldMatrix = pwordMatrix;
}

void QRender::SetViewMatrix(Matrix pviewMatrix)
{
	ViewMatrix = pviewMatrix;
}

void QRender::SetProjMatrix(Matrix projMatirx)
{
	ProjMatrix = projMatirx;
}
void QRender::SetTexcoordTransform(float dx, float dy, float scale)
{
	mTexCoord_offsetX = dx;
	mTexCoord_offsetY = dy;
	mTexCoord_scale = scale;
}
void QRender::VertexShader(Vertex& invertex)
{
	VertexShaderOutput_Vertex outVertex;
	QVector pos(invertex.m_Position.x, invertex.m_Position.y, invertex.m_Position.z,1.0f);
	pos = pos *WorldMatrix;
	pos = pos *ViewMatrix;

	float Z_ViewSpace = pos.z;
	if (Z_ViewSpace >= 0)
	{
		pos.x /= (Z_ViewSpace);
		pos.y /= (Z_ViewSpace);
	}
	outVertex.posH = pos;


	Matrix WorldMat_Trans = MathInterface::MatrixInverse(WorldMatrix);
	WorldMat_Trans = MathInterface::MatrixTranspose(WorldMat_Trans);
	QVector Normal(invertex.m_Normal.x, invertex.m_Normal.y, invertex.m_Normal.z, 1.0f);
	Normal = Normal*WorldMat_Trans;

	//texture process
	outVertex.texcoord = FLOAT2(
		invertex.m_UV.x*mTexCoord_scale + mTexCoord_offsetX,
		invertex.m_UV.y*mTexCoord_scale + mTexCoord_offsetY);

	//lighting process          
/************************************************************

todo

************************************************************/
	m_pVB_HomoSpace->push_back(outVertex);

}