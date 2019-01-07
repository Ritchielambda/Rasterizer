#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include"typedef.h"
#include"Texture2D.h"
#include <string>
#include <vector>
class FileLoader
{
public:
	FileLoader() = default;
	bool LoadObjFile(std::wstring fpath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer);
	bool LoadObjPpm(std::wstring fpath, int width,int height,QVector** pixelbuffer);
	Texture2D* LoadBitmapToColorArray(std::wstring filePath);
private:

};
