#pragma once
#include"MathInterface.h"
#include<vector>
class Mesh
{
public:
	Mesh();
	void Settexture(Texture2D * texture);

	std::vector<Vertex> m_vertexbuffer;
	std::vector<int> m_indexbuffer;
	Texture2D* m_texture;
};