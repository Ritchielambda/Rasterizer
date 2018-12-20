#pragma once
#include"MathInterface.h"
#include"Rasterizer.h"
#include"typedef.h"
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

	void QRender::SetTexcoordTransform(float dx, float dy, float scale);

	//there is no need to provide these two interface , rendere will be call only render mesh,
	//class QRender don't need to save input data
	//void SetVertexbuffer(int Vertexsize, std::vector<Vertex>* data);

	//void SetIndexbuffer(int Indexsize, std::vector<int> *data);

	

	void DrawIndexed();

	void Present();

	void toCVV(Vertex& vertex);

	Rasterizer * rasterizer;


private:

	HWND hwnd;

	RenderMode rendermode;

	int m_bufferwidth;

	int m_bufferheight;
	//------------------------pipeline stage----------------------

	Matrix WVP;

	Matrix WorldMatrix;

	Matrix ProjMatrix;

	Matrix ViewMatrix;

	Matrix Mat_Screen;

	void VertexShader(Vertex& invertex);

	void HomoSpaceClipping_Triangles(std::vector<UINT>* const pIB);

	void DrawTriangles(QRenderdrawcalldata & drawCallData);

	void RasterizeTriangles();

	std::vector<VertexShaderOutput_Vertex>*		m_pVB_HomoSpace;//vertices in homogeous clipping space

	std::vector<VertexShaderOutput_Vertex>*	 m_pVB_HomoSpace_Clipped;//after clipping

	std::vector<UINT>*										m_pIB_HomoSpace_Clipped;

	std::vector<RasterizedFragment>*		m_pVB_Rasterized;//vertices attribute have been interpolated

	//ARGB Color;

	float						mTexCoord_offsetX;//texcoord transformation info

	float						mTexCoord_offsetY;

	float						mTexCoord_scale;

	std::vector<Vertex>* InVertexbuffer;

	std::vector<int>* InIndexbuffer;

	int Vertexsize;

	int Indexsize;


};