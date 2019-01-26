#pragma once
#include"MyGameEngine.h"
#include "Menus.h"
#include"Base_GameObject.h"
#include "BulletMgr.h"
#include "Asteroid.h"
#include"SceneEnvironment.h"
#include "Chicken.h"


#include"player.h"

#include "MainGameLogic.h"

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
//	extern QPlayer							gPlayer;
	extern 	MainGame				gMainGame;
	extern UINT					gFPS;
	extern HWND					ghwnd;



	void						InitGlobal();

	void UpdateWindowTitle();

	void	GameStateSelector();

	void StartAnimation();

	void StartMenu();

	void ChooseSceneMenu();

	void fMainGame();
	
	
}
const int WindowWidth = 800;	//window width
const int WindowHeight = 600;	//window height
bool InitializeWindow(HINSTANCE hInstance,	//Initialize our window
	int ShowWnd,
	int width, int height,
	bool windowed);
//void drawpixeltest();
int messageloop();	//Main part of the program

LRESULT CALLBACK WndProc(HWND hWnd,	//Windows callback procedure
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

extern LPCTSTR WndClassName;	//Define our window class name
