#include"MathInterface.h"

const float MathInterface::PI = 3.1415926f;
UINT ColorToUINT(ARGB color)
{
	BYTE red = 255 * color.r/*  color.w*/;
	BYTE green = 255 * color.g/* color.w*/;
	BYTE blue = 255 * color.b /* color.w*/;
	return (UINT)((BYTE)blue | (WORD)((BYTE)green << 8) | (DWORD)((BYTE)red << 16));
}
float MathInterface::Lerp(float&lhs, float&rhs, float factor)
{
	return float(lhs + (rhs - lhs)*factor);
}
FLOAT2 MathInterface::Lerp(FLOAT2&lhs, FLOAT2&rhs, float factor)
{
	return FLOAT2(Lerp(lhs.x,rhs.x, factor),Lerp(lhs.y,rhs.y, factor));

}
Vertex MathInterface::Lerp(Vertex& lhs, Vertex & rhs, float factor)
{
	ARGB color = lhs.Color + (rhs.Color - lhs.Color)*factor;
	return Vertex(Lerp(lhs.m_Position.x, rhs.m_Position.x,factor),(lhs.m_Position.y, rhs.m_Position.y, factor), (lhs.m_Position.z, rhs.m_Position.z, factor), (lhs.m_Position.w, rhs.m_Position.w, factor),color);
}
FLOAT3 MathInterface::Lerp(FLOAT3&lhs, FLOAT3&rhs, float factor)
{
	return FLOAT3(Lerp(lhs.x, rhs.x, factor), Lerp(lhs.y, rhs.y, factor), Lerp(lhs.z, rhs.z, factor));
}
Matrix MathInterface::MatrixIdentity()
{
	return Matrix(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
}
Matrix MathInterface::MatrixTranspose(const Matrix& mat)
{
	return Matrix(mat._11, mat._21, mat._31, mat._41,
		mat._12, mat._22, mat._32, mat._42,
		mat._13, mat._23, mat._33, mat._43,
		mat._14, mat._24, mat._34, mat._44);
}
float MathInterface::MatrixDet(const Matrix& mat)//矩阵行列式
{
	float result =
		mat._11*mat._22*mat._33*mat._44 - mat._11*mat._22*mat._34*mat._43 -
		mat._11*mat._23*mat._32*mat._44 + mat._11*mat._23*mat._34*mat._42 +

		mat._11*mat._24*mat._32*mat._43 - mat._11*mat._24*mat._33*mat._42 -
		mat._12*mat._21*mat._33*mat._44 + mat._12*mat._21*mat._34*mat._43 +

		mat._12*mat._23*mat._31*mat._44 - mat._12*mat._23*mat._34*mat._41 -
		mat._12*mat._24*mat._31*mat._43 + mat._12*mat._24*mat._33*mat._41 +

		mat._13*mat._21*mat._32*mat._44 - mat._13*mat._21*mat._34*mat._42 -
		mat._13*mat._22*mat._31*mat._44 + mat._13*mat._22*mat._34*mat._41 +

		mat._13*mat._24*mat._31*mat._42 - mat._13*mat._24*mat._32*mat._41 -
		mat._14*mat._21*mat._32*mat._43 + mat._14*mat._21*mat._33*mat._42 +

		mat._14*mat._22*mat._31*mat._43 - mat._14*mat._22*mat._33*mat._41 -
		mat._14*mat._23*mat._31*mat._42 + mat._14*mat._23*mat._32*mat._41;

	return result;
}

float MathInterface::MatrixAdjElem(
	float a1, float a2, float a3,
	float b1, float b2, float b3,
	float c1, float c2, float c3)
{
	return a1*(b2*c3 - c2*b3) - a2*(b1*c3 - c1*b3) + a3*(b1*c2 - c1*b2);
}
Matrix MathInterface::MatrixAdj(const Matrix& mat)
{
	float a1 = MatrixAdjElem(mat._22, mat._23, mat._24, mat._32, mat._33, mat._34, mat._42, mat._43, mat._44);
	float a2 = MatrixAdjElem(mat._21, mat._23, mat._24, mat._31, mat._33, mat._34, mat._41, mat._43, mat._44);
	float a3 = MatrixAdjElem(mat._21, mat._22, mat._24, mat._31, mat._32, mat._34, mat._41, mat._42, mat._44);
	float a4 = MatrixAdjElem(mat._21, mat._22, mat._23, mat._31, mat._32, mat._33, mat._41, mat._42, mat._43);
	float b1 = MatrixAdjElem(mat._12, mat._13, mat._14, mat._32, mat._33, mat._34, mat._42, mat._43, mat._44);
	float b2 = MatrixAdjElem(mat._11, mat._13, mat._14, mat._31, mat._33, mat._34, mat._41, mat._43, mat._44);
	float b3 = MatrixAdjElem(mat._11, mat._12, mat._14, mat._31, mat._32, mat._34, mat._41, mat._42, mat._44);
	float b4 = MatrixAdjElem(mat._11, mat._12, mat._13, mat._31, mat._32, mat._33, mat._41, mat._42, mat._43);
	float c1 = MatrixAdjElem(mat._12, mat._13, mat._14, mat._22, mat._23, mat._24, mat._42, mat._43, mat._44);
	float c2 = MatrixAdjElem(mat._11, mat._13, mat._14, mat._21, mat._23, mat._24, mat._41, mat._43, mat._44);
	float c3 = MatrixAdjElem(mat._11, mat._12, mat._14, mat._21, mat._22, mat._24, mat._41, mat._42, mat._44);
	float c4 = MatrixAdjElem(mat._11, mat._12, mat._13, mat._21, mat._22, mat._23, mat._41, mat._42, mat._43);
	float d1 = MatrixAdjElem(mat._12, mat._13, mat._14, mat._22, mat._23, mat._24, mat._32, mat._33, mat._34);
	float d2 = MatrixAdjElem(mat._11, mat._13, mat._14, mat._21, mat._23, mat._24, mat._31, mat._33, mat._34);
	float d3 = MatrixAdjElem(mat._11, mat._12, mat._14, mat._21, mat._22, mat._24, mat._31, mat._32, mat._34);
	float d4 = MatrixAdjElem(mat._11, mat._12, mat._13, mat._21, mat._22, mat._23, mat._31, mat._32, mat._33);

	Matrix result(
		a1, -a2, a3, -a4,
		-b1, b2, -b3, b4,
		c1, -c2, c3, -c4,
		-d1, d2, -d3, d4
	);
	return MatrixTranspose(result);
}
//逆矩阵 = 伴随矩阵/(行列式值的绝对值)
Matrix MathInterface::MatrixInverse(const Matrix& mat)
{
	float det = abs(static_cast<int>(MatrixDet(mat)));
	Matrix adj = MatrixAdj(mat);
	Matrix inverse;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			inverse.m[i][j] = adj.m[i][j] / det;
		}

	return inverse;
}
Matrix MathInterface::MatrixScaling(float ScaleX, float ScaleY, float ScaleZ)
{
	return Matrix(
		ScaleX, 0, 0, 0,
		0, ScaleY, 0, 0,
		0, 0, ScaleZ, 0,
		0, 0, 0, 1
	);
}
Matrix MathInterface::MatrixTranslate(float TransX, float TransY, float TransZ)
{
	return Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		TransX, TransY, TransZ, 1
	);
}
Matrix MathInterface::MatrixRotationX(float angle)
{
	return Matrix(
		1, 0, 0, 0,
		0, cos(angle), sin(angle), 0,
		0, -sin(angle), cos(angle), 0,
		0, 0, 0, 1
	);
}
Matrix MathInterface::MatrixRotationY(float angle)
{
	return Matrix(
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	);
}
Matrix MathInterface::MatrixRotationZ(float angle)
{
	return Matrix(
		cos(angle), sin(angle), 0, 0,
		-sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
Matrix MathInterface::MatrixLookAtLH(QVector eyePos, QVector lookAt, QVector up)
{
	QVector zaxis = lookAt - eyePos;
	zaxis.Normalize();
	QVector xaxis = up.Cross(zaxis).Normalize();
	QVector yaxis = zaxis.Cross(xaxis);

	return Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		-xaxis.dot(eyePos), -yaxis.dot(eyePos), -zaxis.dot(eyePos), 1
	);
}
Matrix MathInterface::MatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	Matrix mat;
	mat.SetZero();
	// tan(fovAngleY*0.5f)
	float height = cos(fovAngleY*0.5f) / sin(fovAngleY*0.5f);
	mat._11 = height / aspectRatio;
	mat._22 = height;
	mat._33 = farZ / (farZ - nearZ);
	mat._34 = 1.f;
	mat._43 = (nearZ * farZ) / (nearZ - farZ);
	return mat;
}
QVector MathInterface::Reflect(const QVector& I, const QVector& N)
{
	//公式 R = I - 2(I·N)N
	float tmp = 2.f * I.dot(N);
	return I - (N * tmp);
}
Texture2D MathInterface::LoadBitmapToColorArray(std::wstring filePath)
{
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, nullptr);

	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(filePath.c_str());


	{
		UINT height = bmp->GetHeight();
		UINT width = bmp->GetWidth();
		//初始化Texture2D
		Texture2D texture(width, height);

		Gdiplus::Color color;

		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				bmp->GetPixel(x, y, &color);

				texture.m_pixelbuffer[x][height - 1 - y] = QVector(
					color.GetRed() / 255.f,
					color.GetGreen() / 255.f,
					color.GetBlue() / 255.f,
					1.f
				);
			}
		delete bmp;
		Gdiplus::GdiplusShutdown(gdiplustoken);
		return texture;
	}
}