#pragma once
#include"MathInterface.h"
#include"Rasterizer.h"
enum RenderMode
{
	wireframe,
	fillmode 
};
class QRender
{
public:
	QRender( HWND handle,int width,int height);

	void SetWVP(const Matrix& mat);

	void SetVertexbuffer(int Vertexsize, Vertex* data);

	void SetIndexbuffer(int Indexsize, int * data);



	void DrawIndexed();

	void Present();

	void toCVV(Vertex& vertex);

	Rasterizer * rasterizer;
private:

	HWND hwnd;

	RenderMode rendermode;

	int m_width;

	int m_height;

	Matrix WVP;

	Matrix Mat_Screen;

	ARGB Color;

	Vertex* Vertexbuffer;

	int* Indexbuffer;

	int Vertexsize;

	int Indexsize;

};