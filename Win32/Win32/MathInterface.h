#pragma once
#include"Matrix.h"
#include"Vector.h"
#include<math.h>
#include<Windows.h>
#include"Texture2D.h"
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <sstream>
struct ARGB
{
	ARGB() { a = r = g = b = 0; }
	ARGB(float pa, float pr, float pg, float pb) :a(pa), r(pr), g(pg), b(pb) {}
	ARGB operator -(ARGB& rhs) { return ARGB(this->a - rhs.a, this->r - rhs.r, this->g - rhs.g, this->b - rhs.b); }
	ARGB operator +(ARGB& rhs) { return ARGB(this->a + rhs.a, this->r + rhs.r, this->g + rhs.g, this->b + rhs.b); }
	ARGB operator /(float rhs) { return ARGB(this->a / rhs, this->r / rhs, this->g / rhs, this->b / rhs); }
	ARGB& operator +=(ARGB& rhs) { return ARGB(this->a += rhs.a, this->r += rhs.r, this->g += rhs.g, this->b += rhs.b); return *this; }
	ARGB operator *(float rhs) { return ARGB(this->a * rhs, this->r * rhs, this->g * rhs, this->b * rhs); }
	~ARGB() {}
	float a;
	float r;
	float g;
	float b;
};
struct Vertex
{
	Vertex(float posx, float posy, float posz, float posa, ARGB color, FLOAT3 normal = FLOAT3(0, 0, 0)) :m_Position(posx, posy, posz, posa), Color(color), m_Normal(normal) {}
	Vertex& operator +=(Vertex rhs) { this->m_Position.x += rhs.m_Position.x; this->m_Position.y += rhs.m_Position.y; this->Color += rhs.Color; return *this; }
	Vertex& operator *=(const Matrix& rhs) { m_Position = m_Position*rhs; return *this; }
	QVector m_Position;
	FLOAT2 m_UV;
	FLOAT3 m_Normal;
	ARGB Color;
};
UINT ColorToUINT(ARGB color);
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
	Texture2D LoadBitmapToColorArray(std::wstring filePath);
}
