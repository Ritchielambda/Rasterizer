#include"Game.h"

const FLOAT3 c_CameraInitalPos_Cosmos = FLOAT3(1000.0f, 1000.0f, 1000.0f);
const FLOAT3 c_CameraIntialLookat_Cosmos = FLOAT3(0.0f, 1000.0f, 1000.0f);

const FLOAT3 c_CameraInitalPos_CheckerBoard = FLOAT3(-800.0f, 800.0f, 1000.0f);
const FLOAT3 c_CameraIntialLookat_CheckerBoard = FLOAT3(0.0f, 1000.0f, 1000.0f);

const FLOAT3 c_CameraTargetPos = FLOAT3(200.0f, 200.0f, 200.0f);
const FLOAT3 c_CameraTargetLookat = FLOAT3(0, 0, 0);
GamePlay::MainGame::MainGame()
	:mChickenBoss(&mBulletMgr)
	,mPlayer(&mBulletMgr)
	,mIsPlayerVictorious(FALSE)
{

}

void GamePlay::MainGame::Init(SCENE_TYPE modelID)
{
	mSceneMgr.Init(modelID);
	mChickenBoss.Init(modelID);
	mPlayer.Init();

	mMainGameState = GameState::MainGame::GS_MainGameStartAnimation;

	switch (modelID)
	{
	case SCENE_TYPE::SCENE_COSMOS1:
		gCamera.SetPosition(c_CameraInitalPos_Cosmos);
		gCamera.SetLookAt(c_CameraIntialLookat_Cosmos);
		break;

	case SCENE_TYPE::SCENE_CHECKERBOARD:
		gCamera.SetPosition(c_CameraInitalPos_CheckerBoard);
		gCamera.SetLookAt(c_CameraIntialLookat_CheckerBoard);
		break;
	default:
		break;
	}
	
	mPauseMenuBgr_Continue.LoadBitmapToColorArray(L"Media/GUI/PauseMenu_Continue.BMP");
	mPauseMenuBgr_Back.LoadBitmapToColorArray(L"Media/GUI/PauseMenu_Back.BMP");
	mGameOverMenuBgr_Lose.LoadBitmapToColorArray(L"Media/GUI/GameOver_Lose.BMP");
	mGameOverMenuBgr_Win.LoadBitmapToColorArray(L"Media/GUI/GameOver_Win.BMP");
}

void GamePlay::MainGame::UpdateAndRenderMainGame()
{
	switch (mMainGameState)
	{
	case GameState::MainGame::GS_MainGameStartAnimation:
	{
		mFunction_UpdateAndRenderStartAnimaton();
		break;
	}

	case GameState::MainGame::GS_Playing:
	{
		mFunction_UpdateAndRenderPlaying();
		break;
	}

	case GameState::MainGame::GS_DeathExplode:
	{
		//chicken/player die, boom
		mFunction_UpdateAndRenderDeathExplode();
		break;
	}

	case GameState::MainGame::GS_PauseMenu:
	{
		mFunction_UpdateAndRender_PauseMenu();
		break;
	}

	case GameState::MainGame::GS_GameOverMenu:
	{
		mFunction_UpdateAndRender_GameOverMenu(mIsPlayerVictorious);
		break;
	}

	default:
		MessageBoxA(0, "game state error!!", 0, 0); ;
		break;
	}
}
//-----------------------------------private-----------------------------------
//-----------------------------------------------------------------------------

void GamePlay::MainGame::mFunction_UpdateAndRenderStartAnimaton()
{
	static float startAnimationTimeCounter = 0.0f;
	startAnimationTimeCounter += gTimeElapsed;

	FLOAT3 camPos = gCamera.GetPosition();
	FLOAT3 cameLookat = gCamera.GetLookAt();
	float restDistance = (camPos - c_CameraTargetPos).Length();

	if (restDistance > 10.0f)
	{
		gRenderer.ClearScreen();
		gCamera.SetPosition(Lerp(camPos, c_CameraTargetPos, gTimeElapsed*0.001f));
		gCamera.SetLookAt(Lerp(cameLookat, c_CameraTargetLookat, gTimeElapsed*0.001f));
		mSceneMgr.Update();
		mSceneMgr.Render();
		//mChickenBoss.Render();

		gRenderer.Present(ghwnd);
	}
	else
	{
		mMainGameState = GameState::MainGame::GS_Playing;
		startAnimationTimeCounter = 0.0f;
	}
}

