#pragma once

class Texture2D
{
public:
	Texture2D() = default;
	Texture2D(int width, int height);
	~Texture2D();
	Texture2D(const Texture2D & tex);
	Texture2D &operator = (const Texture2D& tex);
	QVector Sample(const FLOAT2& tex);
	UINT GetWidth() {return m_width;}
	UINT GetHeight() { return m_height; }
	QVector& GetPixel(UINT x, UINT y) { return m_pixelbuffer[x][y]; }
	void LoadBitmapToColorArray(std::wstring filePath);
public:

	//mem
	UINT m_width;
	UINT m_height;
	QVector ** m_pixelbuffer;

};
