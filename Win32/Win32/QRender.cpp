#include"QRender.h"
#include<math.h>
#include <cmath>
#include"LoadBitmap.h"
QRender::QRender(HWND handle, UINT width,UINT height)
{
	hwnd = handle;
	WVP = MathInterface::MatrixIdentity();  //
	rasterizer = new Rasterizer(handle, width, height);
	rendermode = fillmode;
	Mat_Screen = MathInterface::MatrixScreenTransform(m_bufferwidth,m_bufferheight);

	m_pVB_HomoSpace = new std::vector<VertexShaderOutput_Vertex>;
	m_pVB_HomoSpace_Clipped = new std::vector<VertexShaderOutput_Vertex>;
	m_pIB_HomoSpace_Clipped = new std::vector<UINT>;
	m_pVB_Rasterized = new std::vector<RasterizedFragment>;
	m_pTexture = nullptr;//wait for user to set
	WorldMatrix.Identity();
	ViewMatrix.Identity();
	ProjMatrix.Identity();
	mTexCoord_offsetX = 0.0f;
	mTexCoord_offsetY = 0.0f;
	mTexCoord_scale = 1.0f;

	// ------------------------------------------LIGHTING setting -------------------------------------------------------------
	/*mCameraPos = { 0,0,0 };
	mLightEnabled = TRUE;*/
	//for (UINT i = 0; i < c_maxLightCount; ++i)memset(&mDirLight[i], 0, sizeof(DirectionalLight));


	Init(width, height,handle);
}

QRender::QRender()
{

	//  Effective 16:  创建时带有  new  []   delete 也要有相应的[]
	delete rasterizer;
	delete m_pVB_HomoSpace ;
	delete m_pVB_HomoSpace_Clipped;
	delete m_pIB_HomoSpace_Clipped;
	delete m_pVB_Rasterized;
	delete m_pTexture;//wait for user to set

	
}

void QRender::Init(UINT bufferwidth, UINT bufferheight,HWND handle)
{
	//process cursor

//----------------------------------------------

//					to do

//--------------------------------------------------

//--------------------------Initialize   BitMap---------------------------------
	

//process initialization of buffers
	bool isSucceed = false;
	//  may be change   buffer size is specialized to objects   rather than screen
	m_bufferheight = bufferheight;
	m_bufferwidth = bufferwidth;
	m_pOutColorBuffer = new std::vector<COLOR4>(m_bufferheight*m_bufferwidth);
	m_pZBuffer = new std::vector<float>(m_bufferheight*m_bufferwidth);
	m_BitMapBuffer = new UINT[m_bufferheight*m_bufferwidth];
	COLOR4 color = COLOR4(QVector(0.0f, 0.0f, 0.0f, 0.0f));
	ClearScreen(color);
	mFunction_InitializeBitMap();
}
void QRender::SetWVP(const Matrix& mat)
{
	WVP = mat;
}

