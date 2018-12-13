#pragma once
#include"Matrix.h"
#include"Vector.h"
#include<math.h>
#include"Grometry.h"
namespace MathInterface
{
	extern const float PI;
	float Lerp(float&lhs, float&rhs, float factor);
	FLOAT2 Lerp(FLOAT2&lhs, FLOAT2&rhs, float factor);
	FLOAT3 Lerp(FLOAT3&lhs, FLOAT3&rhs, float factor);
	Vertex Lerp(Vertex& lhs, Vertex & rhs, float factor);
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
}
