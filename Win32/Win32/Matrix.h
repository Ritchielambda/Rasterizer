#pragma once

template<class T>
void swap(T &lhs, T &rhs);
template<class T>
void swap(T &lhs, T &rhs)
{
	T temp = lhs;
	lhs = rhs;
	rhs = temp;
}



struct Matrix
{
	union
	{
		float m[4][4];
		struct {
			float _11; float _12; float _13; float _14;
			float _21; float _22; float _23; float _24;
			float _31; float _32; float _33; float _34;
			float _41; float _42; float _43; float _44;
		};
	};
public:
	Matrix() = default;
	Matrix(float p11, float p12, float p13, float p14,
		float p21, float p22, float p23, float p24,
		float p31, float p32, float p33, float p34,
		float p41, float p42, float p43, float p44) :_11(p11), _12(p12), _13(p13), _14(p14),
		_21(p21), _22(p22), _23(p23), _24(p24),
		_31(p31), _32(p32), _33(p33), _34(p34),
		_41(p41), _42(p42), _43(p43), _44(p44){}
	Matrix(const Matrix & rhs)
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] = rhs.m[i][j];
	}
	Matrix&operator =(const Matrix& rhs)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->m[i][j] = rhs.m[i][j];
			}
		}
		return *this;
	}



	// 运算
public:
	Matrix operator+(const Matrix &rhs);
	Matrix operator-(const Matrix &rhs);
	Matrix operator*(const Matrix &rhs);//矩阵乘法

	bool operator==(const Matrix &rhs);
	void Identity();//单位阵
	void SetZero();
};