void QRender::Present()
{
	mFunction_UpdateBitMapBuffer();
	
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
COLOR4 QRender::mFunction_SampleTexture(float x, float y)
{
	//texture mapping disabled, diffuse color will be taken from material
	if (m_pTexture == nullptr)return QVector(1.0f, 1.0f, 1.0f,1.0f);

	//wrap-mode
	UINT width = m_pTexture->GetWidth();
	UINT height = m_pTexture->GetHeight();
	float pixelX = abs(width * (float(x - UINT(x))));
	float pixelY = abs(height * (float(y - UINT(y))));
	return m_pTexture->GetPixel(UINT(pixelX), UINT(pixelY));
}
inline bool QRender::mFunction_DepthTest(UINT x, UINT y, float testZ)
{
	float& targetZ = m_pZBuffer->at(y*m_bufferwidth + x);
	if (testZ <= targetZ&&testZ>0.0f)
	{
		targetZ = testZ;
		return true;
	}
	else
	{
		return true;
	}
}

void QRender::ClearZbuffer()
{
	for (UINT i = 0; i < (*m_pZBuffer).size(); ++i)
	{
		//  after homogeneous space transform ,1 is biggest Z
		(*m_pZBuffer)[i] = 1;
	}
}

void QRender::ClearScreen(COLOR4 clearColor)
{
	for (UINT i = 0; i < m_bufferheight*m_bufferwidth; ++i)
	{
		m_pOutColorBuffer->at(i) = clearColor;
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

	RasterizeTriangles();

	//----------------------PIXEL SHADER-------------------------
	for (auto& rasterizedVertex : *m_pVB_Rasterized)
	{
		PixelShader_DrawTriangles(rasterizedVertex);
	}
	//------------------------PRESENT----------------------------
	Present();
}

void QRender::mFunction_SetZ(UINT x, UINT y, float z)
{
	m_pZBuffer->at(y*m_bufferwidth + x) = z;
}

void QRender::mFunction_UpdateBitMapBuffer()
{

	/////--------------------------used for PRESENT------------------------------------------------------


	//为什么这里获得的HDC不是我创建的windows屏幕的表面

	/////--------------------------used for PRESENT------------------------------------------------------

	///test bitmap code---------------------------------------------------
	//Texture2D texture = MathInterface::LoadBitmapToColorArray(L"2.bmp");
	//for (int i = 0; i < texture.m_height; ++i)
	//{
	//	for (int j = 0; j < texture.m_width; ++j)
	//	{
	//		m_BitMapBuffer[i*texture.m_width + j] = QVectorConverttoINT(texture.m_pixelbuffer[j][i]);
	//	}
	//}


	for (int i = 0; i < (*m_pOutColorBuffer).size(); ++i)
	{
		UINT pixelcolor = mFunction_QVectorConverttoINT((*m_pOutColorBuffer).at(i));
		m_BitMapBuffer[i] = pixelcolor;
	}
}

void QRender::mFunction_InitializeBitMap()
{
	//create BitMap

	HDC hdc = GetDC(hwnd);
	m_hdc = CreateCompatibleDC(hdc);
	BITMAPINFOHEADER bmphdr = { 0 };
	bmphdr.biSize = sizeof(BITMAPINFOHEADER);
	bmphdr.biWidth = m_bufferwidth;
	bmphdr.biHeight = m_bufferheight;
	bmphdr.biPlanes = 1;
	bmphdr.biBitCount = 32;
	bmphdr.biSizeImage = m_bufferwidth * m_bufferheight * 4;
	//创建后缓冲区
	//先创建一个内存dc
	//hdc = CreateCompatibleDC(nullptr);
	HBITMAP outputbitmap;
	outputbitmap = CreateDIBSection(m_hdc, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS, reinterpret_cast<void**>(&m_BitMapBuffer), nullptr, 0);

	if (!outputbitmap)
	{
		MessageBox(0, (LPCWSTR) "CreateDIBSection Failed.", 0, 0);
	}
	BITMAP bm;
	HBITMAP m_OldBitmap = (HBITMAP)SelectObject(m_hdc, outputbitmap);
	ReleaseDC(hwnd, m_hdc);
	GetObject(outputbitmap, sizeof(BITMAP), &bm);
}

void QRender::RasterizeTriangles()
{
/************************************************************

todo

************************************************************/
	for (UINT tri = 0; tri < m_pIB_HomoSpace_Clipped->size() - 2; tri += 3)
	{
		UINT idx1 = m_pIB_HomoSpace_Clipped->at(tri);
		UINT idx2 = m_pIB_HomoSpace_Clipped->at(tri + 1);
		UINT idx3 = m_pIB_HomoSpace_Clipped->at(tri + 2);

		RasterizedFragment outVertex;

		//3 vertices of triangles  in homogeneous space
		const auto&v1 = m_pVB_HomoSpace_Clipped->at(idx1);
		const auto&v2 = m_pVB_HomoSpace_Clipped->at(idx2);
		const auto&v3 = m_pVB_HomoSpace_Clipped->at(idx3);

		//convert to pixel space
		auto convertToPixelSpace = [&](const VertexShaderOutput_Vertex& v, FLOAT2& outV)
		{
			outV.x = float(m_bufferwidth) * (v.posH.x + 1.0f) / 2.0f;
			outV.y = float(m_bufferheight) * (-v.posH.y + 1.0f) / 2.0f;
		};
		FLOAT2 v1_pixel, v2_pixel, v3_pixel;//pixel space
		convertToPixelSpace(v1, v1_pixel);
		convertToPixelSpace(v2, v2_pixel);
		convertToPixelSpace(v3, v3_pixel);

		//Basis Vector, used to compute the bilinear interpolation coord (s,t) of current pixel
		FLOAT2 basisVector1 = v2_pixel - v1_pixel;
		FLOAT2 basisVector2 = v3_pixel - v1_pixel;

		float D = basisVector1.x*basisVector2.y - basisVector2.x*basisVector1.y;
		if (D == 0)return;// parallel   return

		float minY = MathInterface::Clamp(min(min(v1_pixel.y, v2_pixel.y), v3_pixel.y), 0, float(m_bufferheight - 1));
		float maxY = MathInterface::Clamp(max(max(v1_pixel.y, v2_pixel.y), v3_pixel.y), 0, float(m_bufferheight - 1));

		for (int j = int(minY); j<int(maxY); ++j)
		{
			BOOL intersectSucceeded = FALSE;
			UINT x1 = 0, x2 = 0;
			intersectSucceeded = 
				mFunction_HorizontalIntersect(float(j), v1_pixel, v2_pixel, v3_pixel, x1, x2);

			if (intersectSucceeded)
			{
				for (UINT i = x1; i <= x2; ++i)
				{
					if (i >= m_bufferwidth || j >= m_bufferheight)
					{
						break;
					}


					FLOAT2 currentPoint_pixel = FLOAT2(float(i) + 0.5f, float(j) + 0.5f);

					//v1 (A) is the orginal point of basis, calculate the relative pixel coordinate
					FLOAT2 currentPointLocal_pixel = currentPoint_pixel - v1_pixel;

					//calculate the bilinear interpolation ratio coordinate (s,t)
					// (->localP) = s (->V1) + t(->V2)
					float s = (currentPointLocal_pixel.x*basisVector2.y - currentPointLocal_pixel.y*basisVector2.x) / D;
					float t = (basisVector1.x*currentPointLocal_pixel.y - basisVector1.y*currentPointLocal_pixel.x) / D;

					//depth correct interpolation ,then perform depth test
					float depth = 1.0f / (s / v2.posH.z + t / v3.posH.z + (1 - s - t) / v1.posH.z);
					if (mFunction_DepthTest(i, j, depth) == FALSE)goto label_nextPixel;

					//write to  depth buffer
					mFunction_SetZ(i, j, depth);

					outVertex.pixelX = i;
					outVertex.pixelY = j;

					//perspective correct interpolation
					outVertex.color =
						(s / v2.posH.z* v2.color +
							t / v3.posH.z *v3.color +
							(1 - s - t) / v1.posH.z *v1.color)*depth;

					outVertex.texcoord =
						(s / v2.posH.z * v2.texcoord +
							t / v3.posH.z * v3.texcoord +
							(1 - s - t) / v1.posH.z * v1.texcoord)*depth;

					m_pVB_Rasterized->push_back(outVertex);
				label_nextPixel:;
				}//for next pixel

			}//if horizontal intersect succeed

		}//for each y

	}//for each homogeneous clip space triangle


	//rasterizer->drawtriangles()
}

void QRender::PixelShader_DrawTriangles(RasterizedFragment &inVertex)
{
	//invertex.color.w actually didn't work
	COLOR4 outColor;
	//if texture mapping is disabled, (1,1,1) will be returned to apply multiplication
	COLOR4 texSampleColor = mFunction_SampleTexture(inVertex.texcoord.x, inVertex.texcoord.y);
	outColor = COLOR4(inVertex.color.x, inVertex.color.y, inVertex.color.z, inVertex.color.w) * texSampleColor;
	m_pOutColorBuffer->at(inVertex.pixelY*m_bufferwidth + inVertex.pixelX) = outColor;
}

bool QRender::mFunction_HorizontalIntersect(float y, const FLOAT2 & v1, const FLOAT2 & v2, const FLOAT2 & v3, UINT & outX1, UINT & outX2)
{
	const FLOAT2 trianglesVertices[3]={v1,v2,v3};

	//	//step1, find out how many vertices have the same Y Coord with test Y Coord
	std::vector<UINT> indexList_sameYCoord;
	if (v1.y == y) indexList_sameYCoord.push_back(0);
	if (v2.y == y) indexList_sameYCoord.push_back(1);
	if (v3.y == y) indexList_sameYCoord.push_back(2);

	//line-line
	const auto func_intersect = [](float y, const FLOAT2&v1, const FLOAT2& v2, FLOAT2& outFLOAT2)->bool
	{
		bool b1 = v1.y > y && v2.y < y;
		bool b2 = v1.y < y && v2.y > y;
		// if two points lies on two sides of line Y=y
		if (b1 || b2)
		{
			outFLOAT2 = MathInterface::Lerp(v1, v2, (y - v1.y) / (v2.y - v1.y));
			return true;
		}
		else
		{
			return false;
		}
	};
	//step2, discuss every circumstances and compute intersect result[x1, x2]
	FLOAT2 outVec;
	switch (indexList_sameYCoord.size())
	{
		//------------0 : no points lie on line Y=y-----------------
	case 0:
	{
		bool canIntersect = false;
		std::vector<FLOAT2> intersectPointList;

		FLOAT2 tmpVec;
		canIntersect = func_intersect(y, v1, v2, tmpVec);
		if (canIntersect)intersectPointList.push_back(tmpVec);

		canIntersect = func_intersect(y, v2, v3, tmpVec);
		if (canIntersect)intersectPointList.push_back(tmpVec);

		canIntersect = func_intersect(y, v1, v3, tmpVec);
		if (canIntersect)intersectPointList.push_back(tmpVec);
		if (intersectPointList.size() == 2)
		{
			outX1 = UINT(MathInterface::Clamp(intersectPointList.at(0).x, 0, float(m_bufferwidth)));
			outX2 = UINT(MathInterface::Clamp(intersectPointList.at(1).x, 0, float(m_bufferwidth)));
			if (outX1 > outX2)std::swap(outX1, outX2);
			return TRUE;
		}
		else
		{
			//intersection failed, less than 2 points are intersected.
			return FALSE;
		}
		break;
	}

	//------------1 : one point lie on the line Y=y-----------------
	case 1:
	{
		FLOAT2 tmpVec;
		BOOL canOppositeLineSegmentIntersect = FALSE;
		switch (indexList_sameYCoord.at(0))
		{
		case 0:
			canOppositeLineSegmentIntersect = func_intersect(y, v2, v3, tmpVec);
			break;
		case 1:
			canOppositeLineSegmentIntersect = func_intersect(y, v1, v3, tmpVec);
			break;
		case 2:
			canOppositeLineSegmentIntersect = func_intersect(y, v1, v2, tmpVec);
			break;
		}
		if (canOppositeLineSegmentIntersect)
		{
			outX1 = UINT(MathInterface::Clamp(tmpVec.x, 0, float(m_bufferwidth)));//new intersect point
			outX2 = UINT(MathInterface::Clamp(trianglesVertices[indexList_sameYCoord.at(0)].x, 0, float(m_bufferwidth)));
			if (outX1 > outX2)std::swap(outX1, outX2);
			return true;
		}
		else
		{
			return false;
		}
		break;
	}
	//------------2 : 2 vertices lie on line Y=y-----------------
	case 2:
	{
		outX1 = UINT(MathInterface::Clamp(trianglesVertices[indexList_sameYCoord.at(1)].x, 0, float(m_bufferwidth)));//new intersect point
		outX2 = UINT(MathInterface::Clamp(trianglesVertices[indexList_sameYCoord.at(0)].x, 0, float(m_bufferwidth)));
		if (outX1 > outX2)std::swap(outX1, outX2);
		return true;
		break;
	}
		default:
			return false;
	}
	

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
void QRender::SetMaterial(const Material & material)
{
	m_Material = material;
}
void QRender::SetTexure(Texture2D * texture)
{
	m_pTexture = texture;
}
UINT QRender::GetBufferwidth()
{
	return m_bufferwidth;
}
UINT QRender::GetBufferheight()
{
	return m_bufferheight;
}
HDC QRender::GetHDC()
{
	return m_hdc;

}
void QRender::SetTexcoordTransform(float dx, float dy, float scale)
{
	mTexCoord_offsetX = dx;
	mTexCoord_offsetY = dy;
	mTexCoord_scale = scale;
}
void QRender::RenderMesh(Mesh & mesh)
{
	//process camera  and Matrix
	//------------------------------------------

	//----------------to do---------------------

	//--------------------------------------------
	
	SetTexure(mesh.m_texture);


	QRenderdrawcalldata drawCallData;
	drawCallData.offset = 0;
	drawCallData.pIndexbuffer = mesh.m_indexbuffer;
	drawCallData.pVertexbuffer = mesh.m_vertexbuffer;
	drawCallData.VertexCount = mesh.GetVertexCount();

	DrawTriangles(drawCallData);

}
void QRender::VertexShader(Vertex& invertex)
{
	VertexShaderOutput_Vertex outVertex;
	QVector pos(invertex.m_Position.x, invertex.m_Position.y, invertex.m_Position.z,1.0f);
	pos = pos *WorldMatrix;
	pos = pos *ViewMatrix;

	//float Z_ViewSpace = pos.z;
	pos = pos*ProjMatrix;
	//if (Z_ViewSpace >= 0)
	//{
	//	pos.x /= (Z_ViewSpace);
	//	pos.y /= (Z_ViewSpace);
	//}
	pos.x = pos.x / pos.w;
	pos.y = pos.y / pos.w;
	pos.z = pos.z / pos.w;
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
	outVertex.color = QVector(invertex.Color.r, invertex.Color.g, invertex.Color.b, invertex.Color.a);
	m_pVB_HomoSpace->push_back(outVertex);

}
UINT QRender::mFunction_QVectorConverttoINT(COLOR4 &Color )
{
	BYTE red = 255 * Color.x/*  color.w*/;
	BYTE green = 255 * Color.y/* color.w*/;
	BYTE blue = 255 * Color.z /* color.w*/;
	return (UINT)((BYTE)blue | (WORD)((BYTE)green << 8) | (DWORD)((BYTE)red << 16));
}