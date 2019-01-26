#include"Game.h"

void GamePlay::Menus::Init()
{
	mMainMenuBgr_Start.LoadBitmapToColorArray(L"Media/GUI/MainMenu_start.BMP");
	mMainMenuBgr_Exit.LoadBitmapToColorArray(L"Media/GUI/MainMenu_Exit.BMP");
	mChooseSceneBrg_Cosmos.LoadBitmapToColorArray(L"Media/GUI/ChooseScene_Cosmos.BMP");
	mChooseSceneBrg_Checkerboard.LoadBitmapToColorArray(L"Media/GUI/ChooseScene_checkerB.BMP");
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
		gRenderer.DrawPicture(mMainMenuBgr_Start, 0, 0, width, height);
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
		gRenderer.DrawPicture(mMainMenuBgr_Exit, 0, 0, width, height);
	}break;
	default:
		break;
	}
	gRenderer.Present(ghwnd);
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
	

		if (IS_KEY_DOWN(VK_RETURN))
		{
			gMainGame.Init(SCENE_TYPE::SCENE_COSMOS1);
			gRootGameState = GameState::GS_MainGame;
			Sleep(300);
		}

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
		gRenderer.DrawPicture(mChooseSceneBrg_Cosmos, 0, 0, width, height);

	}break;
	case GameState::ChooseSceneMenu::GS_CheckerBoard:
	{
		//SCENE2: CHECKERBOARD 格子世界

		if (IS_KEY_DOWN(VK_RETURN))
		{

			gMainGame.Init(SCENE_TYPE::SCENE_CHECKERBOARD);
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
		gRenderer.DrawPicture(mChooseSceneBrg_Checkerboard, 0, 0, width, height);
	}break;
	default:
		break;
	}
	gRenderer.Present(ghwnd);
}
