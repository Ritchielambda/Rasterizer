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




#include"Texture2D.h"
#include"FileLoader.h"
#include"Mesh.h"
#include"Camera.h"
#include"QRender.h"

#include"QTimer.h"
#include"Base_GameObject.h"
#include"player.h"




#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)