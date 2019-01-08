#include"Vector.h"
float QVector::Length()
{
	float len = x*x + y*y + z*z + w*w;
	len = static_cast<float>(sqrt(len));
	return len;
}
float QVector::dot(const QVector&rhs) const
{
	return (x*rhs.x + y*rhs.y + z*rhs.z+w*rhs.w);
}
QVector QVector::Cross(const QVector& rhs) const
{
	float m1 = y * rhs.z - z * rhs.y;
	float m2 = z * rhs.x - x * rhs.z;
	float m3 = x * rhs.y - y * rhs.x;
	return QVector(m1, m2, m3, 0.0f);
}
QVector QVector::Normalize()
{
	float length = Length();
	if (length != 0)
	{
		float inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
	}
	return *this;
}
QVector QVector::operator+(const QVector & rhs)const
{
	return QVector(x + rhs.x,y+rhs.y,z+rhs.z,w+rhs.w);
}
QVector QVector::operator-(const QVector & rhs)const//minus
{
	return QVector(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}
QVector QVector::operator-()const
{
	return QVector(-x, -y, -z, -w);

}
QVector QVector::operator*(const Matrix & rhs)const
{
	return QVector(x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41,
					x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42,
					x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43,
					x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
}
QVector QVector::operator*(const float rhs)const
{
	return QVector(
		x * rhs,
		y * rhs,
		z * rhs,
		w
	);
}

QVector QVector::operator/(const float rhs) const
{

	return QVector(
		x / rhs,
		y / rhs,
		z / rhs,
		w
	);
}
