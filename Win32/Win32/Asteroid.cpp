#include"Game.h"

using namespace GamePlay;
static std::default_random_engine rndEngine;
static std::uniform_real_distribution<float> posDist(-c_halfMovementRestrictBoxWidth / 2.0f, c_halfMovementRestrictBoxWidth / 2.0f);
static std::uniform_real_distribution<float> speedDist(-0.2f, 0.2f);
static std::uniform_real_distribution<float> rotSpeedDist(-0.001f, 0.001f);



void GamePlay::Asteroid::Init(UINT AsteroidType)
{
	switch (UINT(AsteroidType))
	{
	case AsteroidType_Rock1:
		mMesh.LoadFile_OBJ(L"rock1.obj");
		break;
	case AsteroidType_Rock2:
		mMesh.LoadFile_OBJ(L"rock2.obj");
		break;
	case AsteroidType_Rock3:
		mMesh.LoadFile_OBJ(L"rock3.obj");
		break;
	case AsteroidType_Box:
	default:
		float width = rand() % 70 + 70;
		mMesh.CreateBox(width, width, width, 3, 3, 3);
		break;
	}
	mTexture.LoadBitmapToColorArray(L"rock.bitmap");
	
	Material mat;
	mat.ambient = { 0.3f,0.3f,0.3f };
	mat.diffuse = { 0.7f,0.7f,0.7f };
	mMesh.SetMaterial(mat);

	mMesh.SetPosition(posDist(rndEngine), posDist(rndEngine), posDist(rndEngine));
	mRotateSpeed = { rotSpeedDist(rndEngine),rotSpeedDist(rndEngine), rotSpeedDist(rndEngine) };
	mMoveSpeed = { speedDist(rndEngine),speedDist(rndEngine), speedDist(rndEngine) };
	
}

void GamePlay::Asteroid::Update()
{
	FLOAT3 pos = mMesh.GetPosition();
	pos += mMoveSpeed;
	if (abs(pos.x) > c_halfMovementRestrictBoxWidth ||
		abs(pos.y) > c_halfMovementRestrictBoxWidth ||
		abs(pos.z) > c_halfMovementRestrictBoxWidth)
	{
		pos = { posDist(rndEngine), posDist(rndEngine), posDist(rndEngine) };
	}
	mMesh.SetPosition(pos);

	mMesh.RotateX_Pitch(mRotateSpeed.x*gTimeElapsed);
	mMesh.RotateY_Yaw(mRotateSpeed.y*gTimeElapsed);
	mMesh.RotateZ_Roll(mRotateSpeed.z*gTimeElapsed);

}

void GamePlay::Asteroid::Render()
{
	gRenderer.RenderMesh(mMesh);
}

void GamePlay::Asteroid::GetBoundingBox(BOUNDINGBOX & outBox)
{
	mMesh.ComputeBoundingBox(outBox);
}
