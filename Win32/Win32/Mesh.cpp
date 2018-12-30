#include"Mesh.h"
Mesh::Mesh()
{
	m_vertexbuffer = new std::vector<Vertex>;
	m_indexbuffer = new std::vector<UINT>;
	m_texture = nullptr;
	
}
Mesh::~Mesh()
{
	delete m_vertexbuffer;
	delete m_indexbuffer;
	if (m_texture) delete m_texture;
}
void Mesh::Settexture(Texture2D * texture)
{
	m_texture = texture;//此处为深拷贝
}

UINT Mesh::GetVertexCount()
{
	return m_vertexbuffer->size();
}

bool Mesh::LoadFile_OBJ(std::wstring pFilePath)
{

	bool fileLoadSucceeded = FALSE;
	fileLoadSucceeded = FileLoader::LoadObjFile(pFilePath, *m_vertexbuffer, *m_indexbuffer);
	if(!fileLoadSucceeded)return false;
	return true;
}

bool Mesh::LoadTexture(std::wstring pFilePath)
{
	bool fileLoadSucceeded = FALSE;
	m_texture = FileLoader::LoadBitmapToColorArray(pFilePath);
	if (!fileLoadSucceeded)return false;
	return true;

}
