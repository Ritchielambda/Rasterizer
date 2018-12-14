#pragma once
#include"Vector.h"
#include<Windows.h>
class Texture2D
{
public:
	Texture2D() = default;
	Texture2D(int width, int height);
	~Texture2D();
	Texture2D(const Texture2D & tex);
	Texture2D &operator = (const Texture2D& tex);
	QVector Sample(const FLOAT2& tex);

public:
	//mem
	int m_width;
	int m_height;
	QVector ** m_pixelbuffer;
};