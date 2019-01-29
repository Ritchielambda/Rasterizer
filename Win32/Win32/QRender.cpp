#include"MyGameEngine.h"
using namespace MathInterface;
QRender::QRender(HWND handle, UINT width,UINT height)
{
	hwnd = handle;
	WVP = MathInterface::MatrixIdentity();  //
	//rasterizer = new Rasterizer(handle, width, height);
	rendermode = fillmode;
	Mat_Screen = MathInterface::MatrixScreenTransform(m_bufferwidth,m_bufferheight);
	// hide cursor
	ShowCursor(false);
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
	m_CameraPos = { 0,0,0 };
	mLightEnabled = true;
	for (UINT i = 0; i < c_maxLightCount; ++i)memset(&mDirLight[i], 0, sizeof(DirectionalLight));


	Init(width, height,handle);
}



QRender::~QRender()
{

	//  Effective 16:  new and delete must be used in pair;
	//delete rasterizer;
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
	ClearZbuffer();
	mFunction_InitializeBitMap();
}
void QRender::SetWVP(const Matrix& mat)
{
	WVP = mat;
}

void QRender::Present(HWND& rhwnd)
{
	mFunction_UpdateBitMapBuffer();
	HDC hdc = GetDC(rhwnd);
	BitBlt(hdc, 0, 0, GetBufferwidth(), GetBufferheight(), GetHDC(), 0, 0, SRCCOPY);
	
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
	*m_pVB_HomoSpace_Clipped = std::move(*m_pVB_HomoSpace);//std::move

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
void QRender::HomoSpaceClipping_Points(std::vector<UINT>* pIB)
{
	*m_pIB_HomoSpace_Clipped = (*pIB);
	*m_pVB_HomoSpace_Clipped = std::move(*m_pVB_HomoSpace);

	UINT i = 0;
	while (i < m_pIB_HomoSpace_Clipped->size())
	{
		UINT idx = m_pIB_HomoSpace_Clipped->at(i);

		auto const v1 = m_pVB_HomoSpace_Clipped->at(idx);

		bool bOutOfBox = v1.posH.x <= -1.0f || v1.posH.x >= 1.0f ||
			v1.posH.y <= -1.0f || v1.posH.y >= 1.0f ||
			v1.posH.z <= 0.0f || v1.posH.z >= 1.0f;

		if (bOutOfBox)
		{
			UINT rubbishFragmentStartIndex = m_pIB_HomoSpace_Clipped->size() - 1;
			std::swap(m_pIB_HomoSpace_Clipped->at(i), m_pIB_HomoSpace_Clipped->at(rubbishFragmentStartIndex));
			//then pop back the rubbish indices
			m_pIB_HomoSpace_Clipped->pop_back();
		}
		else
		{
			++i;
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
	COLOR4 tmpcolor =  m_pTexture->GetPixel(UINT(pixelX), UINT(pixelY));
	return tmpcolor;
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
		return false;
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

void QRender::ClearScreen(COLOR4 clearColor,bool Z)
{
	for (UINT i = 0; i < m_bufferheight*m_bufferwidth; ++i)
	{
		m_pOutColorBuffer->at(i) = clearColor;
	}
	for (UINT i = 0; i < (*m_pZBuffer).size(); ++i)
	{
		//  after homogeneous space transform ,1 is biggest Z
		(*m_pZBuffer)[i] = Z;
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
	//Present();
}

void QRender::DrawPoint(QRenderdrawcalldata & drawCallData)
{
	m_pVB_HomoSpace->clear();
	m_pVB_HomoSpace_Clipped->clear();
	m_pIB_HomoSpace_Clipped->clear();
	m_pVB_Rasterized->clear();

	UINT offset = drawCallData.offset;
	UINT vCount = drawCallData.VertexCount;
	auto const pVB = drawCallData.pVertexbuffer;
	auto const pIB = drawCallData.pIndexbuffer;

	m_pVB_HomoSpace->reserve(vCount);
	m_pVB_HomoSpace->reserve(m_bufferwidth*m_bufferheight / 3);//3 is approximate estimation

	///---------------------VERTEX SHADER--------------------
	
	for (UINT i = offset; i < vCount; ++i)
	{
		Vertex& currVertex = pVB->at(offset + i);
		// draw point use VS process every vertex
		VertexShader(currVertex);
	}//--------------------HOMOSPACE CLIPPINT----------------------------
	HomoSpaceClipping_Points(pIB);

	//---------------------RASTERIZER----------------------------------------
	RasterizerPoints();

	//-----------------------PIXEL SHADER------------------------------------

	for (auto& rasterizedvertex : *m_pVB_Rasterized)
	{
		PixelShader_DrawPoints(rasterizedvertex);
	}
	
}

void QRender::mFunction_SetZ(UINT x, UINT y, float z)
{
	m_pZBuffer->at(y*m_bufferwidth + x) = z;
}

void QRender::mFunction_UpdateBitMapBuffer()
{




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
	//
	//
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



COLOR4 QRender::mFunction_VertexLighting(const FLOAT3 & vPosW, const FLOAT3 & vNormalW)
{
	//--------------for each vertex, perform Gouraud Shading ------------------

	QVector outColor = { 0.0f,0.0f,0.0f,0.0f };
	FLOAT3 unitNormal = vNormalW;
	unitNormal = unitNormal.Normalize();
	for (UINT i = 0; i < 1; ++i)
	{
		if (mDirLight[i].mIsEnabled)
		{
			//normalized light vector
			FLOAT3 unitIncomingLightVec = mDirLight[i].mDirection;
			unitIncomingLightVec.Normalize();

			//vector from current vertex to Camera(Eye),used when compute specular
			FLOAT3 toEye = m_CameraPos - vPosW;
			toEye.Normalize();

			//unit vertex normal
			FLOAT3 unitNormal = vNormalW;
			unitNormal.Normalize();

			//Ambient Color
			FLOAT3 currentAmbient = m_Material.ambient* mDirLight[i].mAmbientColor * m_Material.diffuse;

			//diffuse Factor (first make sure that angle <normal,light> is less than PI/2
			FLOAT3 currentDiffuse = { 0,0,0 };
			FLOAT3 currentSpecular = { 0,0,0 };

			float diffuseFactor = mDirLight[i].mDiffuseIntensity*MathInterface::Vec3_Dot((-1)*unitIncomingLightVec, unitNormal);
			if (diffuseFactor > 0.0f)
			{
				//diffuse color (eye pos independent)
				currentDiffuse = diffuseFactor * mDirLight[i].mDiffuseColor;

				//if Texture Mapping is disabled, then use pure diffuse color of material
				if (m_pTexture == nullptr)
				{
					//component-wise
					currentDiffuse = currentDiffuse * m_Material.diffuse;
				}
				//else the color will be passed down to pixel shader to multiply by 
				//per-pixel sample diffuse color


				//Specular color - eye position dependent
				/*VECTOR3 unitOutgoingLightVec = Vec3_Reflect(unitIncomingLightVec, unitNormal);
				float specFactor =
				mDirLight[i].mSpecularIntensity *
				pow(max(Vec3_Dot(unitOutgoingLightVec, toEye), 0.0f), mMaterial.specularSmoothLevel);

				//Vector3 * vector3 means component-wise mult , return vec3(x1*x2,y1*y2,z1*z2)
				currentSpecular = specFactor* mMaterial.specular * mDirLight[i].mSpecularColor;*/

			}

			FLOAT3 outColor3 = currentAmbient + currentDiffuse + currentSpecular;
			outColor += COLOR4(outColor3.x, outColor3.y, outColor3.z, 0.0f);

		}

	}
	return outColor;
}

void QRender::RasterizeTriangles()
{

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
			//real y is opposite to pixel_y
			outV.y = m_bufferheight - outV.y - 1;
		};
		FLOAT2 v1_pixel, v2_pixel, v3_pixel;//pixel space
		convertToPixelSpace(v1, v1_pixel);
		convertToPixelSpace(v2, v2_pixel);
		convertToPixelSpace(v3, v3_pixel);

		//Basis Vector, used to compute the bilinear interpolation coord (s,t) of current pixel
		FLOAT2 basisVector1 = v2_pixel - v1_pixel;
		FLOAT2 basisVector2 = v3_pixel - v1_pixel;

		float D = basisVector1.x*basisVector2.y - basisVector2.x*basisVector1.y;
		if (D == 0)
			continue;
		

		float minY = MathInterface::Clamp(min(min(v1_pixel.y, v2_pixel.y), v3_pixel.y), 0, float(m_bufferheight - 1));
		float maxY = MathInterface::Clamp(max(max(v1_pixel.y, v2_pixel.y), v3_pixel.y), 0, float(m_bufferheight - 1));

		for (int j = int(minY); j<int(maxY)+1; ++j)
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

void QRender::RasterizerPoints()
{
	for (UINT i = 0; i < m_pIB_HomoSpace_Clipped->size(); ++i)
	{
		RasterizedFragment outVertex;

		UINT index = m_pIB_HomoSpace_Clipped->at(i);
		const auto& v1 = m_pVB_HomoSpace_Clipped->at(index);

		auto converttoPixelSpace = [&](const const VertexShaderOutput_Vertex& v, FLOAT2& outV)
		{
			outV.x = float(m_bufferwidth)*(v.posH.x + 1.0f) / 2.0f;
			outV.y = float(m_bufferheight)*(v.posH.y + 1.0f) / 2.0f;
		};
		FLOAT2 v1_pixel;
		converttoPixelSpace(v1, v1_pixel);

		if (v1_pixel.x >= m_bufferwidth || v1_pixel.y >= m_bufferheight)
		{
			goto label_nextPixel;
		}

		float depth = v1.posH.z;
		if(mFunction_DepthTest(UINT(v1_pixel.x),UINT( v1_pixel.y), depth)==false) goto label_nextPixel;

		//I will use normal bilinear interpolation to see the result first
		outVertex.pixelX = UINT(v1_pixel.x);
		outVertex.pixelY = UINT(v1_pixel.y);

		//write to  depth buffer
		mFunction_SetZ(outVertex.pixelX, outVertex.pixelY, depth);

		//perspective correct interpolation
		outVertex.color = v1.color;
		outVertex.texcoord = v1.texcoord;

		m_pVB_Rasterized->push_back(outVertex);

	label_nextPixel:;
	}
}

void QRender::mFunction_SetPixel(UINT width, UINT height, COLOR4 color)
{
	m_pOutColorBuffer->at(height*m_bufferwidth + width) = color;
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

void QRender::PixelShader_DrawPoints(RasterizedFragment &inVertex)
{
	COLOR4 outColor;
	outColor = COLOR4(inVertex.color.x, inVertex.color.y, inVertex.color.z, inVertex.color.w);

	//draw a bigger point (4x4 pixel)
	int px1 = Clamp(inVertex.pixelX - 2, 0, m_bufferwidth);
	int px2 = Clamp(inVertex.pixelX + 2, 0, m_bufferwidth);
	int py1 = Clamp(inVertex.pixelY - 2, 0, m_bufferheight);
	int py2 = Clamp(inVertex.pixelY + 2, 0, m_bufferheight);
	for (int i = px1; i < px2; i++)
		for (int j = py1; j < py2; j++)
			m_pOutColorBuffer->at(j*m_bufferwidth + i) = outColor;
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
void QRender::SetCameraPos(FLOAT3 campos)
{
	m_CameraPos = campos;
}
void QRender::SetCamera(ICamera & cam)
{
	m_pCamera = &cam;
}
void QRender::SetLighting(int index, const DirectionalLight& light)
{
	mDirLight[index] = light;
}
void QRender::SetLightingEnabled(bool flag)
{
	mLightEnabled = flag;
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
void QRender::DrawRect(FLOAT2 LTPercentage, int width, int height,COLOR4 color)
{
	if (LTPercentage.x >= 1 || LTPercentage.y >= 1)
	{
		DEBUG_MSG1("Draw rect LTPercentage wrong!")
	}
	FLOAT2 StartPos = { LTPercentage.x*m_bufferwidth,LTPercentage.y*m_bufferheight };
	for (UINT i = StartPos.x; i < StartPos.x + width; ++i)
	{
		for (UINT j = StartPos.y; j < StartPos.y + height; ++j)
		{
			m_pOutColorBuffer->at(j*m_bufferwidth +i) = color;
		}
	}
}
bool QRender::DrawPicture(Texture2D &texture, UINT x1, UINT y1, UINT x2, UINT y2)
{
	x1 = Clamp(x1,0, GetBufferwidth() - 1);
	x2 = Clamp(x2, 0, GetBufferwidth() - 1);
	y1 = Clamp(y1, 0, GetBufferheight() - 1);
	y2 = Clamp(y2, 0, GetBufferheight() - 1);

	if (x1 >= x2 || y1 >= y2)
	{
		DEBUG_MSG1("drawpicture region wrong");
		return false;
	}
	UINT drawRegionWidth = x2 - x1 + 1;
	UINT drawRegionHeight = y2 - y1 + 1;

	//because Minification/Magnification could occur, we must find the 
	//x/y ratio coord first to filter
	for (UINT i = x1; i < x2; ++i)
	{
		float x_ratio = float(i - x1) / drawRegionWidth;
		for (UINT j = y1; j < y2; ++j)
		{
			float y_ratio = float(j - y1) / drawRegionHeight;
			UINT picCoordX = UINT(x_ratio*texture.GetWidth());
			UINT picCoordY = UINT(y_ratio*texture.GetHeight());
			mFunction_SetPixel(i, j, texture.GetPixel(picCoordX, picCoordY));
		}
	}
	return true;
}
void QRender::DrawLine(COLOR4 color, UINT x1, UINT y1, UINT x2, UINT y2)
{
	if (x1 == x2)
	{
		//if slope doesn't exist
		for (UINT j = y1; j <= y2; j++)
		{
			mFunction_SetPixel(x1, j, color);//pixel outside the boundary won't be drawn
		}
	}
	else
	{

		float	k = float(int(y2) - int(y1)) / float(int(x2) - int(x1));
		float k_inv = 1 / k;
		//bresenham-like line drawing

		//two circumstances
		//1. abs(slope)>=1; 
		//2. abs(slope)<=1;
		float offset = 0.0f;
		if (abs(k) <= 1.0f)
		{
			UINT i = x1;

			while (i != x2)
			{
				//UINT() forced type conversion will truncate the fraction part
				mFunction_SetPixel(i, y1 + UINT(offset), color);

				//anti-alising
				//mFunction_BlendPixel(i, y1 + UINT(offset)+ 1,  fractionPart(offset),color);
				offset += k;//dy = dx * k;

				if (x2 > x1)++i; else --i;
			}
		}
		else
		{
			UINT j = y1;
			while (j != y2)
			{
				mFunction_SetPixel(x1 + UINT(offset), j, color);

				//anti-alising
				//mFunction_BlendPixel(x1+UINT(offset) +1,j, fractionPart(offset),color);
				offset += (k_inv);

				if (y2 > y1)
				{
					++j;
				}
				else
				{
					--j;
				}
			}
		}
	}
}
void QRender::SetTexcoordTransform(float dx, float dy, float scale)
{
	mTexCoord_offsetX = dx;
	mTexCoord_offsetY = dy;
	mTexCoord_scale = scale;
}
void QRender::RenderMesh(Mesh & mesh)
{
	//process mesh material and world matrix;
	// camera done
	//------------------------------------------
	Matrix matW, matV, matP;

	mesh.GetWorldMatrix(matW);
	m_pCamera->GetViewMatrix(matV);
	m_pCamera->GetProjMatrix(matP);


	SetWordMatrix(matW);
	SetViewMatrix(matV);
	SetProjMatrix(matP);
	SetCameraPos(m_pCamera->GetPosition());
	SetMaterial(mesh.mMaterial);
	//if (mesh.m_texture->m_width == 0) DEBUG_MSG1("render mesh texture size wrong !");
	SetTexure(mesh.m_texture);

	SetLightingEnabled(true);

	QRenderdrawcalldata drawCallData;
	drawCallData.offset = 0;
	drawCallData.pIndexbuffer = mesh.m_indexbuffer;
	drawCallData.pVertexbuffer = mesh.m_vertexbuffer;
	drawCallData.VertexCount = mesh.GetVertexCount();

	DrawTriangles(drawCallData);
}
void QRender::RenderPointCollection(PointCollection & collection)
{
	if (m_pCamera == nullptr) return;
	Matrix matW, matV, matP;
	// how to update point world matrix
	matW.Identity();
	m_pCamera->GetViewMatrix(matV);
	m_pCamera->GetProjMatrix(matP);


	SetWordMatrix(matW);
	SetViewMatrix(matV);
	SetProjMatrix(matP);
	SetCameraPos(m_pCamera->GetPosition());
	SetTexure(nullptr);
	SetLightingEnabled(false);

	QRenderdrawcalldata drawCallData;
	drawCallData.offset = 0;
	drawCallData.pIndexbuffer = collection.m_pIB_Mem;
	drawCallData.pVertexbuffer = collection.m_pVB_Mem;
	drawCallData.VertexCount = collection.GetVertexCount();


	DrawPoint(drawCallData);

}
void QRender::VertexShader(Vertex& invertex)
{
	VertexShaderOutput_Vertex outVertex;
	QVector pos(invertex.m_Position.x, invertex.m_Position.y, invertex.m_Position.z,1.0f);
	pos = pos *WorldMatrix;
	pos = pos *ViewMatrix;
	float Z_ViewSpace = pos.z;
	//float Z_ViewSpace = pos.z;
	pos = pos*ProjMatrix;

	if (Z_ViewSpace >= 0)
	{
		pos.x = pos.x / pos.w;
		pos.y = pos.y / pos.w;
		pos.z = pos.z / pos.w;
		pos.w = 1.0f;
	}
	
	outVertex.posH = pos;
	//Matrix WorldMat_Trans = MathInterface::MatrixInverse(WorldMatrix);
	//WorldMat_Trans = MathInterface::MatrixTranspose(WorldMat_Trans);
	QVector Normal(invertex.m_Normal.x, invertex.m_Normal.y, invertex.m_Normal.z, 1.0f);
	//according to realtime rendering chapter2 ,with no scale transform ,normal transform matrix is world matrix
	Normal = Normal*WorldMatrix;

	//texture process
	outVertex.texcoord = FLOAT2(
		invertex.m_UV.x*mTexCoord_scale + mTexCoord_offsetX,
		invertex.m_UV.y*mTexCoord_scale + mTexCoord_offsetY);

	//lighting process          
	if (mLightEnabled)
	{
		outVertex.color = mFunction_VertexLighting(FLOAT3(outVertex.posH.x, outVertex.posH.y, outVertex.posH.z), FLOAT3(Normal.x, Normal.y, Normal.z));
	}
	else
	{
		outVertex.color =QVector(1.0,0,0,0);
	}
	
	m_pVB_HomoSpace->push_back(outVertex);

}
UINT QRender::mFunction_QVectorConverttoINT(COLOR4 &Color )
{
	BYTE red = 255 * Color.x/*  color.w*/;
	BYTE green = 255 * Color.y/* color.w*/;
	BYTE blue = 255 * Color.z /* color.w*/;
	return (UINT)((BYTE)blue | (WORD)((BYTE)green << 8) | (DWORD)((BYTE)red << 16));
}