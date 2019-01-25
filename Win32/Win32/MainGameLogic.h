#pragma once
namespace GamePlay
{
	const float c_halfMovememntRestrictBoxWidth = 800.0f;

	class MainGame
	{
	public:
		MainGame();

		void Init(SCENE_TYPE modelID);

		void UpdateAndRenderMainGame();

	private:
		void					mFunction_UpdateAndRenderStartAnimaton();

		void					mFunction_UpdateAndRenderPlaying();

		void					mFunction_UpdateAndRender_GameOverMenu(BOOL hasWon);

		void					mFunction_UpdateAndRender_PauseMenu();

		void					mFunction_UpdateAndRenderDeathExplode();//chicken die

																		//..............OTHERS
		void					mFunction_CollisionDetectionAndInteract();

		void					mFunction_GameOverAnimationInit(BOOL hasPlayerWon);
		
		ChickenMonster mChickenBoss;
		
		QPlayer							mPlayer;
		SceneModelManager	mSceneMgr;
		BulletManager				mBulletMgr;
		int								mMainGameState;
		BOOL							mIsPlayerVictorious;//has player won??

		Texture2D				mPauseMenuBgr_Continue;
		Texture2D				mPauseMenuBgr_Back;

		Texture2D				mGameOverMenuBgr_Win;
		Texture2D				mGameOverMenuBgr_Lose;
	};
}