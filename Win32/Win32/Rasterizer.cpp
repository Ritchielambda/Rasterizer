#include"Rasterizer.h"

using namespace MathInterface;
Viewport Rasterizer::viewport(200,200,600,599);

Rasterizer::Rasterizer(HWND h, const size_t pwidth, const size_t pheight) :width(pwidth), height(pheight) 
{
	image = new UINT[pwidth*pheight];
	HDC hdc = GetDC(h);
	m_hdc = CreateCompatibleDC(hdc);
	BITMAPINFOHEADER bmphdr = { 0 };
	bmphdr.biSize = sizeof(BITMAPINFOHEADER);
	bmphdr.biWidth = pwidth;
	bmphdr.biHeight = pheight;
	bmphdr.biPlanes = 1;
	bmphdr.biBitCount = 32;
	bmphdr.biSizeImage = pheight * pwidth * 4;
	Zbuffer = new float*[width];
	for (int i = 0; i < width; ++i)
	{
		Zbuffer[i] = new float[height];
	}
	//创建后缓冲区
	//先创建一个内存dc
	//hdc = CreateCompatibleDC(nullptr);
	m_bit_map = CreateDIBSection(m_hdc, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS, reinterpret_cast<void**>(&image), nullptr, 0);

	
	if (!m_bit_map)
	{
		MessageBox(0, (LPCWSTR) "CreateDIBSection Failed.", 0, 0);
	}
	
	m_OldBitmap = (HBITMAP)SelectObject(m_hdc, m_bit_map);
	ReleaseDC(h, hdc);
	GetObject(m_bit_map, sizeof(BITMAP), &bm);
}
Rasterizer::~Rasterizer()
{}
ARGB Rasterizer::getpixel(int x, int y)
{
	return ARGB(0,255, 0, 0);
}
void Rasterizer::setpixel(int x, int y, ARGB color)
{
	if (x >= 0 && x < width && y >= 0 && y <height)
	image[width*y + x] = ColorToUINT(color);
}

