#pragma once
struct Vertex;
namespace MathInterface
{

	struct FLOAT2
	{
		FLOAT2() { x = y = 0; }
		FLOAT2(float x1, float y1) :x(x1), y(y1) {}
		float x;
		float y;
		FLOAT2 operator-(FLOAT2 &rhs) { return FLOAT2(x - rhs.x, y - rhs.y); }
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
		float	Length() const
		{
			return sqrtf(x*x + y*y + z*z);
		};
		FLOAT3& operator+=(const FLOAT3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		};
		FLOAT3 operator+(const FLOAT3 &rhs) { return FLOAT3(x + rhs.x, y + rhs.y, z + rhs.z); }
		friend FLOAT3 operator*(float scaleFactor, const FLOAT3& vec)
		{
			return FLOAT3(scaleFactor*vec.x, scaleFactor*vec.y, scaleFactor*vec.z);
		}
		FLOAT3& operator*=(const float scaleFactor)
		{
			x *= scaleFactor;
			y *= scaleFactor;
			z *= scaleFactor;
			return *this;
		}
		FLOAT3 operator*(const FLOAT3& rhs) { return FLOAT3(x*rhs.x, y*rhs.y, z*rhs.z); }
		FLOAT3 operator-(const FLOAT3 &rhs) { return FLOAT3(x - rhs.x, y - rhs.y, z - rhs.z); }
		FLOAT3& Normalize() {
			float len = Length();
			if (len != 0)
			{
				x /= len;
				y /= len;
				z /= len;
			}
			else
			{
				x = y = z = 0.0f;
			}
			return *this;
		}

		float x;
		float y;
		float z;
	};
	struct QVector
	{

		float x;
		float y;
		float z;
		float w;

		QVector(float px, float py, float pz, float pw) :x(px), y(py), z(pz), w(pw) {}
		QVector() = default;
		QVector(const QVector & rhs) :x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
		QVector&operator = (const QVector&rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
		QVector operator*(const QVector&vec)const { return QVector(vec.x*x, vec.y*y, vec.z*z, vec.w*w); }
		friend QVector operator*(float scaleFactor, const QVector& vec)
		{
			return QVector(scaleFactor*vec.x, scaleFactor* vec.y, scaleFactor* vec.z, scaleFactor*vec.w);
		}

		QVector operator+(const QVector& vec)
		{
			return QVector(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		QVector& operator+=(const QVector& rhs)
		{
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;
		}

		float Length() 
		{
			float len = x*x + y*y + z*z + w*w;
			len = static_cast<float>(sqrt(len));
			return len;
		}
		QVector Normalize()
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
		float dot(const QVector &rhs) const { return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w); }
		QVector Cross(const QVector& rhs) {
			float m1 = y * rhs.z - z * rhs.y;
			float m2 = z * rhs.x - x * rhs.z;
			float m3 = x * rhs.y - y * rhs.x;
			return QVector(m1, m2, m3, 0.0f);
		}
		QVector operator+(const QVector & rhs)const {
			return QVector(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}
		QVector operator-(const QVector & rhs)const 
		{
			return QVector(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}//minus
		QVector operator-() { return QVector(-x, -y, -z, -w); }
		QVector operator*(const Matrix & rhs) {
			return QVector(x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41,
				x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42,
				x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43,
				x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
		}
		QVector operator*(const float rhs)const {
			return QVector(
				x * rhs,
				y * rhs,
				z * rhs,
				w
			);
		}
		QVector operator/(const float rhs) {
			return QVector(
				x / rhs,
				y / rhs,
				z / rhs,
				w
			);
		}

	};

	struct BOUNDINGBOX
	{
		BOUNDINGBOX()
		{
			max = { 1,1,1 };
			min = { 0,0,0 };
		}

		FLOAT3 max;
		FLOAT3 min;
	};


	UINT ColorToUINT(QVector color);
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
	Matrix MatrixPitchYawRoll(float pitch_X, float yaw_Y, float roll_Z);
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

	  float Vec3_Dot(const  FLOAT3& vec1, const FLOAT3& vec2);

	  FLOAT3 Vec3_Reflect(const FLOAT3 &vec1, const FLOAT3& norm);
}
