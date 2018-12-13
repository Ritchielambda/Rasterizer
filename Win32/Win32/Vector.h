#pragma once
#include"Matrix.h"
#include<math.h>
struct FLOAT2
{
	FLOAT2() { x = y = 0; }
	FLOAT2(float x1, float y1) :x(x1), y(y1) {}
	float x;
	float y;
};
struct FLOAT3
{
	FLOAT3() { x = y = z = 0; }
	FLOAT3(float x1, float y1, float z1) :x(x1), y(y1), z(z1) {}
	float x;
	float y;
	float z;
};
class QVector
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	QVector(float px, float py, float pz, float pw) :x(px), y(py), z(pz), w(pw) {}
	QVector() = default;
	QVector(const QVector & rhs):x(rhs.x),y(rhs.y),z(rhs.z),w(rhs.w){}
	QVector&operator = (const QVector&rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }


public:
	float Length();
	QVector Normalize();
	float dot(const QVector &rhs);
	QVector Cross(const QVector& rhs)const; 
	QVector operator+(const QVector & rhs)const;
	QVector operator-(const QVector & rhs)const; //minus
	QVector operator-()const;
	QVector operator*(const Matrix & rhs)const;

};
