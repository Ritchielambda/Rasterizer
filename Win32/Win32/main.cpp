
//Include necessary Headers//
#include <windows.h>
#include"MyGameEngine.h"
#include"Game.h"

using namespace GamePlay;
int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
		   HINSTANCE hPrevInstance, 
		   LPSTR lpCmdLine,
		   int nShowCmd)
{
	//Initialize Window//
	if(!InitializeWindow(hInstance, nShowCmd, WindowWidth, WindowHeight, true))
	{
		//If initialization failed, display an error message
		MessageBox(0, L"Window Initialization - Failed",
        			L"Error", MB_OK);
		return 0;
	}
	GamePlay::InitGlobal();
	//InitializeRender();
	messageloop();	//Jump into the heart of our program

	return 0;
}