void GamePlay::MainGame::mFunction_UpdateAndRenderPlaying()
{
	mSceneMgr.Update();
	mPlayer.Update();
	mChickenBoss.Update(mPlayer.GetPosition());

	//collision detection

	mFunction_CollisionDetectionAndInteract();
	mBulletMgr.UpdateBullets();

	if (IS_KEY_DOWN(VK_ESCAPE))
	{
		mMainGameState = GameState::MainGame::GS_PauseMenu;
		Sleep(300);

	}
	if (mPlayer.IsDead())
	{
		mFunction_GameOverAnimationInit(FALSE);
	}
	if (mChickenBoss.IsDead())
	{
		mFunction_GameOverAnimationInit(TRUE);
	}

	gRenderer.ClearScreen();

	//------------------------render 3d---------------------------

	mSceneMgr.Render();
	mChickenBoss.Render();
	mPlayer.Render();
	mBulletMgr.Render();
	//---------------HUD----------------------------------------
	//player blood bar
	UINT bloodBarWidth = UINT(50 * mPlayer.GetHP() / mPlayer.GetInitialHealth());
	gRenderer.DrawRect(FLOAT2{ 0.1f,0.9f }, bloodBarWidth*3, 30, COLOR4(1, 0, 0, 0));

	UINT chickenBloodBarWidth = UINT(80.0f*(mChickenBoss.GetHP() / mChickenBoss.GetInitialHealth()));
	gRenderer.DrawRect(FLOAT2{ 0.4f,0.9f }, chickenBloodBarWidth *3, 30, COLOR4(1, 1, 0, 0));



	UINT width = gRenderer.GetBufferwidth();
	UINT height = gRenderer.GetBufferheight();
	gRenderer.DrawLine({ 1.0f,1.0f,1.0f,1.0f }, width / 2 - 8, height / 2, width / 2 + 8, height / 2);
	gRenderer.DrawLine({ 1.0f,1.0f,1.0f,1.0f }, width / 2, height / 2 - 8, width / 2, height / 2 + 8);
	gRenderer.Present(ghwnd);
}

void GamePlay::MainGame::mFunction_UpdateAndRender_GameOverMenu(BOOL hasWon)
{
	UINT width = gRenderer.GetBufferwidth();
	UINT height = gRenderer.GetBufferheight();

	gRenderer.ClearScreen();

	if (hasWon == TRUE)
	{
		gRenderer.DrawPicture(mGameOverMenuBgr_Win, 0, 0, width, height);
		if (IS_KEY_DOWN(VK_RETURN))
		{
			gRootGameState = GameState::GS_StartMenu;
			mMainGameState = GameState::MainGame::GS_Playing;
			Sleep(300);

		}
	}
	else
	{
		gRenderer.DrawPicture(mGameOverMenuBgr_Lose, 0, 0, width, height);
		if (IS_KEY_DOWN(VK_RETURN))
		{
			gRootGameState = GameState::GS_StartMenu;
			mMainGameState = GameState::MainGame::GS_Playing;
			Sleep(300);
		}
	}
	gRenderer.Present(ghwnd);
}

void GamePlay::MainGame::mFunction_UpdateAndRender_PauseMenu()
{
	static int pauseMenuState = GameState::MainGame::PauseMenu::GS_Choose_Continue;

	UINT width = gRenderer.GetBufferwidth();
	UINT height = gRenderer.GetBufferheight();

	gRenderer.ClearScreen();
	switch (pauseMenuState)
	{
	case GameState::MainGame::PauseMenu::GS_Choose_Continue:
	{
		if (IS_KEY_DOWN(VK_RETURN) == TRUE || IS_KEY_DOWN(VK_ESCAPE) == TRUE)
		{
			//Go back to maingame
			gRootGameState = GameState::GS_MainGame;
			mMainGameState = GameState::MainGame::GS_Playing;

			Sleep(300);
		}
		if (IS_KEY_DOWN(VK_DOWN) == TRUE)
		{
			pauseMenuState = GameState::MainGame::PauseMenu::GS_Choose_Quit;
		}

		gRenderer.DrawPicture(mPauseMenuBgr_Continue, 0, 0, width, height);
		break;
	}
	case GameState::MainGame::PauseMenu::GS_Choose_Quit:
	{
		if (IS_KEY_DOWN(VK_RETURN))
		{
			gRootGameState = GameState::GS_StartMenu;
			Sleep(300);
		}
		if (IS_KEY_DOWN(VK_ESCAPE))
		{
			gRootGameState = GameState::GS_MainGame;
			mMainGameState = GameState::MainGame::GS_Playing;
			//why it can prevent consistently press key
			Sleep(300);
		}
		if (IS_KEY_DOWN(VK_UP))
		{
			pauseMenuState = GameState::MainGame::PauseMenu::GS_Choose_Continue;
		}
		gRenderer.DrawPicture(mPauseMenuBgr_Back, 0, 0, width, height);
	}
	default:

		break;
	}
	gRenderer.Present(ghwnd);
}

