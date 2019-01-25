#include"MyGameEngine.h"


Mesh::Mesh()
{
	m_vertexbuffer = new std::vector<Vertex>;
	m_indexbuffer = new std::vector<UINT>;
	m_texture = nullptr;
	
	mRotationX_Pitch = 0;
	mRotationY_Yaw = 0;
	mRotationZ_Roll = 0;

	mPosition = { 0,0,0 };
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
void Mesh::CreatePlane(float fWidth, float fDepth, UINT iRowCount, UINT iColumnCount)
{
	if (iColumnCount <= 2) { iColumnCount = 2; }
	if (iRowCount <= 2) { iRowCount = 2; }

	m_vertexbuffer->clear();
	m_indexbuffer->clear();

	//delegate vert/idx creation duty to MeshGenerator 
	mMeshGenerator.CreatePlane(fWidth, fDepth, iRowCount, iColumnCount, *m_vertexbuffer, *m_indexbuffer);

}
void Mesh::CreateBox(float fWidth, float fHeight, float fDepth, UINT iDepthStep, UINT iWidthStep, UINT iHeightStep)
{

	m_vertexbuffer->clear();
	m_indexbuffer->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateBox(fWidth, fHeight, fDepth, iDepthStep, iWidthStep, iHeightStep, *m_vertexbuffer, *m_indexbuffer);
}
void Mesh::CreateSphere(float fRadius, UINT iColumnCount, UINT iRingCount, BOOL bInvertNormal)
{
	//check if the input "Step Count" is illegal
	if (iColumnCount <= 3) { iColumnCount = 3; }
	if (iRingCount <= 1) { iRingCount = 1; }

	m_vertexbuffer->clear();
	m_indexbuffer->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateSphere(fRadius, iColumnCount, iRingCount, bInvertNormal, *m_vertexbuffer, *m_indexbuffer);

}
void Mesh::CreateCylinder(float fRadius, float fHeight, UINT iColumnCount, UINT iRingCount)
{
	if (iColumnCount <= 3) { iColumnCount = 3; }
	if (iRingCount <= 2) { iRingCount = 2; }

	m_vertexbuffer->clear();
	m_indexbuffer->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateCylinder(fRadius, fHeight, iColumnCount, iRingCount, *m_vertexbuffer, *m_indexbuffer);

}
void Mesh::Settexture(Texture2D * texture)
{
	m_texture = texture;//此处为深拷贝
}

void Mesh::SetPosition(float x, float y, float z)
{
	mPosition = FLOAT3(x,y,z);
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

void Mesh::ComputeBoundingBox(BOUNDINGBOX & outBox)
{
	mFunction_ComputeBoundingBox();
	outBox = mBoundingBox;
}

void Mesh::Destroy()
{
}

void Mesh::mFunction_UpdateWorldMatrix()
{

	Matrix tmpMatrix;

	tmpMatrix.Identity();

	tmpMatrix =MathInterface::MatrixPitchYawRoll(mRotationX_Pitch, mRotationY_Yaw, mRotationZ_Roll);

	//修改平移项
	tmpMatrix.m[3][0] = mPosition.x;
	tmpMatrix.m[3][1] = mPosition.y;
	tmpMatrix.m[3][3] = mPosition.z;

	mMatrixWorld = tmpMatrix;
}

void Mesh::mFunction_ComputeBoundingBox(std::vector<FLOAT3>* pVertexBuffer)
{
	mFunction_UpdateWorldMatrix();
	//计算包围盒.......重载1

	UINT i = 0;
	FLOAT3 tmpV;
	//遍历所有顶点，算出包围盒3分量均最 小/大 的两个顶点
	for (i = 0; i < m_vertexbuffer->size(); i++)
	{
		if (i == 0)
		{
			FLOAT3 pos = FLOAT3(m_vertexbuffer->at(0).m_Position.x, m_vertexbuffer->at(0).m_Position.y, m_vertexbuffer->at(0).m_Position.z);
			mBoundingBox.min = pos;
			mBoundingBox.max = pos;
		}

		//N_DEFAULT_VERTEX
		tmpV = FLOAT3(pVertexBuffer->at(i).x, pVertexBuffer->at(i).y, pVertexBuffer->at(i).z);
		//tmpV = Math::Matrix_Multiply(mMatrixWorld, VECTOR4(tmpV.x,tmpV.y,tmpV.z,1.0f));
		if (tmpV.x <(mBoundingBox.min.x)) { mBoundingBox.min.x = tmpV.x; }
		if (tmpV.y <(mBoundingBox.min.y)) { mBoundingBox.min.y = tmpV.y; }
		if (tmpV.z <(mBoundingBox.min.z)) { mBoundingBox.min.z = tmpV.z; }

		if (tmpV.x >(mBoundingBox.max.x)) { mBoundingBox.max.x = tmpV.x; }
		if (tmpV.y >(mBoundingBox.max.y)) { mBoundingBox.max.y = tmpV.y; }
		if (tmpV.z >(mBoundingBox.max.z)) { mBoundingBox.max.z = tmpV.z; }
	}

	mBoundingBox.max += mPosition;
	mBoundingBox.min += mPosition;
}

void Mesh::mFunction_ComputeBoundingBox()
{
	mFunction_UpdateWorldMatrix();
	//计算包围盒.......重载1

	UINT i = 0;
	FLOAT3 tmpV;
	//遍历所有顶点，算出包围盒3分量均最 小/大 的两个顶点
	for (i = 0; i < m_vertexbuffer->size(); i++)
	{
		if (i == 0)
		{
			mBoundingBox.min = FLOAT3(m_vertexbuffer->at(0).m_Position.x, m_vertexbuffer->at(0).m_Position.y, m_vertexbuffer->at(0).m_Position.z);
			mBoundingBox.max = FLOAT3(m_vertexbuffer->at(0).m_Position.x, m_vertexbuffer->at(0).m_Position.y, m_vertexbuffer->at(0).m_Position.z);
		}

		//N_DEFAULT_VERTEX
		tmpV = FLOAT3(m_vertexbuffer->at(i).m_Position.x, m_vertexbuffer->at(i).m_Position.y, m_vertexbuffer->at(i).m_Position.z);
		//tmpV = Math::Matrix_Multiply(mMatrixWorld, VECTOR4(tmpV.x,tmpV.y,tmpV.z,1.0f));
		if (tmpV.x <(mBoundingBox.min.x)) { mBoundingBox.min.x = tmpV.x; }
		if (tmpV.y <(mBoundingBox.min.y)) { mBoundingBox.min.y = tmpV.y; }
		if (tmpV.z <(mBoundingBox.min.z)) { mBoundingBox.min.z = tmpV.z; }

		if (tmpV.x >(mBoundingBox.max.x)) { mBoundingBox.max.x = tmpV.x; }
		if (tmpV.y >(mBoundingBox.max.y)) { mBoundingBox.max.y = tmpV.y; }
		if (tmpV.z >(mBoundingBox.max.z)) { mBoundingBox.max.z = tmpV.z; }
	}

	mBoundingBox.max += mPosition;
	mBoundingBox.min += mPosition;
}

inline FLOAT2 Mesh::mFunction_ComputeTexCoord_SphericalWrap(FLOAT3 vBoxCenter, FLOAT3 vPoint)
{

	FLOAT2 outTexCoord(0, 0);
	FLOAT3 tmpP = vPoint - vBoxCenter;

	//投影到单位球上
	tmpP.Normalize();

	//反三角函数算球坐标系坐标，然后角度值映射到[0,1]
	float angleYaw = 0.0f;
	float anglePitch = 0.0f;
	float tmpLength = sqrtf(tmpP.x*tmpP.x + tmpP.z*tmpP.z);

	// [ -PI/2 , PI/2 ]
	anglePitch = atan2(tmpP.y, tmpLength);

	// [ -PI	, PI ]
	angleYaw = atan2(tmpP.z, tmpP.x);

	//map to [0,1]
	outTexCoord.x = (angleYaw + MathInterface::PI) / (2.0f * MathInterface::PI);
	outTexCoord.y = (anglePitch + (MathInterface::PI / 2.0f)) / MathInterface::PI;

	return outTexCoord;
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
	m_texture->LoadBitmapToColorArray(pFilePath);
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
