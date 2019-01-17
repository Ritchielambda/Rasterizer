#pragma once

class Mesh :private FileLoader
{
	friend class QRender;
public:
	Mesh();
	~Mesh();

	void	CreatePlane(float fWidth, float fDepth, UINT iRowCount = 5, UINT iColumnCount = 5);

	void CreateBox(float fWidth, float fHeight, float fDepth, UINT iDepthStep = 3, UINT iWidthStep = 3, UINT iHeightStep = 3);

	void	CreateSphere(float fRadius, UINT iColumnCount = 20, UINT iRingCount = 20, BOOL bInvertNormal = FALSE);

	void CreateCylinder(float fRadius, float fHeight, UINT iColumnCount = 40, UINT iRingCount = 8);


	bool LoadFile_OBJ(std::wstring pFilePath);
	bool LoadTexture(std::wstring pFilePath);

	void	SetMaterial(const Material& material);

	void	SetColor(const QVector& color);
		
	void	Settexture(Texture2D * texture);

	void	SetPosition(float x, float y, float z);

	void	SetPosition(const FLOAT3& pos);

	void	SetRotation(float angleX, float angleY, float angleZ);

	void	SetRotationX_Pitch(float angleX);

	void	SetRotationY_Yaw(float angleY);

	void	SetRotationZ_Roll(float angleZ);

	FLOAT3  GetPosition();

	void	RotateX_Pitch(float angle);

	void	RotateY_Yaw(float angle);

	void	RotateZ_Roll(float angle);

	float	GetRotationY_Yaw();

	float	GetRotationX_Pitch();

	float	GetRotationZ_Roll();

	//In order to avoid directly compute inverse of Matrix

	void		GetWorldMatrix(Matrix& outMat);

	UINT		GetVertexCount();

	void		GetVertex(UINT iIndex, Vertex& outVertex);

	void		GetVertexBuffer(std::vector<Vertex>& outBuff);

	void		ComputeBoundingBox(BOUNDINGBOX& outBox);
private:
	Matrix						mMatrixWorld;

	GeometryMeshGenerator	mMeshGenerator;

	BOUNDINGBOX				mBoundingBox;
	FLOAT3   mPosition;

	float		mRotationX_Pitch;
	float		mRotationY_Yaw;
	float		mRotationZ_Roll;

	void		Destroy();

	void		mFunction_UpdateWorldMatrix();

	void		mFunction_ComputeBoundingBox(std::vector<FLOAT3>* outBuff);

	void		mFunction_ComputeBoundingBox();

	inline FLOAT2 mFunction_ComputeTexCoord_SphericalWrap(FLOAT3 vBoxCenter, FLOAT3 vPoint);

	Material mMaterial;

	Texture2D* m_texture;

	std::vector<Vertex>* m_vertexbuffer;
	std::vector<UINT>* m_indexbuffer;

};