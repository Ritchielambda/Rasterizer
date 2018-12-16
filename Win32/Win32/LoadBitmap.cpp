#include "LoadBitmap.h"
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <sstream>
#pragma comment(lib, "gdiplus.lib")
using namespace std;
using namespace Gdiplus;
Texture2D MathInterface::LoadBitmapToColorArray(wstring filePath)
{
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, nullptr);

	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(filePath.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();
	//≥ı ºªØTexture2D
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