void Rasterizer::drawlineDDA(int x1, int y1,int x2,int y2, ARGB color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);

	if (dx > dy)
	{
		if (x1 > x2) {
			swap(x1, x2); swap(y1, y2);
		}
		float k = 0;
		if (dx)
		k = static_cast<float>(y2 - y1) / (x2 - x1);

		float y = y1;
		for (int i = x1; i < x2; ++i)
		{
			setpixel(i, y, color);
			y += k;
		}
	}
	else
	{
		if (y1>y2) {
			swap(x1, x2); swap(y1, y2);
		}
		float k = 0;
		if(dy)
		k =( static_cast<float>(x2 - x1)) / (y2 - y1);
		float x = x1;
		for (int i = y1; i < y2; ++i)
		{
			setpixel(x, i, color);
			x += k;
		}
	}
}
void Rasterizer::drawlinemiddle(int x1,int y1,int x2,int y2,ARGB color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx >= dy)                                     //以dx=1为步进，否则会出现断点
	{
		if (x1>x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		int A = y1 - y2;
		int B = x2 - x1;
		if (y2 >= y1)                                  //斜率[0,1]
		{
			int d = (A << 1) + B;                     //f(x+1,y+0.5)*2以消除浮点数运算
			int upIncrement = (A + B) << 1;             //取上点时d的增量
			int downTncrement = A << 1;               //取下点时d的增量
			for (int x = x1, y = y1; x <= x2; ++x)
			{
				setpixel(x, y, color);
				if (d<0) {                           //中点在直线下，取上点
					d += upIncrement;
					++y;
				}
				else
				{
					d += downTncrement;
				}
			}
		}
		else                                        //斜率[-1,0)
		{
			int d = (A << 1) - B;
			int upIncrement = A << 1;
			int downTncrement = (A - B) << 1;
			for (int x = x1, y = y1; x <= x2; ++x)
			{
				setpixel(x, y, color);
				if (d<0) {
					d += upIncrement;
				}
				else
				{
					d += downTncrement;
					--y;
				}
			}
		}
	}
	else
	{
		if (y1>y2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		int A = x1 - x2;
		int B = y2 - y1;
		if (x2 >= x1)
		{
			int d = (A << 1) + B;                 //f(x+0.5,y+1)*2以消除浮点数运算,此处Ay+Bx+C=0
			int upIncrement = (A + B) << 1;         //取上点时d的增量
			int downTncrement = A << 1;           //取下点时d的增量
			for (int x = x1, y = y1; y <= y2; ++y)
			{
				setpixel(x, y, color);
				if (d<0) {                       //中点在直线下，取上点
					d += upIncrement;
					++x;
				}
				else
				{
					d += downTncrement;
				}
			}
		}
		else
		{
			int d = (A << 1) - B;
			int upIncrement = A << 1;
			int downTncrement = (A - B) << 1;
			for (int x = x1, y = y1; y <= y2; ++y)
			{
				setpixel(x, y, color);
				if (d<0) {
					d += upIncrement;
				}
				else
				{
					d += downTncrement;
					--x;
				}
			}
		}
	}

}
void Rasterizer::drawlineBresenham(int x1, int y1, int x2, int y2, ARGB color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx >= dy)                                     //以dx=1为步进，否则会出现断点
	{
		if (x1>x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		int flag = y2 >= y1 ? 1 : -1;                     //斜率[-1,1]
		int k = flag*(dy << 1);
		int e = -dx*flag;
		for (int x = x1, y = y1; x <= x2; ++x)
		{
			setpixel(x, y, color);
			e += k;
			if (flag*e>0)
			{
				y += flag;
				e -= 2 * dx*flag;
			}
		}
	}
	else
	{
		if (y1>y2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		int flag = x2>x1 ? 1 : -1;
		int k = flag*(dx << 1);
		int e = -dy*flag;
		for (int x = x1, y = y1; y <= y2; ++y)
		{
			setpixel(x, y, color);
			e += k;
			if (flag*e>0)
			{
				x += flag;
				e -= 2 * dy*flag;
			}
		}
	}

}
ScanLine Rasterizer::generatescanline(Vertex vl, Vertex vr)
{
	float width = vr.m_Position.x - vl.m_Position.x;
	int startX = vl.m_Position.x + 0.5;
	Vertex step((vr.m_Position.x - vl.m_Position.x) / width, (vr.m_Position.y - vl.m_Position.y) / width,0,0 ,(vr.Color - vl.Color)/width);
	return ScanLine(vl, step, width,startX, vl.m_Position.y+0.5);
}
void Rasterizer::drawScanline(Vertex vl, Vertex vr)
{
	ScanLine line = generatescanline(vl, vr);
	Vertex step = line.Setp;
	int lenght = line.length + 0.5;
	for (int i = 0; i < lenght; i++)
	{
		//if (i == lenght / 2)
		//	Vertex v(step);
		if (line.StartVertex.Divz > GetZvalue(line.x + i, line.y))
		{
			setpixel(line.x + i, line.y, line.StartVertex.Color); SetZvalue(line.x + i, line.y, line.StartVertex.Divz);
		}
		line.StartVertex += step;
	}
}
void Rasterizer::SortVertex(Vertex &v1, Vertex &v2, Vertex &v3)
{
	if ((v2.m_Position.y < v3.m_Position.y)||((v2.m_Position.y == v3.m_Position.y)&& (v2.m_Position.x > v3.m_Position.x)))//冒泡算法 先按照y 最大为序，然后y相同按照x小为序
	{
		swap(v2, v3);
	}
	if ((v1.m_Position.y < v2.m_Position.y) || ((v1.m_Position.y == v2.m_Position.y) && (v1.m_Position.x > v2.m_Position.x)))//冒泡算法 先按照y 最大为序，然后y相同按照x小为序
	{
		swap(v1, v2);
	}
	if ((v2.m_Position.y < v3.m_Position.y) || ((v2.m_Position.y == v3.m_Position.y) && (v2.m_Position.x > v3.m_Position.x)))//冒泡算法 先按照y 最大为序，然后y相同按照x小为序
	{
		swap(v2, v3);
	}
}
void Rasterizer::drawflatButtomtriangles(Vertex v1, Vertex v2, Vertex v3)
{
	int startY = v1.m_Position.y + 0.5;
	int endY = v3.m_Position.y + 0.5;
	for (int y = startY; y >= endY; --y)
	{
		float factor = static_cast<float>(y - startY) / (endY - startY);
		Vertex vl = Lerp(v1,v2, factor);
		Vertex vr = Lerp(v1,v3, factor);
		drawScanline(vl, vr);
	}
}
void Rasterizer::drawflatToptriangles(Vertex v1, Vertex v2, Vertex v3)
{
	int startY = v1.m_Position.y + 0.5;
	int endY = v3.m_Position.y + 0.5;
	for (int y = startY; y >= endY; --y)
	{
		float factor = static_cast<float>(y - startY) / (endY - startY);
		Vertex vl = Lerp(v1, v3, factor);
		Vertex vr = Lerp(v2, v3, factor);
		drawScanline(vl, vr);
	}
}
void Rasterizer::drawtriangles(Vertex& v1, Vertex &v2, Vertex& v3)
{
	QVector vec1 = v2.m_Position - v1.m_Position;
	QVector vec2 = v3.m_Position - v2.m_Position;
	FLOAT3 f1(vec1.x, vec1.y, 0);
	FLOAT3 f2(vec2.x, vec2.y, 0);
	float z = vec1.x*vec2.y - vec1.y*vec2.x;
	if (z > 0)
		return;
	SortVertex(v1, v2, v3);
	if (v2.m_Position.y == v3.m_Position.y)
	{
		drawflatButtomtriangles(v1, v2, v3);
	}
	else if (v1.m_Position.y == v2.m_Position.y)
	{
		drawflatToptriangles(v1, v2, v3);
	}
	else
	{
		float factor = (v1.m_Position.y - v2.m_Position.y) / (v1.m_Position.y - v3.m_Position.y);
		Vertex v4 = Lerp(v1, v3, factor);
		if(v2.m_Position.x>v4.m_Position.x)
		{
			swap(v2, v4);
		}
		drawflatButtomtriangles(v1, v2, v4);
		drawflatToptriangles(v2, v4, v3);
	}
	return;
}
Outcode Rasterizer::ComputeOutCode(float x, float y)
{
	Outcode code;

	code = INSIDE;          // initialised as being inside of [[clip window]]

	if (x < viewport.left)           // to the left of clip window
		code |= LEFT;
	else if (x > viewport.right)      // to the right of clip window
		code |= RIGHT;
	if (y < viewport.bottom)           // below the clip window
		code |= BOTTOM;
	else if (y > viewport.top)      // above the clip window
		code |= TOP;
	return code;
}
void Rasterizer::DrawRectangle()
{
	drawlineBresenham(viewport.left, viewport.bottom, viewport.right, viewport.bottom, ARGB(0, 1, 0, 0));
	drawlineBresenham(viewport.left, viewport.top, viewport.right, viewport.top, ARGB(0, 1, 0, 0));
	drawlineBresenham(viewport.left, viewport.top, viewport.left, viewport.bottom, ARGB(0, 1, 0, 0));
	drawlineBresenham(viewport.right, viewport.top, viewport.right, viewport.bottom, ARGB(0, 1, 0, 0));
}
void Rasterizer::LineClipping(Vertex& v1, Vertex &v2)
{
	int x0 = v1.m_Position.x;
	int y0 = v1.m_Position.y;
	int x1 = v2.m_Position.x;
	int y1 = v2.m_Position.y;
	Outcode outcode0 = ComputeOutCode(v1.m_Position.x, v1.m_Position.y);
	Outcode outcode1 = ComputeOutCode(v2.m_Position.x, v2.m_Position.y);
	bool accept = false;
	while (TRUE)
	{
		if (!(outcode0|outcode1))
		{
			accept = true;
			break;
		}
		else if (outcode0&outcode1)
		{
			break;
		}
		else
		{
			float x, y;
			Outcode outcodeOut = outcode0 ? outcode0 : outcode1;
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (viewport.top - y0) / (y1 - y0);
				y = viewport.top;
			}
			else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (viewport.bottom - y0) / (y1 - y0);
				y = viewport.bottom;
			}
			else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (viewport.right - x0) / (x1 - x0);
				x = viewport.right;
			}
			else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y0 + (y1 - y0) * (viewport.left -x0) / (x1 - x0);
				x = viewport.left;
			}
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(x1, y1);
			}
		}
		

	}
	if (accept) {
		// Following functions are left for implementation by user based on
		// their platform (OpenGL/graphics.h etc.)
		DrawRectangle();
		drawlineBresenham(x0, y0, x1, y1, ARGB(0, 0, 1, 0));
	}
}


float Rasterizer::GetZvalue(int x, int y)
{
	if (x >= 0 && x < width&&y >= 0 && y < height)
		return Zbuffer[x][y];
	return 0;
}
void Rasterizer::SetZvalue(int x, int y,float value)
{
	if (x >= 0 && x < width&&y >= 0 && y < height)
		Zbuffer[x][y] = value;;
	return ;
}
void Rasterizer::ClearZbuffer()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			Zbuffer[i][j] = 0;// 1 is biggest,so everything  is in front of it;
		}
	}
}