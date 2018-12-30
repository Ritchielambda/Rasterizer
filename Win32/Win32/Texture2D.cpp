#include"Texture2D.h"
Texture2D::Texture2D(int width, int height)
{
	m_width = width;
	m_height = height;
	m_pixelbuffer = new QVector*[width];
	for (int i = 0; i < width; ++i)
	{
		m_pixelbuffer[i] = new QVector[height];
	}
}
Texture2D::~Texture2D()
{
	if (m_pixelbuffer)
	{
		for (int i = 0; i < m_width; ++i)
		{
			delete[] m_pixelbuffer[i];
		}
	}
}
Texture2D::Texture2D(const Texture2D & tex):m_width(tex.m_width),m_height(tex.m_height)//�������캯�� ����Ҫɾ��ԭ��ָ�룬��Ϊ�ǹ���~
{
	m_pixelbuffer = new QVector*[m_width];
	for (int i = 0; i < m_width; ++i)
	{
		m_pixelbuffer[i] = new QVector[m_height];
	}
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			m_pixelbuffer[i][j] = tex.m_pixelbuffer[i][j];
		}
	}
}
Texture2D& Texture2D::operator = (const Texture2D& tex)
{
	if (this == &tex)
		return *this;

	for (int i = 0; i < m_width; i++)
	{
		delete[]m_pixelbuffer[i];
	}
	delete[]m_pixelbuffer;
	m_width = tex.m_width;
	m_height = tex.m_height;
	m_pixelbuffer = new QVector*[m_width];

	for (int i = 0; i < m_width; ++i)
	{
		m_pixelbuffer[i] = new QVector[m_height];
	}
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			m_pixelbuffer[i][j] = tex.m_pixelbuffer[i][j];
		}
	}
	return *this;
}
QVector Texture2D::Sample(const FLOAT2& tex)
{
	if (tex.x >= 0 && tex.x <= 1 && tex.y >= 0 && tex.y <= 1)
	{
		UINT x = tex.x * (m_width - 1);
		UINT y = tex.y * (m_height - 1);
		return m_pixelbuffer[x][y];
	}
	else
	{
		float u, v;
		if (tex.x > 1)
		{
			u = tex.x - static_cast<int>(tex.x);
		}
		else if (tex.x < 0)
		{
			u = static_cast<int>((-tex.x) + 1) + tex.x;
		}
		if(tex.y>1)
		{
			v = static_cast<int>((-tex.y) + 1) + tex.y;
		}
		else if (tex.y < 0)
		{
			v = static_cast<int>((-tex.y) + 1) + tex.y;
		}
		UINT x = u * (m_width - 1);
		UINT y = v * (m_height - 1);
		return m_pixelbuffer[x][y];
	}
}