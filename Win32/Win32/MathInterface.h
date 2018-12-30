#pragma once
#include"Matrix.h"
#include"Vector.h"
#include<math.h>
#include<Windows.h>
#include"typedef.h"
#include"Texture2D.h"
#include<vector>
namespace MathInterface
{


	UINT ColorToUINT(ARGB color);
	extern const float PI;
	float Lerp(const float&lhs, const float&rhs, float factor);
	FLOAT2 Lerp(const FLOAT2&lhs, const FLOAT2&rhs, float factor);
	FLOAT3 Lerp(const FLOAT3&lhs, const FLOAT3&rhs, float factor);
	Vertex Lerp(const Vertex& lhs, const Vertex & rhs, float factor);
	inline float ConvertToRadians(float fDegrees) { return fDegrees * (PI / 180.0f); }
	Matrix MatrixIdentity();
	Matrix MatrixTranspose(const Matrix& mat);
	float MatrixDet(const Matrix& mat);
	float MatrixAdjElem(
		float a1, float a2, float a3,
		float b1, float b2, float b3,
		float c1, float c2, float c3);




	//������� ��������ʽ��ɵľ����ת��
	Matrix MatrixAdj(const Matrix& mat);

	//����� = �������/(����ʽֵ�ľ���ֵ)
	Matrix MatrixInverse(const Matrix& mat);

	//����
	Matrix MatrixScaling(float ScaleX,float ScaleY,float ScaleZ);
	//ƽ��
	Matrix MatrixTranslate(float TransX, float TransY, float TransZ);
	//��ת����
	Matrix MatrixRotationX(float angle);
	Matrix MatrixRotationY(float angle);
	Matrix MatrixRotationZ(float angle);
	//��ȡ�ӽǾ���
	Matrix MatrixLookAtLH(QVector eyePos, QVector lookAt, QVector up);
	//����ͶӰ����
	Matrix MatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
	QVector Reflect(const QVector& I, const QVector& N);
	//��ͶӰ��任����Ļ�ռ�
	Matrix MatrixScreenTransform(int clientWidth, int clientHeight);


	 UINT Clamp(UINT val, UINT min, UINT max);

	  float Clamp(float val, float min, float max);

	  FLOAT2 Clamp(const FLOAT2 & val, const FLOAT2 & min, const FLOAT2 & max);

	  FLOAT3 Clamp(const FLOAT3 & val, const FLOAT3 & min, const FLOAT3 & max);


}