void GamePlay::MainGame::mFunction_UpdateAndRenderDeathExplode()
{
	static float deathExplodeTimeCounter = 0.0f;
	deathExplodeTimeCounter += gTimeElapsed;
	if (deathExplodeTimeCounter < 1000.0f)
	{
		gRenderer.ClearScreen();
		mChickenBoss.Render();
		mSceneMgr.Render();

		gRenderer.Present(ghwnd);
	}
	//time to enjoy watching the explosion
	if (deathExplodeTimeCounter > 1000.0f&&deathExplodeTimeCounter < 3000.0f)
	{
		gRenderer.ClearScreen();
		if (mIsPlayerVictorious = FALSE)
		{
			mChickenBoss.Render();
		}
		mSceneMgr.Render();
		mBulletMgr.UpdateBullets();
		mBulletMgr.Render();
		gRenderer.Present(ghwnd);
	}
	if (deathExplodeTimeCounter > 3000.0f)
	{
		mMainGameState = GameState::MainGame::GS_GameOverMenu;
		deathExplodeTimeCounter = 0.0f;
	}
}

void GamePlay::MainGame::mFunction_CollisionDetectionAndInteract()
{
	BOUNDINGBOX playerAABB, chickenAABB;
	mPlayer.GetBoundingBox(playerAABB);
	mChickenBoss.GetBoundingBox(chickenAABB);

	//first bullet player collision
	std::vector<FLOAT3> collidePointList;


	//player get hit

	mBulletMgr.collisionDection(playerAABB, true, collidePointList);
	if (collidePointList.size() != 0)
	{
		mPlayer.ReduceHP(5.0f);

	}
	//clear point list in funcs
	mBulletMgr.collisionDection(chickenAABB, true, collidePointList);
	if (collidePointList.size() != 0)
	{
		mChickenBoss.ReduceHP(5.0f);
		mChickenBoss.BeHitAndChangeColor();
	}
	collidePointList.clear();
	for (UINT i = 0; i < mSceneMgr.GetAsteroidCount(); ++i)
	{
		//retrieve bounding box of each asteroid
		BOUNDINGBOX rockBox;
		mSceneMgr.GetAsteroidBoundingBox(i, rockBox);

		//do nothing except killing collided bullets
		mBulletMgr.collisionDection(rockBox, true, collidePointList);
	}
	collidePointList.clear();
}

void GamePlay::MainGame::mFunction_GameOverAnimationInit(BOOL hasPlayerWon)
{
	static std::default_random_engine rndEngine;
	static std::uniform_real_distribution<float> unitDist(-1.0f, 1.0f);
	if (hasPlayerWon)
	{
		mMainGameState = GameState::MainGame::GS_DeathExplode;
		mBulletMgr.killAllBullet();
		mIsPlayerVictorious = TRUE;

		gCamera.SetLookAt(mChickenBoss.Getposition());
		for (int i = 0; i < 2000; ++i)
		{
			FLOAT3 dir = { unitDist(rndEngine),unitDist(rndEngine),unitDist(rndEngine) };
			dir.Normalize();
			mBulletMgr.SpawnBullet(mChickenBoss.Getposition(), dir, FLOAT3(1, 0, 0));
		}
	}
	else
	{
		mMainGameState = GameState::MainGame::GS_DeathExplode;
		mBulletMgr.killAllBullet();
		mIsPlayerVictorious = FALSE;
		for (int i = 0; i < 2000; ++i)
		{
			FLOAT3 dir = { unitDist(rndEngine), unitDist(rndEngine), unitDist(rndEngine) };
			dir.Normalize();
			mBulletMgr.SpawnBullet(mPlayer.GetPosition(), dir, FLOAT3(1, 0, 0));

		}
		gCamera.SetPosition(mPlayer.GetPosition() + FLOAT3(300, 300, 300));
		gCamera.SetLookAt(mPlayer.GetPosition());

	}
}

