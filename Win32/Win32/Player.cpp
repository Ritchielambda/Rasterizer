#include"Game.h"

const float GamePlay::QPlayer::c_halfPlayerWidth = 25.0f;
const float GamePlay::QPlayer::c_halfPlayerHeight = 35.0f;
const float GamePlay::QPlayer::c_halfPlayerDepth = 25.0f;

GamePlay::QPlayer::QPlayer(BulletManager* bulletmgr) 
	:m_pBulletMgr(bulletmgr),
	Base_GameObject(c_playerInitalHealth)
{

}

void GamePlay::QPlayer::Init()
{
	//set cursorpos
	gCamera.SetPosition(0.0f, 0.0f, -8.0f);
	gCamera.SetLookAt(0.0f, 0.0f, 0.0f);
	gCamera.SetViewAngle(MathInterface::PI / 2.0f, 4.0f / 3.0f);
	gCamera.SetViewFrustumPlane(1.0f, 2500.0f);
	Base_GameObject::SetHP(c_playerInitalHealth);
	
}

void GamePlay::QPlayer::Update()
{
	mFunction_UpdateMovement(gTimeElapsed);
	mFunction_Fire(gTimeElapsed);

}

void GamePlay::QPlayer::Render()
{
	//this interface for create third person game
}

void GamePlay::QPlayer::GetBoundingBox(MathInterface::BOUNDINGBOX & outBox)
{
	outBox.min =
	{
		mCurrentPos.x - c_halfPlayerWidth,
		mCurrentPos.y - c_halfPlayerHeight,
		mCurrentPos.z - c_halfPlayerDepth
	};
	outBox.max =
	{
		mCurrentPos.x + c_halfPlayerWidth,
		mCurrentPos.y + c_halfPlayerHeight,
		mCurrentPos.z + c_halfPlayerDepth
	};

}

FLOAT3 GamePlay::QPlayer::GetPosition()
{
	return mCurrentPos;
}

FLOAT3 GamePlay::QPlayer::GetPrevPosition()
{
	return mLastPos;
}

float GamePlay::QPlayer::GetInitialHealth() const
{
	return c_playerInitalHealth;
}

void GamePlay::QPlayer::mFunction_Fire(float timeElapsed)
{
	static float fireCoolingDownTime = 0.0f;

	static const float fireTimeCoolDownThreshold = 100.0f;
	fireCoolingDownTime += timeElapsed;

	if (fireCoolingDownTime >= fireTimeCoolDownThreshold)
	{
		FLOAT3 unitCamera = gCamera.GetDirection().Normalize();
		FLOAT3 vel_Bullet = unitCamera;

		float gunDistanceFromplayer = 0.0f;
		//incase bullets collide with self bounding box
		float gunDistanceFromMainAxis = min(c_halfPlayerWidth, c_halfPlayerDepth) - 1.0f;

		FLOAT3 pos_Bullet1 = mCurrentPos +
			FLOAT3(gunDistanceFromMainAxis*cos(gCamera.GetRotationY_Yaw()), 0, -gunDistanceFromMainAxis*sin(gCamera.GetRotationY_Yaw()));

		//left gun
		FLOAT3 pos_Bullet2 = mCurrentPos +
			FLOAT3(-gunDistanceFromMainAxis*cos(gCamera.GetRotationY_Yaw()), 0, gunDistanceFromMainAxis*sin(gCamera.GetRotationY_Yaw()));

		//-----------------Spawn Bullet------------------

		//right bullet
		m_pBulletMgr->SpawnBullet(pos_Bullet1, vel_Bullet*0.8f, { (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f });
		//left bullet
		m_pBulletMgr->SpawnBullet(pos_Bullet2, vel_Bullet*0.8f, { (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f });

		//reset cool down time
		fireCoolingDownTime = 0.0f;
	}
}

void GamePlay::QPlayer::mFunction_UpdateMovement(float timeElapsed)
{
	FLOAT3 moveVector = { 0,0,0 };
	if (IS_KEY_DOWN('A'))
	{
		moveVector.x -= 1.0f;
	}
	if (IS_KEY_DOWN('D'))
	{
		moveVector.x += 1.0f;
	}
	if (IS_KEY_DOWN('W'))
	{
		moveVector.z += 1.0f;
	}
	if (IS_KEY_DOWN('S'))
	{
		moveVector.z -= 1.0f;
	}
	if (IS_KEY_DOWN(VK_LCONTROL))
	{
		moveVector.y -= 1.0f;
	}
	if (IS_KEY_DOWN(VK_SPACE))
	{
		moveVector.y += 1.0f;
	}
	moveVector.Normalize();
	moveVector *= (0.02f*timeElapsed);
	gCamera.fps_MoveRight(moveVector.x);
	gCamera.fps_MoveForward(moveVector.z);
	gCamera.fps_MoveUp(moveVector.y);

	FLOAT3 camPos = gCamera.GetPosition();
	mLastPos = mCurrentPos;
	mCurrentPos = camPos;

	// ------------------------------------cursor movement--------------------------------------
	static POINT lastCursorPos = { 0,0 };
	static POINT currentCursorPos = { 0,0 };
	static const int scrWidth = ::GetSystemMetrics(SM_CXSCREEN);
	static const int scrHeight = ::GetSystemMetrics(SM_CYSCREEN);
	lastCursorPos = currentCursorPos;
	::GetCursorPos(&currentCursorPos);

	//if cursor reach the boundary,go to another side
	if (currentCursorPos.x == scrWidth - 1)
	{
		::SetCursorPos(0, currentCursorPos.y);
		lastCursorPos = { 0,currentCursorPos.y };
		currentCursorPos = lastCursorPos;
	}
	else
	{
		if (currentCursorPos.x == 0)
		{
			::SetCursorPos(scrWidth - 1, currentCursorPos.y);
			lastCursorPos = { scrWidth - 1,currentCursorPos.y };
			currentCursorPos = lastCursorPos;
		}
	}

	if (currentCursorPos.y == scrHeight - 1)
	{
		::SetCursorPos(currentCursorPos.x, 0);
		lastCursorPos = { currentCursorPos.x,0 };
		currentCursorPos = lastCursorPos;
	}
	else
	{
		if (currentCursorPos.y == 0)
		{
			::SetCursorPos(currentCursorPos.x, scrHeight - 1);
			lastCursorPos = { currentCursorPos.x,scrHeight - 1 };
			currentCursorPos = lastCursorPos;
		}
	}

	//camera rotation

	int cursorDeltaX = currentCursorPos.x - lastCursorPos.x;
	int cursorDeltaY = currentCursorPos.y - lastCursorPos.y; 
	gCamera.RotateY_Yaw(0.0002*cursorDeltaX*timeElapsed);
	gCamera.RotateX_Pitch(0.0002*cursorDeltaY*timeElapsed);
}
