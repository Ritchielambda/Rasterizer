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

	void SetWordMatrix(Matrix wordMatrix);

	void SetViewMatrix(Matrix viewMatrix);

	void SetProjMatrix(Matrix projMatirx);

	//there is no need to provide these two interface , rendere will be call only render mesh,
	//class QRender don't need to save input data
	//void SetVertexbuffer(int Vertexsize, std::vector<Vertex>* data);

	//void SetIndexbuffer(int Indexsize, std::vector<int> *data);

	void VertexShader(Vertex& invertex);

	void HomoSpaceClipping_Triangles();

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

	Matrix WorldMatrix;

	Matrix ProjMatrix;

	Matrix ViewMatrix;

	Matrix Mat_Screen;

	ARGB Color;

	std::vector<Vertex>* InVertexbuffer;

	std::vector<int>* InIndexbuffer;

	int Vertexsize;

	int Indexsize;


};