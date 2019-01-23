#pragma once

namespace GamePlay
{
	class Menus
	{
	public:
		void Init();

		void UpdateAndRender_StartMenu();

		void UpdateAndRender_StartChooseScene();

	private:

		Texture2D mMainMenuBgr_Start;
		Texture2D mMainMenuBgr_Exit;


		Texture2D mChooseSceneBrg_Cosmos;
		Texture2D mChooseSceneBrg_Checkerboard;
	};
}