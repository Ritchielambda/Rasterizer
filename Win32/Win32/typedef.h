#pragma once
#include<vector>
#include"Vector.h"
#include<Windows.h>

struct ARGB
{
	ARGB() { a = r = g = b = 0; }
	ARGB(float pr, float pg, float pb, float pa) : r(pr), g(pg), b(pb), a(pa) {}
	ARGB operator -(const ARGB& rhs) const { return ARGB(this->r - rhs.r, this->g - rhs.g, this->b - rhs.b, this->a - rhs.a); }
	ARGB operator +(const ARGB& rhs)const { return ARGB(this->r + rhs.r, this->g + rhs.g, this->b + rhs.b, this->a + rhs.a); }
	ARGB operator /(float rhs) { return ARGB(this->r / rhs, this->g / rhs, this->b / rhs, this->a / rhs); }
	ARGB& operator +=(ARGB& rhs) { this->r += rhs.r; this->g += rhs.g; this->b += rhs.b; this->a += rhs.a; return *this; }
	ARGB operator *(float rhs) { return ARGB(this->r * rhs, this->g * rhs, this->b * rhs, this->a * rhs); }
	ARGB& operator =(ARGB& rhs) { this->a = rhs.a; this->r = rhs.r; this->g = rhs.g; this->b = rhs.b; return *this; }
	~ARGB() {}
	float r;
	float g;
	float b;
	float a;
};
struct Vertex //顶点的w分量是1，因为进行矩阵变换的时候可以做平移
{
	Vertex() = default;
	Vertex(float posx, float posy, float posz, float posa, ARGB color, FLOAT3 normal = FLOAT3(0, 0, 0)) :m_Position(posx, posy, posz, posa), Color(color), m_Normal(normal) {}
	Vertex& operator +=(Vertex rhs) { this->m_Position.x += rhs.m_Position.x; this->m_Position.y += rhs.m_Position.y; this->Color += rhs.Color; return *this; }
	Vertex& operator *=(const Matrix& rhs) { m_Position = m_Position*rhs; return *this; }
	QVector m_Position;
	FLOAT2 m_UV;
	FLOAT3 m_Normal;
	ARGB Color;
	float Divz;
};

struct OBJ_vertexInfoIndex
{
	OBJ_vertexInfoIndex()
	{
		vertexID = texcoordID = vertexNormalID = 0;
	};

	OBJ_vertexInfoIndex(int vID, int texcID, int vnID)
	{
		vertexID = vID;
		texcoordID = texcID;
		vertexNormalID = vnID;
	}

	inline bool operator==(OBJ_vertexInfoIndex const& v)const
	{
		if (vertexID == v.vertexID && texcoordID == v.texcoordID && vertexNormalID == v.vertexNormalID)
		{
			return true;
		}
		return false;
	}

	int vertexID;
	int texcoordID;
	int vertexNormalID;
};
//struct VertexShaderOut_Vertex
//{
//	QVector m_position;
//	FLOAT2 m_texture;
//	FLOAT3 m_Normal;
//};
typedef QVector COLOR4;
struct QRenderdrawcalldata
{
	std::vector<Vertex>* pVertexbuffer;
	std::vector<UINT>* pIndexbuffer;
	UINT offset;
	UINT VertexCount;
};
struct VertexShaderOutput_Vertex
{
	QVector posH;//homogenous position
	QVector color;
	FLOAT2 texcoord;

	//Gouraud shading don't need to pass down the posW and normalW to pixel shader
	//while Phong shading (per-pixel) need it
};
struct RasterizedFragment
{
	UINT pixelX;
	UINT pixelY;
	QVector color;
	FLOAT2 texcoord;
	//VECTOR3 normal;
};
struct Material
{
	Material()
	{
		ambient = { 0,0,0 };
		diffuse = { 1.0f,0,0 };
		specular = { 1.0f,1.0f,1.0f };
		specularSmoothLevel = 10;
	};
	Material& operator=(const Material & mat) { this->ambient = mat.ambient; this->diffuse = mat.diffuse; this->specular = mat.specular; this->specularSmoothLevel = mat.specularSmoothLevel; return *this; }
	FLOAT3 ambient;
	FLOAT3 diffuse;
	FLOAT3 specular;
	UINT   specularSmoothLevel;

};
struct DirectionalLight
{
public:
	DirectionalLight()
	{
		ZeroMemory(this, sizeof(*this));
		mSpecularIntensity = 1.0f;
		mDirection = FLOAT3(1.0f, 0, 0);
		mDiffuseIntensity = 0.5;
		mIsEnabled = true;
	}

	bool operator==(DirectionalLight& Light)
	{
		//two memory fragments are identical
		if (memcmp(this, &Light, sizeof(Light)) == 0)
		{
			return true;
		}
		return false;
	}

	FLOAT3 mAmbientColor;	 float		mSpecularIntensity;
	FLOAT3 mDiffuseColor;		float			mDiffuseIntensity;
	FLOAT3 mSpecularColor;	 bool		mIsEnabled;
	FLOAT3 mDirection;
};