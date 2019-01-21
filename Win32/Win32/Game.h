#pragma once
#include"MyGameEngine.h"
#include "Chicken.h"
#include "Asteroid.h"
#include"player.h"
#include"SceneEnvironment.h"
namespace GamePlay
{
	const float c_halfMovementRestrictBoxWidth = 800.0f;
	namespace GameState
	{
		const int GS_StartAnimation = 1;
		const int GS_StartMenu = 2;
		const int GS_ChooseSceneMenu = 3;
		const int GS_MainGame = 4;
	}

	// many namespace to define

	extern QRender				gRenderer;
	extern ICamera				gCamera;
	extern float					gTimeElapsed;
	extern QPlayer							gPlayer;
	extern UINT					gFPS;
	extern HWND					hwnd;
	void MainGame();
	
}