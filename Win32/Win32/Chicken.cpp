#include"Game.h"

using namespace GamePlay;
static float movePatternTimeCounter = 0.0f;

static float attackPatternTimeCounter = 0.0f;
Gameplay::ChickenMonster::ChickenMonster(BulletManager * pBulletMgr)
	:m_pBulletMgr(pBulletMgr),
	m_AttackState(CHICKEN_ATTACK_STATE_CHASE_PLAYER),
	Base_GameObject(c_chickenInitialHealth)// private member is still valid?  it didnt be derived

{
	mMat_Common.ambient = { 0.3f,0.3f,0.3f };
	mMat_Common.diffuse = { 0.8f,0.8f,0.8f };
	mMat_Common.specular = { 0.5f,0.5f,0.5f };

	mMat_Red.ambient = { 0.9f,0,0 };
	mMat_Red.diffuse = { 0.3f,0,0 };
	mMat_Common.specular = { 0.5f,0.5f,0.5f };
}

void Gameplay::ChickenMonster::Init(SCENE_TYPE modelID)
{
	switch (modelID)
	{
	case SCENE_TYPE::SCENE_COSMOS1:
		mTexture_Common.LoadBitmapToColorArray(L"chicken.bitmap");
		break;
	case SCENE_TYPE::SCENE_CHECKERBORAR:
		mTexture_Common.LoadBitmapToColorArray(L"chicken.bitmap");
		break;
	}

	mMesh.LoadFile_OBJ(L"chicken.obj");
	mPos = FLOAT3(0, 0, 0);
	mLookat = FLOAT3(100.0f, -100.f, 0.0f);

	mMesh.SetMaterial(mMat_Common);

	mMesh.Settexture(&mTexture_Common);
	mMesh.SetPosition(mPos.x, mPos.y, mPos.z);

	Base_GameObject::SetHP(c_chickenInitialHealth);
	movePatternTimeCounter = 0.0f;
	attackPatternTimeCounter = 0.0f;
}

void Gameplay::ChickenMonster::Render()
{
	gRenderer.RenderMesh(mMesh);
}

void Gameplay::ChickenMonster::GetBoundingBox(BOUNDINGBOX & outBox)
{
	mMesh.ComputeBoundingBox(outBox);
}

void Gameplay::ChickenMonster::BeHitAndChangeColor()
{
	mMatType = CHICKEN_MATERIAL_TYPE_HIT;
}

FLOAT3 Gameplay::ChickenMonster::Getposition()
{
	return mPos;
}

float Gameplay::ChickenMonster::GetInitialHealth() const
{
	return c_chickenInitialHealth;
}

void Gameplay::ChickenMonster::mFunction_UpdateMovement(const FLOAT3 & playerPos)
{
	movePatternTimeCounter += gTimeElapsed;

	switch (m_AttackState)
	{
	case CHICKEN_ATTACK_STATE_CHASE_PLAYER:
		if (movePatternTimeCounter <= 7000.0f)
		{
			//try to rotate to look at player
			FLOAT3 toPlayer = playerPos - mPos;
			float currentYawAngle = mMesh.GetRotationY_Yaw();
			float targetYawAngle = atan2(toPlayer.x, toPlayer.z);

			//I don't want the chicken to abruptly look to chicken,
			//so lerp is inevitable (instead of directly set rotation Y)
			mMesh.SetRotationY_Yaw(Lerp(currentYawAngle, targetYawAngle, 0.2f));
		}
		if (movePatternTimeCounter > 5000.0f && movePatternTimeCounter <= 5500.0f)
		{
			mPos.y = Lerp(mPos.y, playerPos.y, 0.2f);
		}
		mFunction_Fire(playerPos - mPos);

		//this state can end
		if (movePatternTimeCounter > 10000.0f)
		{
			movePatternTimeCounter = 0.0f;
			m_AttackState = CHICKEN_ATTACK_STATE_TYPE1;
		}
		break;
	case CHICKEN_ATTACK_STATE_TYPE1:
		if (movePatternTimeCounter < 10000.0f)
		{

			mPos.y = Lerp(mPos.y, playerPos.y, 0.02f);

			mMesh.RotateY_Yaw(0.001f*gTimeElapsed);
			float angleY = mMesh.GetRotationY_Yaw();
			FLOAT3 shootDir = { sinf(angleY),0,cosf(angleY) };// CAN'T understand this angle  ,look at player?
			mFunction_Fire(shootDir);
		}
		if (movePatternTimeCounter > 10000.0f)
		{
			movePatternTimeCounter = 0.0f;
			m_AttackState = CHICKEN_ATTACK_STATE_TYPE2;
		}
		break;
	case CHICKEN_ATTACK_STATE_TYPE2:
	{
		if (movePatternTimeCounter <= 8000.0f)
		{
			//move vertically to get to the same level with play
			mPos.y = Lerp(mPos.y, playerPos.y, 0.02f);

			//horizontally rotate
			mMesh.RotateY_Yaw(-0.002f*gTimeElapsed);
			float angleY = mMesh.GetRotationY_Yaw();
			FLOAT3 shootDir = { sinf(angleY),0,cosf(angleY) };
			mFunction_Fire(shootDir);
		}
		if (movePatternTimeCounter > 8000.0f)
		{
			movePatternTimeCounter = 0.0f;
			m_AttackState = CHICKEN_ATTACK_STATE_ULTIMATE_EXPLODE;
		}
	}
		break;
	case CHICKEN_ATTACK_STATE_ULTIMATE_EXPLODE:
	{
		if (movePatternTimeCounter <= 5000.0f)
		{
			//actually, explode bullets don't need a shoot direction
			mFunction_Fire(FLOAT3(1.0f, 0, 0));
		}
		if (movePatternTimeCounter > 5000.0f)
		{
			movePatternTimeCounter = 0.0f;
			//another cycle
			m_AttackState = CHICKEN_ATTACK_STATE_CHASE_PLAYER;
		}
	}
		break;
	default:
		break;
	}
	mMesh.SetPosition(mPos);
}

void Gameplay::ChickenMonster::mFunction_Fire(FLOAT3 firePos)
{
}

void Gameplay::ChickenMonster::mFunction_UpdateTexture()
{
}
