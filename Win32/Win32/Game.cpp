#include"Game.h"

using namespace GamePlay;
static	QTimer			localTimer(TIMER_TIMEUNIT_MILLISECOND);
namespace GamePlay
{
	QRender				gRenderer(hwnd,800,600);

	ICamera				gCamera;
	float						gTimeElapsed = 0.0f;
	QPlayer							gPlayer;
	UINT						gFPS = 0;
	HWND						hwnd = NULL;
	int						gRootGameState;
	Menus					gMenus;
	MainGame				gMainGame;
}	
void GamePlay::fMainGame()
{
	localTimer.NextTick();
	gTimeElapsed = Clamp(localTimer.GetInterval(), 0.0f, 100.0f);
	gFPS = localTimer.GetFPS();
	gPlayer.Update();
}