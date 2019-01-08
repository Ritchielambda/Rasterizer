#include"Mesh.h"
Mesh::Mesh()
{
	m_vertexbuffer = new std::vector<Vertex>;
	m_indexbuffer = new std::vector<UINT>;
	m_texture = nullptr;
	


	Material defaultMat;
	defaultMat.ambient = { 0.2f,0.2f,0.2f };
	defaultMat.diffuse = { 0.8f,0.8f,0.8f };
	defaultMat.specular = { 1.0f,1.0f,1.0f };
	defaultMat.specularSmoothLevel = 20;
	Mesh::SetMaterial(defaultMat);
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

void Mesh::SetPosition(float x, float y, float z)
{
}

void Mesh::SetPosition(const FLOAT3 & pos)
{
	mPosition = pos;
}

void Mesh::SetRotation(float angleX, float angleY, float angleZ)
{
	mRotationX_Pitch = angleX;
	mRotationY_Yaw = angleY;
	mRotationZ_Roll = angleZ;
}

void Mesh::SetRotationX_Pitch(float angleX)
{
	mRotationX_Pitch = angleX;
}

void Mesh::SetRotationY_Yaw(float angleY)
{
	mRotationY_Yaw = angleY;
}

void Mesh::SetRotationZ_Roll(float angleZ)
{
	mRotationZ_Roll = angleZ;
}

FLOAT3 Mesh::GetPosition()
{
	return mPosition;
	
}

void Mesh::RotateX_Pitch(float angle)
{
	SetRotationX_Pitch(mRotationX_Pitch + angle);
}

void Mesh::RotateY_Yaw(float angle)
{
	SetRotationY_Yaw(mRotationY_Yaw + angle);
}

void Mesh::RotateZ_Roll(float angle)
{
	SetRotationZ_Roll(mRotationZ_Roll + angle);
}

float Mesh::GetRotationY_Yaw()
{
	return mRotationY_Yaw;
}

float Mesh::GetRotationX_Pitch()
{
	return mRotationX_Pitch;
}

float Mesh::GetRotationZ_Roll()
{
	return mRotationZ_Roll;
}

void Mesh::GetWorldMatrix(Matrix & outMat)
{
	mFunction_UpdateWorldMatrix();
	outMat = mMatrixWorld;
}

UINT Mesh::GetVertexCount()
{
	return m_vertexbuffer->size();
}

void Mesh::GetVertex(UINT iIndex, Vertex & outVertex)
{
	if (iIndex < m_vertexbuffer->size())
	{
		outVertex = m_vertexbuffer->at(iIndex);
	}
}

void Mesh::GetVertexBuffer(std::vector<Vertex>& outBuff)
{
	std::vector<Vertex>::iterator iterBegin, iterLast;
	iterBegin = m_vertexbuffer->begin();
	iterLast = m_vertexbuffer->end();
	outBuff.assign(iterBegin, iterLast);
}

void Mesh::Destroy()
{
}

void Mesh::mFunction_UpdateWorldMatrix()
{

	Matrix tmpMatrix;

	tmpMatrix.Identity();

	tmpMatrix =MathInterface::MatrixPitchYawRoll(mRotationY_Yaw, mRotationX_Pitch, mRotationZ_Roll);

	//修改平移项
	tmpMatrix.m[0][3] = mPosition.x;
	tmpMatrix.m[1][3] = mPosition.y;
	tmpMatrix.m[2][3] = mPosition.z;

	mMatrixWorld = tmpMatrix;
}

bool Mesh::LoadFile_OBJ(std::wstring pFilePath)
{

	bool fileLoadSucceeded = false;
	fileLoadSucceeded = FileLoader::LoadObjFile(pFilePath, *m_vertexbuffer, *m_indexbuffer);
	if(!fileLoadSucceeded)return false;
	return true;
}

bool Mesh::LoadTexture(std::wstring pFilePath)
{
	bool fileLoadSucceeded = false;
	m_texture = FileLoader::LoadBitmapToColorArray(pFilePath);
	if (!fileLoadSucceeded)return false;
	return true;

}

void Mesh::SetMaterial(const Material & material)
{
	mMaterial = material;
}

void Mesh::SetColor(const QVector & color)
{
	for (auto& v : *m_vertexbuffer)
	{
		v.Color = color;
	}
}
