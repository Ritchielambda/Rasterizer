#pragma once
#include<vector>
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