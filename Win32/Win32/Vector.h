#pragma once
#include"Matrix.h"
#include<math.h>
struct FLOAT2
{
	FLOAT2() { x = y = 0; }
	FLOAT2(float x1, float y1) :x(x1), y(y1) {}
	float x;
	float y;
	FLOAT2 operator-(FLOAT2 &rhs) { return FLOAT2(this->x - rhs.x, this->y - rhs.y); }
	friend FLOAT2 operator*(float scaleFactor, const FLOAT2& vec)
	{
		return FLOAT2(scaleFactor*vec.x, scaleFactor* vec.y);
	}
	FLOAT2 operator+(const FLOAT2& vec)
	{
		return FLOAT2(x + vec.x, y + vec.y);
	}
	FLOAT2 operator*(const float scaleFactor)
	{
		return FLOAT2(scaleFactor*x, scaleFactor* y);
	}
};
struct FLOAT3
{
	FLOAT3() {}
	FLOAT3(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
	FLOAT3(const float column[3])
	{
		x = column[0];
		y = column[1];
		z = column[2];
	}
	FLOAT3& operator=(const FLOAT3 &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }


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
	QVector operator*(const QVector&vec) { return QVector(vec.x*x, vec.y*y, vec.z*z, vec.w*w); }
	friend QVector operator*(float scaleFactor, const QVector& vec)
	{
		return QVector(scaleFactor*vec.x, scaleFactor* vec.y, scaleFactor* vec.z, scaleFactor*vec.w);
	}

	QVector operator+(const QVector& vec)
	{
		return QVector(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

public:
	float Length();
	QVector Normalize();
	float dot(const QVector &rhs) const;
	QVector Cross(const QVector& rhs)const; 
	QVector operator+(const QVector & rhs)const;
	QVector operator-(const QVector & rhs)const; //minus
	QVector operator-()const;
	QVector operator*(const Matrix & rhs)const;
	QVector operator*(const float rhs)const;

};
