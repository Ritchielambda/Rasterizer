#pragma once
#include"MyGameEngine.h"
#include "Menus.h"
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

		namespace  StartMenu
		{
			const int GS_Choose_Start = 20;
			const int GS_Choose_Quit = 21;
		}

		namespace  ChooseSceneMenu
		{
			const int GS_Cosmos = 30;
			const int GS_CheckerBoard = 31;
		};

		namespace MainGame
		{
			const int GS_MainGameStartAnimation = 45;
			const int GS_Playing = 40;
			const int GS_GameOverMenu = 41;
			const int GS_PauseMenu = 42;
			const int GS_DeathExplode = 43;

			namespace  GameOverMenu
			{
				const int GS_Win_Choose_Back = 410;
				const int GS_Lose_Choose_Back = 411;
			};

			namespace PauseMenu
			{
				const int GS_Choose_Continue = 420;
				const int GS_Choose_Quit = 421;
			};
		};
	}

	// many namespace to define
	extern int					gRootGameState;
	extern 	Menus					gMenus;
	extern QRender				gRenderer;
	extern ICamera				gCamera;
	extern float					gTimeElapsed;
	extern QPlayer							gPlayer;
	extern UINT					gFPS;
	extern HWND					hwnd;
	void MainGame();
	
}