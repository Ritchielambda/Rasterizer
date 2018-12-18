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
struct VertexShaderOut_Vertex
{
	QVector m_position;
	FLOAT2 m_texture;
	FLOAT3 m_Normal;
};
struct QRenderdrawcalldata
{
	std::vector<Vertex> m_Vertexbuffer;
	std::vector<int> m_Indexbuffer;
	UINT offset;
	UINT VertexCount;
};