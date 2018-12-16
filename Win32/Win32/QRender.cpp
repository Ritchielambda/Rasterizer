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

void QRender::SetVertexbuffer(int PVertexsize, Vertex* data)
{
	if (Vertexbuffer == data) return;
	//if (Vertexbuffer) delete Vertexbuffer;
	Vertexsize = PVertexsize;
	Vertexbuffer = data;
}
void QRender::SetIndexbuffer(int PIndexsize, int * data)
{
	if (Indexbuffer == data) return;
	//if (Indexbuffer) delete Indexbuffer;
	Indexsize = PIndexsize;
	Indexbuffer = data;
}
void QRender::DrawIndexed()//it's a small project,only support draw triangles with every three vertexs
{
	// test only draw one obj  a time   clear buffer here
	
	if ((!Vertexbuffer) && (!Indexbuffer)) return;
	for (int i = 0; i < Indexsize; i += 3)//  do transforming from obj space to screen space
	{
		Vertex v1 = Vertexbuffer[Indexbuffer[i]];
		Vertex v2 = Vertexbuffer[Indexbuffer[i+1]];
		Vertex v3 = Vertexbuffer[Indexbuffer[i+2]];
		v1 *= WVP;
		v2 *= WVP;
		v3*= WVP;
		v1.Divz = 1 / v1.m_Position.w;
		v2.Divz = 1 / v2.m_Position.w;
		v3.Divz = 1 / v3.m_Position.w;
		toCVV(v1);
		toCVV(v2);
		toCVV(v3);
		v1 *= Mat_Screen;
		v2 *= Mat_Screen;
		v3 *= Mat_Screen;
		rasterizer->drawtriangles(v1, v2, v3);
	}

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
