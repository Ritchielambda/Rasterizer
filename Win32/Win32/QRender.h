#pragma once

enum RenderMode
{
	wireframe,
	fillmode 
};
class QRender
{
public:
	QRender( HWND handle,UINT width,UINT height);


	~QRender();
	void Init(UINT bufferwidth, UINT bufferheight,HWND handle);

	//SET function    for out interface
	void SetWVP(const Matrix& mat);

	void ClearZbuffer();

	void ClearScreen(COLOR4 clearColor = { 0,0,0,0 }, bool Z = true);
	
	void SetWordMatrix(Matrix wordMatrix);

	void SetViewMatrix(Matrix viewMatrix);

	void SetProjMatrix(Matrix projMatirx);

	void SetMaterial(const Material& material);

	void SetTexure(Texture2D* texture);

	void SetCameraPos(FLOAT3 campos);

	void SetCamera(ICamera& cam);

	void SetLighting(int index,const DirectionalLight& light);

	void SetLightingEnabled(bool );

	UINT GetBufferwidth();

	UINT GetBufferheight();

	HDC GetHDC();

	void DrawRect(FLOAT2 lefttopPos, int width, int height,COLOR4 color);

	bool DrawPicture(Texture2D& texture,UINT x1,UINT y1,UINT x2,UINT y2);

	void DrawLine(COLOR4 color, UINT x1, UINT y1, UINT x2, UINT y2);

	void QRender::SetTexcoordTransform(float dx, float dy, float scale);
	
	void RenderMesh(Mesh & mesh);

	void RenderPointCollection(PointCollection& collection);


	void Present(HWND& hwnd);

	//there is no need to provide these two interface , rendere will be call only render mesh,
	//class QRender don't need to save input data
	//void SetVertexbuffer(int Vertexsize, std::vector<Vertex>* data);

	//void SetIndexbuffer(int Indexsize, std::vector<int> *data);

	

	void toCVV(Vertex& vertex);

	//Rasterizer * rasterizer;

	HWND hwnd;
private:
	
	HDC  m_hdc;
	UINT m_bufferwidth;
	UINT m_bufferheight;

	HBITMAP m_outputBitMap;
	BITMAP bm;
	HBITMAP m_OldBitmap;
	UINT*    m_BitMapBuffer;

	ICamera * m_pCamera;
	FLOAT3    m_CameraPos;
	RenderMode rendermode;



	std::vector<float>*		m_pZBuffer;//depth buffer

	
	//------------------------pipeline stage----------------------

	Matrix WVP;

	Matrix WorldMatrix;

	Matrix ProjMatrix;

	Matrix ViewMatrix;

	Matrix Mat_Screen;

	static const UINT	c_maxLightCount = 8;
	bool					mLightEnabled;
	DirectionalLight	mDirLight[c_maxLightCount];//"IsEnabled"control whether to enable a light in one draw call

	void VertexShader(Vertex& invertex);

	void HomoSpaceClipping_Triangles(std::vector<UINT>* const pIB);

	void	HomoSpaceClipping_Points(std::vector<UINT>* pIB);

	void DrawTriangles(QRenderdrawcalldata & drawCallData);

	void DrawPoint(QRenderdrawcalldata & drawCallData);

	void RasterizeTriangles();

	void RasterizerPoints();

	void mFunction_SetPixel(UINT width, UINT height, COLOR4 color);

	void PixelShader_DrawTriangles(RasterizedFragment&);

	void PixelShader_DrawPoints(RasterizedFragment&);
















	//--------------------------------Helper Function-------------------------------------------------------------------------

	bool mFunction_HorizontalIntersect(float y, const FLOAT2& v1, const FLOAT2& v2, const FLOAT2& v3, UINT& outX1, UINT& outX2);

	COLOR4 mFunction_SampleTexture(float x,float y);

	bool mFunction_DepthTest(UINT x, UINT y, float testZ);

	void mFunction_SetZ(UINT x, UINT y, float z);

	UINT QRender::mFunction_QVectorConverttoINT(COLOR4 &Color);

	void mFunction_UpdateBitMapBuffer();

	void mFunction_InitializeBitMap();


	COLOR4 mFunction_VertexLighting(const FLOAT3& vPosW, const FLOAT3& vNormalW);

	std::vector<VertexShaderOutput_Vertex>*		m_pVB_HomoSpace;//vertices in homogeous clipping space

	std::vector<VertexShaderOutput_Vertex>*	    m_pVB_HomoSpace_Clipped;//after clipping

	std::vector<UINT>*							m_pIB_HomoSpace_Clipped;

	std::vector<RasterizedFragment>*		    m_pVB_Rasterized;//vertices attribute have been interpolated

	std::vector<COLOR4>*						m_pOutColorBuffer;//output color buffer

	Material                                    m_Material;

	std::vector<float> vec_diffusefactor;
	Texture2D*									m_pTexture;//current using texture

	float										mTexCoord_offsetX;//texcoord transformation info

	float										mTexCoord_offsetY;

	float										mTexCoord_scale;

	std::vector<Vertex>* InVertexbuffer;

	std::vector<int>* InIndexbuffer;

	int Vertexsize;

	int Indexsize;



};