#pragma once
#include<Windows.h>
#include<math.h>
#include"Matrix.h"
#include"Vector.h"
#include"MathInterface.h"
typedef int Outcode;
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000
struct Viewport
{
	Viewport(int pleft, int pbottom, int pright, int ptop) :left(pleft), bottom(pbottom), right(pright), top(ptop) {}
	int bottom;
	int top;
	int left;
	int right;
};


class ScanLine
{
public:
	ScanLine(Vertex pStartV, Vertex pstep, int plength,int px,int py) :StartVertex(pStartV), Setp(pstep), length(plength),x(px),y(py) {}
	Vertex StartVertex;
	Vertex Setp;
	int length;
	int x;
	int y;
};
class Rasterizer
{
public:

	/*Public function*/
	Rasterizer(HWND h, const size_t pwidth, const size_t pheight);
	/*render*/
	ARGB getpixel(int x, int y);
	void setpixel(int x, int y, ARGB color);
	void drawlineDDA(int x1, int y1,int x2,int y2, ARGB color);
	void drawlinemiddle(int x1, int y1, int x2, int y2, ARGB color);
	void drawlineBresenham(int x1, int y1, int x2, int y2, ARGB color);
	void drawScanline(Vertex vl, Vertex vr);
	void drawflatButtomtriangles(Vertex v1, Vertex v2, Vertex v3);
	void drawflatToptriangles(Vertex v1, Vertex v2, Vertex v3);
	void drawtriangles(Vertex& v1, Vertex &v2, Vertex &v3);
	void LineClipping(Vertex &v1, Vertex &v2);
	void DrawRectangle();
	float GetZvalue(int x, int y);
	void SetZvalue(int x, int y, float value);
	void ClearZbuffer();
	//Assistant function
	ScanLine generatescanline(Vertex vl, Vertex vr);
	
	void SortVertex(Vertex &v1, Vertex &v2, Vertex& v3);
	Outcode ComputeOutCode(float x, float y);
	/*get member function*/
	HWND gethwnd() { return hwnd; }
	size_t getwidth() { return width; }
	size_t getheight() { return height; }
	HDC gethdc() { return m_hdc; }
	~Rasterizer();
	

	/*Public member*/
	 HBITMAP m_bit_map;
	BITMAP bm;
	HBITMAP m_OldBitmap;
	static Viewport viewport;
private:
	UINT *image;
	size_t width;
	size_t height;
	HWND hwnd;
	HDC m_hdc;
	float ** Zbuffer;

};

