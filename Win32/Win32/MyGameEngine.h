#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<random>
#include<stdio.h>
#include<cmath>

#include<math.h>
#include"Matrix.h"
#include"MathInterface.h"
#include"typedef.h"



#include"PointCollection.h"
#include"Texture2D.h"
#include"FileLoader.h"
#include "GeometryMeshGenerator.h"
#include"Mesh.h"
#include"Camera.h"
#include"QRender.h"

#include"QTimer.h"




#define DEBUG_MSG1(msg)	::MessageBoxA(0,msg,0,0);

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)