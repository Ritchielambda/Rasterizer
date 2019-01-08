#pragma once
#include"MathInterface.h"
#include"FileLoader.h"
#include<vector>
class Mesh :private FileLoader
{
	friend class QRender;
public:
	Mesh();
	~Mesh();




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

	//void		ComputeBoundingBox(BOUNDINGBOX& outBox);
private:
	Matrix						mMatrixWorld;


	FLOAT3   mPosition;

	float		mRotationX_Pitch;
	float		mRotationY_Yaw;
	float		mRotationZ_Roll;

	void		Destroy();

	void		mFunction_UpdateWorldMatrix();


	Material mMaterial;

	Texture2D* m_texture;

	std::vector<Vertex>* m_vertexbuffer;
	std::vector<UINT>* m_indexbuffer;

};