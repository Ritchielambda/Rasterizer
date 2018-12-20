#include"QRender.h"
QRender::QRender(HWND handle, int width,int height)
{
	hwnd = handle;
	WVP = MathInterface::MatrixIdentity();  //创建shader时，初始化变化矩阵。
	m_width = width;
	m_height = height;
	
	rasterizer = new Rasterizer(handle, width, height);
	rendermode = fillmode;
	Mat_Screen = MathInterface::MatrixScreenTransform(m_width,m_height);
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
void QRender::HomoSpaceClipping_Triangles()//等先改好了render的结构  再来修改这个函数
{

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
void QRender::VertexShader(Vertex& invertex)
{
	QVector pos(invertex.m_Position.x, invertex.m_Position.y, invertex.m_Position.z,1.0f);
	pos = pos *WorldMatrix;
	pos = pos *ViewMatrix;

	float Z_ViewSpace = pos.z;
	if (Z_ViewSpace >= 0)
	{
		pos.x /= (Z_ViewSpace);
		pos.y /= (Z_ViewSpace);
	}
}