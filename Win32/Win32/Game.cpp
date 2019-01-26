#include"Game.h"

using namespace GamePlay;
static	QTimer			localTimer(TIMER_TIMEUNIT_MILLISECOND);
LPCTSTR WndClassName = L"firstwindow";	//Define our window class name
namespace GamePlay
{
	QRender				gRenderer(ghwnd,800,600);

	ICamera				gCamera;
	float						gTimeElapsed = 0.0f;
//	QPlayer							gPlayer;
	UINT						gFPS = 0;
	HWND						ghwnd = NULL;
	int						gRootGameState;
	Menus					gMenus;
	MainGame				gMainGame;
}	
void GamePlay::InitGlobal()
{
	gRootGameState = GameState::GS_StartAnimation;
	//gRenderer.Init(800, 600, ghwnd);
	//set window title
	gRenderer.SetCamera(gCamera);
	gMenus.Init();
}
void GamePlay::UpdateWindowTitle()
{
}
void GamePlay::GameStateSelector()
{
	switch (UINT(gRootGameState))
	{
	case GameState::GS_StartAnimation:
		StartAnimation(); break;
	case GameState::GS_StartMenu:
		StartMenu(); break;
	case GameState::GS_ChooseSceneMenu:
		ChooseSceneMenu(); break;
	case GameState::GS_MainGame:
		fMainGame(); break;
	default:
		DEBUG_MSG1("game state error");
		exit(0);
		break;
	}
}
void GamePlay::StartAnimation()
{
	//no startAnimation
	gRootGameState = GameState::GS_StartMenu;
}
void GamePlay::StartMenu()
{
	gMenus.UpdateAndRender_StartMenu();
}
void GamePlay::ChooseSceneMenu()
{
	gMenus.UpdateAndRender_StartChooseScene();
}

void GamePlay::fMainGame()
{
	localTimer.NextTick();
	gTimeElapsed = Clamp(localTimer.GetInterval(), 0.0f, 100.0f);
	gFPS = localTimer.GetFPS();
	gMainGame.UpdateAndRenderMainGame();
}

bool InitializeWindow(HINSTANCE hInstance,	//Initialize our window
	int ShowWnd,
	int width, int height,
	bool windowed)
{
	//Start creating the window//

	WNDCLASSEX wc;	//Create a new extended windows class

	wc.cbSize = sizeof(WNDCLASSEX);	//Size of our windows class
	wc.style = CS_HREDRAW | CS_VREDRAW;	//class styles
	wc.lpfnWndProc = WndProc;	//Default windows procedure function
	wc.cbClsExtra = NULL;	//Extra bytes after our wc structure
	wc.cbWndExtra = NULL;	//Extra bytes after our windows instance
	wc.hInstance = hInstance;	//Instance to current application
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	//Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);	//Window bg color
	wc.lpszMenuName = NULL;	//Name of the menu attached to our window
	wc.lpszClassName = WndClassName;	//Name of our windows class
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); //Icon in your taskbar

	if (!RegisterClassEx(&wc))	//Register our windows class
	{
		//if registration failed, display error
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ghwnd = CreateWindowEx(	//Create our Extended Window
		NULL,	//Extended style
		WndClassName,	//Name of our windows class
		L"������դ��",	//Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,	//style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,	//Top left corner of window
		width,	//Width of our window
		height,	//Height of our window
		NULL,	//Handle to parent window
		NULL,	//Handle to a Menu
		hInstance,	//Specifies instance of current program
		NULL	//used for an MDI client window
	);

	if (!ghwnd)	//Make sure our window has been created
	{
		//If not, display error
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(ghwnd, ShowWnd);	//Shows our window
	UpdateWindow(ghwnd);	//Its good to update our window

	return true;	//if there were no errors, return true
}

int messageloop() {	//The message loop

	MSG msg;	//Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));	//clear message structure to NULL

	while (true)	//while there is a message
	{
		//if there was a windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	//if the message was WM_QUIT
				break;	//Exit the message loop

			TranslateMessage(&msg);	//Translate the message

									//Send the message to default windows procedure
			DispatchMessage(&msg);
		}

		else {	//Otherewise, keep the flow going
			GamePlay::GameStateSelector();
		}


	}

	return (int)msg.wParam;		//return the message

}

LRESULT CALLBACK WndProc(HWND hwnd,	//Default windows procedure
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)	//Check message
	{

	case WM_KEYDOWN:	//For a key down
						//if escape key was pressed, display popup box
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)

				//Release the windows allocated memory  
				DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:	//if x button in top right was pressed
		PostQuitMessage(0);
		return 0;
	}
	//return the message for windows to handle it
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}