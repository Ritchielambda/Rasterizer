﻿
//Include necessary Headers//
#include <windows.h>
#include"Rasterizer.h"
//Define variables/constants//
LPCTSTR WndClassName = L"firstwindow";	//Define our window class name
HWND hwnd = NULL;	//Sets our windows handle to NULL

const int Width  = 800;	//window width
const int Height = 600;	//window height
Image a(hwnd, Width, Height);
//Functions//
bool InitializeWindow(HINSTANCE hInstance,	//Initialize our window
		 int ShowWnd,
		 int width, int height,
		 bool windowed);
void drawpixeltest(Image a);
int messageloop();	//Main part of the program

LRESULT CALLBACK WndProc(HWND hWnd,	//Windows callback procedure
		 UINT msg,
		 WPARAM wParam,
		 LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
		   HINSTANCE hPrevInstance, 
		   LPSTR lpCmdLine,
		   int nShowCmd)
{
	//Initialize Window//
	if(!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		//If initialization failed, display an error message
		MessageBox(0, L"Window Initialization - Failed",
        			L"Error", MB_OK);
		return 0;
	}

	messageloop();	//Jump into the heart of our program

	return 0;
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

	hwnd = CreateWindowEx(	//Create our Extended Window
		NULL,	//Extended style
		WndClassName,	//Name of our windows class
		L"软件光栅化",	//Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,	//style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,	//Top left corner of window
		width,	//Width of our window
		height,	//Height of our window
		NULL,	//Handle to parent window
		NULL,	//Handle to a Menu
		hInstance,	//Specifies instance of current program
		NULL	//used for an MDI client window
		);

	if (!hwnd)	//Make sure our window has been created
	{
    	//If not, display error
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);	//Shows our window
	UpdateWindow(hwnd);	//Its good to update our window

	return true;	//if there were no errors, return true
}

int messageloop(){	//The message loop

	MSG msg;	//Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));	//clear message structure to NULL

	while(true)	//while there is a message
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

		else{	//Otherewise, keep the flow going

			drawpixeltest(a);
		}
		

	}

	return (int)msg.wParam;		//return the message

}

LRESULT CALLBACK WndProc(HWND hwnd,	//Default windows procedure
			 UINT msg,
			 WPARAM wParam,
			 LPARAM lParam)
{
switch( msg )	//Check message
{

	case WM_KEYDOWN:	//For a key down
 	  	//if escape key was pressed, display popup box
		if( wParam == VK_ESCAPE ){
			if(MessageBox(0, L"Are you sure you want to exit?",
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
void drawpixeltest(Image a)
{
	//a.drawlineDDA(500, 400, 350, 350, ARGB(0,255, 0, 0));
	//a.drawlineBresenham(0, 0, 500, 400, ARGB(0,255, 0, 0));
	/*for (int i = 0; i<580; i += 5)
	{
		for (int j = 0; j<580; j += 5)
		{
			double v = sqrt(pow(i - 350, 2) + pow(j - 350, 2)) - 200;
			if (v >= 0 && v < 2)
			{
				a.drawlineDDA(350, 350, i, j, ARGB(0, (1 + rand() % 255), (1 + rand() % 255) , (1 + rand() % 255)));
			}
		}
	}*/
	/*Vertex v1(0, 0,0,0, ARGB(0, 0, 1, 0));
	Vertex v2(800, 600, 0, 0, ARGB(0, 0, 0, 1));
	Vertex v3(600, 300, 0, 0, ARGB(0, 1, 0, 0));*/
	//a.drawtriangles(v1, v2, v3);
	//a.drawlineBresenham(a.viewport.left, a.viewport.bottom, a.viewport.right, a.viewport.bottom, ARGB(0, 255, 0, 0));
	/*a.LineClipping(v1, v2);*/
	Texture2D texture = MathInterface::LoadBitmapToColorArray(L"123.bmp");
	for (int i = 0; i < texture.m_width; ++i)
	{
		for (int j = 0; j < texture.m_height; ++j)
			a.setpixel(i, j, ARGB(texture.m_pixelbuffer[i][j].w, texture.m_pixelbuffer[i][j].x, texture.m_pixelbuffer[i][j].y, texture.m_pixelbuffer[i][j].z));
	}
	HDC hdc = GetDC(hwnd);
	BitBlt(hdc, 0, 0, a.bm.bmWidth, a.bm.bmHeight, a.gethdc(),0, 0, SRCCOPY);
	//StretchBlt(hdc, 0, 0, 200, 200, a.gethdc(), 0, 0, a.bm.bmWidth, a.bm.bmHeight, SRCCOPY);
	
}

