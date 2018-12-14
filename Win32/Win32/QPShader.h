#pragma once
#include"Rasterizer.h"
class QPShader
{
public:
	QPShader();
	void sample(int U,int V,int textureindex);

};