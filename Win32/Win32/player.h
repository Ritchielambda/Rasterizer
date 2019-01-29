#pragma once
namespace GamePlay
{
	class QPlayer :public Base_GameObject
	{
	public:

		QPlayer() :Base_GameObject(c_playerInitalHealth) { m_pBulletMgr = new BulletManager; }
		QPlayer(BulletManager* BulletMgr);

		void Init();
		
		void Update();

		void Render();

		void GetBoundingBox(BOUNDINGBOX& outBox);

		FLOAT3 GetPosition();

		FLOAT3 GetPrevPosition();

		float GetInitialHealth() const;

	private:

		const float c_playerInitalHealth = 300.0f;

		void mFunction_Fire(float timeElapsed);

		void mFunction_UpdateMovement(float timeElapsed);
		


		BulletManager*	 m_pBulletMgr;
		FLOAT3 mLastPos;

		FLOAT3 mCurrentPos;


		//dont't know what to do
		static const float	c_halfPlayerWidth;
		static const float	c_halfPlayerHeight;
		static const float	c_halfPlayerDepth;


	};
}