#include"Game.h"

void GamePlay::Menus::Init()
{
	mMainMenuBgr_Start.LoadBitmapToColorArray(L"bitmap");
	mMainMenuBgr_Exit.LoadBitmapToColorArray(L"bitmap");
	mChooseSceneBrg_Cosmos.LoadBitmapToColorArray(L"bitmap");
	mChooseSceneBrg_Checkerboard.LoadBitmapToColorArray(L"bitmap");
}

void GamePlay::Menus::UpdateAndRender_StartMenu()
{
	static int startMenuState = GameState::StartMenu::GS_Choose_Start;

	UINT width = gRenderer.GetBufferwidth();
	UINT height = gRenderer.GetBufferheight();

	switch (startMenuState)
	{
	case GameState::StartMenu::GS_Choose_Start:
	{
		if (IS_KEY_DOWN(VK_RETURN))
		{
			gRootGameState = GameState::GS_ChooseSceneMenu;
			Sleep(300);
		}
		if (IS_KEY_DOWN(VK_DOWN))
		{
			//move cursor to choose "QUIT"
			startMenuState = GameState::StartMenu::GS_Choose_Quit;
			Sleep(300);
		}
		//draw picture todo
	}
	break;
	case GameState::StartMenu::GS_Choose_Quit:
	{
		if (IS_KEY_DOWN(VK_RETURN))
		{
			//exit the program
			exit(0);
			break;
		}
		if (IS_KEY_DOWN(VK_UP))
		{
			startMenuState = GameState::StartMenu::GS_Choose_Start;
			Sleep(300);
		}
		////draw picture todo
	}break;
	default:
		break;
	}
	//grender.present
}

void GamePlay::Menus::UpdateAndRender_StartChooseScene()
{
	static int chooseSceneMenuState = GameState::ChooseSceneMenu::GS_Cosmos;

	UINT width = gRenderer.GetBufferwidth();
	UINT height = gRenderer.GetBufferheight();

	gRenderer.ClearScreen();

	switch (chooseSceneMenuState)
	{
	case GameState::ChooseSceneMenu::GS_Cosmos:
	{
		//init main game todo
		gRootGameState = GameState::GS_MainGame;
		Sleep(300);
		if (IS_KEY_DOWN(VK_LEFT))
		{
			chooseSceneMenuState = GameState::ChooseSceneMenu::GS_CheckerBoard;
			Sleep(300);
		}

		if (IS_KEY_DOWN(VK_RIGHT))
		{
			chooseSceneMenuState = GameState::ChooseSceneMenu::GS_CheckerBoard;
			Sleep(300);
		}
		//draw picture todo

	}break;
	case GameState::ChooseSceneMenu::GS_CheckerBoard:
	{
		//SCENE2: CHECKERBOARD 格子世界
		if (IS_KEY_DOWN(VK_RETURN))
		{
			//maingame todo
			//gMainGame.Init(SCENE_TYPE::SCENE_CHECKERBOARD);
			gRootGameState = GameState::GS_MainGame;
			Sleep(300);
		};

		//next scene
		if (IS_KEY_DOWN(VK_LEFT))
		{
			chooseSceneMenuState = GameState::ChooseSceneMenu::GS_Cosmos;
			Sleep(300);
		}

		if (IS_KEY_DOWN(VK_RIGHT))
		{
			chooseSceneMenuState = GameState::ChooseSceneMenu::GS_Cosmos;
			Sleep(300);
		}
		//drawpicture todo
	}break;
	default:
		break;
	}
	//grender.present todo
}
