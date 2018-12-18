#include"Mesh.h"
Mesh::Mesh()
{
	
}
void Mesh::Settexture(Texture2D * texture)
{
	m_texture = texture;//此处为深拷贝
}