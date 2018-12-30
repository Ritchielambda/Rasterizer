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




	//伴随矩阵 代数余子式组成的矩阵的转置
	Matrix MatrixAdj(const Matrix& mat);

	//逆矩阵 = 伴随矩阵/(行列式值的绝对值)
	Matrix MatrixInverse(const Matrix& mat);

	//缩放
	Matrix MatrixScaling(float ScaleX,float ScaleY,float ScaleZ);
	//平移
	Matrix MatrixTranslate(float TransX, float TransY, float TransZ);
	//旋转矩阵
	Matrix MatrixRotationX(float angle);
	Matrix MatrixRotationY(float angle);
	Matrix MatrixRotationZ(float angle);
	//获取视角矩阵
	Matrix MatrixLookAtLH(QVector eyePos, QVector lookAt, QVector up);
	//设置投影矩阵
	Matrix MatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
	QVector Reflect(const QVector& I, const QVector& N);
	//从投影面变换到屏幕空间
	Matrix MatrixScreenTransform(int clientWidth, int clientHeight);


	 UINT Clamp(UINT val, UINT min, UINT max);

	  float Clamp(float val, float min, float max);

	  FLOAT2 Clamp(const FLOAT2 & val, const FLOAT2 & min, const FLOAT2 & max);

	  FLOAT3 Clamp(const FLOAT3 & val, const FLOAT3 & min, const FLOAT3 & max);


}
