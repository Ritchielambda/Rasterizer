#pragma once
#include"MathInterface.h"
#include"FileLoader.h"
#include<vector>
class Mesh :private FileLoader
{
public:
	Mesh();
	~Mesh();
	void Settexture(Texture2D * texture);

	Texture2D* m_texture;
	UINT GetVertexCount();
	bool LoadFile_OBJ(std::wstring pFilePath);
	bool LoadTexture(std::wstring pFilePath);
	std::vector<Vertex>* m_vertexbuffer;
	std::vector<UINT>* m_indexbuffer;

private:
	Matrix						mMatrixWorld;
	//MATRIX4x4						mMatrixWorldInvTranspose;

};