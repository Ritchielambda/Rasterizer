#include"QVShader.h"
QVShader::QVShader()
{
	WVP = MathInterface::MatrixIdentity();  //创建shader时，初始化变化矩阵。
}
void QVShader::SetWVP(const Matrix& mat)
{
	WVP = mat;
}

void QVShader::SetVertexbuffer(int PVertexsize, Vertex* data)
{
	if (Vertexbuffer) delete Vertexbuffer;
	Vertexsize = PVertexsize;
	Vertexbuffer = data;
}
void QVShader::SetIndexbuffer(int PIndexsize, int * data)
{
	if (Indexbuffer) delete Indexbuffer;
	Indexsize = PIndexsize;
	Indexbuffer = data;
}
void QVShader::DrawIndexed()//it's a small project,only support draw triangles with every three vertexs
{
	if ((!Vertexbuffer) && (!Indexbuffer)) return;
	for (int i = 0; i < Indexsize; i+=3)//  do transforming from obj space to screen space
	{
		Vertexbuffer[Indexbuffer[i]] *= WVP;
	}
}