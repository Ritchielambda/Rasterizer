#pragma once
#include"Mesh.h"
#include<string>
#include<fstream>
#include<sstream>
#include"typedef.h"
class FileLoader
{
public:
	FileLoader() = default;
	void LoadObjFile(std::string fpath,Mesh * pMesh);
	bool LoadObjPpm(std::string fpath, int width,int height,QVector** pixelbuffer);
private:

};