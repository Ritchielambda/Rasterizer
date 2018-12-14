#pragma once
#include"MathInterface.h"
class QVShader
{
public:
	QVShader();
	void SetWVP(const Matrix& mat);
	void SetVertexbuffer(int Vertexsize, Vertex* data);
	void SetIndexbuffer(int Indexsize, int * data);
	void DrawIndexed();
private:
	Matrix WVP;
	ARGB Color;
	Vertex* Vertexbuffer;
	int* Indexbuffer;
	int Vertexsize;
	int Indexsize;
};