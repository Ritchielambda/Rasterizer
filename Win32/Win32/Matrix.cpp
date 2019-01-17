#include"Matrix.h"

Matrix Matrix::operator+(const Matrix &rhs)
{
	Matrix newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[i][j] = this->m[i][j] + rhs.m[i][j];
		}
	}
	return newMatrix;
}
Matrix Matrix::operator-(const Matrix &rhs)
{
	Matrix newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[i][j] = this->m[i][j] - rhs.m[i][j];
		}
	}
	return newMatrix;
}
Matrix Matrix::operator*(const Matrix &rhs)
{
	Matrix newMatrix;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[j][i] = (m[j][0] * rhs.m[0][i]) +
				(m[j][1] * rhs.m[1][i]) +
				(m[j][2] * rhs.m[2][i]) +
				(m[j][3] * rhs.m[3][i]);
		}
	return newMatrix;
}


bool Matrix::operator==(const Matrix &rhs)
{
	bool flag = true;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (this->m[i][j] != rhs.m[i][j]) { flag = false; return flag; }
		}
	}
	return flag;
}
void Matrix::Identity()
{
	_11 = 1.f; _12 = 0.f; _13 = 0.f; _14 = 0.f;
	_21 = 0.f; _22 = 1.f; _23 = 0.f; _24 = 0.f;
	_31 = 0.f; _32 = 0.f; _33 = 1.f; _34 = 0.f;
	_41 = 0.f; _42 = 0.f; _43 = 0.f; _44 = 1.f;
}
void Matrix::SetZero()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			this->m[i][j] = 0.0f;
		}
	}
}
