#include"Game.h"

void GamePlay::SceneModelManager::Init(SCENE_TYPE model)
{
	//------------------Init Art Resources------------------
	mDefaultMaterial.ambient = { 0.5f,0.5f,0.5f };
	mDefaultMaterial.diffuse = { 1.0f,1.0f,1.0f };
	mDefaultMaterial.specular = { 1.0f,1.0f,1.0f };
	mDefaultMaterial.specularSmoothLevel = 20;

	switch (UINT(model))
	{
	case SCENE_TYPE::SCENE_COSMOS1:
		{
		mSkyTexture.LoadBitmapToColorArray(L"Media/universe.BMP");
		mSkyModel.CreateSphere(2000.0f, 20, 20, TRUE);
		for (UINT i = 0; i < c_asteroidCount; ++i)
		{
			mAsteroid[i].Init((i % 3));
		}
		}break;
	case SCENE_TYPE::SCENE_CHECKERBOARD:
	{
		mSkyTexture.LoadBitmapToColorArray(L"Media/checkerBoard.BMP");

		mSkyModel.CreateSphere(2000, 20, 20, TRUE);
		for (UINT i = 0; i < c_asteroidCount; ++i)
		{
			mAsteroid[i].Init(AsteroidType::AsteroidType_Box);
		}
	}break;
	default:
		break;
	}
	mSkyModel.SetPosition(0, 0, 0);
	mSkyModel.SetMaterial(mDefaultMaterial);
	mSkyModel.Settexture(&mSkyTexture);

	//-----------------Lights----------------------
	mSceneLight.mAmbientColor = { 1.0f,1.0f,1.0f };
	mSceneLight.mDiffuseColor = { 1.0f,1.0f,1.0f };
	mSceneLight.mDiffuseIntensity = 1.0f;
	mSceneLight.mDirection = { -1.0f,-1.0f,-1.0f };
	mSceneLight.mIsEnabled = true;
	mSceneLight.mSpecularColor = { 1.0f,1.0f,1.0f };
	mSceneLight.mSpecularIntensity = 1.2f;

	gRenderer.SetLighting(0, mSceneLight);
	SetCursorPos(0, 0);

}

void GamePlay::SceneModelManager::Update()
{
	FLOAT3 pos = gCamera.GetPosition();
	mSkyModel.SetPosition(pos.x, pos.y, pos.z);
	for (UINT i = 0; i < c_asteroidCount; ++i)mAsteroid[i].Update();
}

void GamePlay::SceneModelManager::Render()
{
	gRenderer.RenderMesh(mSkyModel);
	for (UINT i = 0; i < c_asteroidCount; ++i)mAsteroid[i].Render();
}

void GamePlay::SceneModelManager::GetAsteroidBoundingBox(UINT index, BOUNDINGBOX & outBox)
{
	if (index < c_asteroidCount)
	{
		mAsteroid[index].GetBoundingBox(outBox);
	}
	else
	{
		MessageBoxA(0, "Asteroid Index invalid!!", 0, 0);
	}
}

UINT GamePlay::SceneModelManager::GetAsteroidCount() const
{
	return c_asteroidCount;
}
