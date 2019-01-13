#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<random>
#include<stdio.h>
#include<cmath>

#include"MathInterface.h"
#include"Matrix.h"
#include"Mesh.h"
#include"FileLoader.h"
#include"QRender.h"
#include"Vector.h"
#include"player.h"
#include"typedef.h"
#include"Texture2D.h"
#include"Base_GameObject.h"
#include"Camera.h"

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)