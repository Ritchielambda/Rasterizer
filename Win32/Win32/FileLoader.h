#pragma once

class FileLoader
{
public:
	FileLoader() = default;
	bool LoadObjFile(std::wstring fpath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer);
	bool LoadObjPpm(std::wstring fpath, int width,int height, MathInterface::QVector** pixelbuffer);
	
private:

};
