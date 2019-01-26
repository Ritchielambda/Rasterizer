
//Include necessary Headers//
#include <windows.h>
#include"MyGameEngine.h"
#include"Game.h"


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
	messageloop();	//Jump into the heart of our program

	return 0;
}
//
//void drawpixeltest()
//{
//	//StretchBlt(hdc, 0, 0, 200, 200, a.gethdc(), 0, 0, a.bm.bmWidth, a.bm.bmHeight, SRCCOPY);
//	//render.DrawIndexed();
//
//
//	////render.DrawIndexed();
//	//render.Present();
//	
//	
//	/*Texture2D texture = MathInterface::LoadBitmapToColorArray(L"2.bmp");
//
//	for (int i = 0; i < texture.m_height; ++i)
//	{
//		for (int j = 0; j < texture.m_width; ++j)
//		{
//			image[i*texture.m_width+j] = render->QVectorConverttoINT(texture.m_pixelbuffer[j][i]);
//		}
//	}*/
//	MainGame();
//	gRenderer.ClearScreen(COLOR4(0,0,0,0));
//	gRenderer.ClearZbuffer();
//	gRenderer.RenderMesh(*mesh);
//
//	HDC hdc = GetDC(hwnd);
//	BitBlt(hdc, 0, 0, gRenderer.GetBufferwidth(), gRenderer.GetBufferheight(), gRenderer.GetHDC(), 0, 0, SRCCOPY);
//}
//
//void InitializeRender()
//{
//	mesh = new Mesh;
//	mesh->LoadFile_OBJ(L"teapot.obj");
//	//mesh->LoadTexture(L"1.bmp");
//
//	Matrix trans = MatrixTranslate(0, 10, 250);
//	mesh->SetPosition(0, 0, 10);
//	//set camera done
////	gCamera = new ICamera;
//	gCamera.SetPosition(10, 5, -10.0f);
//	gCamera.SetLookAt(0, 0, 0);
//	gCamera.SetViewAngle(0.5f*3.14f, Width / Height);
//	gCamera.SetViewFrustumPlane(1.0f, 1000);
//
//	//Matrix Projection = MatrixPerspectiveFovLH(0.4f*3.14f, Width/Height, 1, 1000);
//	//Matrix WVP = world*View*Projection;
////	render->SetWordMatrix(world);
////	render->SetViewMatrix(View);
////	render->SetProjMatrix(Projection);
//	//Set lighting
//
//	DirectionalLight mSceneLight;
//	// set direction lighting
//	mSceneLight.mAmbientColor = { 1.0f,1.0f,1.0f };
//	mSceneLight.mDiffuseColor = { 0.0f,.96f,1.0f };
//	mSceneLight.mDiffuseIntensity = 1.0f;
//	mSceneLight.mDirection = { -1,-1.0f,1.0f };
//	mSceneLight.mIsEnabled = TRUE;
//	mSceneLight.mSpecularColor = { 1.0f,1.0f,1.0f };
//	mSceneLight.mSpecularIntensity = 0.7f;
//	gRenderer.SetLighting(0, mSceneLight);
//	gRenderer.SetCamera(gCamera);
//	gRenderer.hwnd = hwnd;
//}
