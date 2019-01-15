#pragma once
namespace GamePlay
{
	class QPlayer :public Base_GameObject
	{
	public:

		QPlayer();//todo  bullet

		void Init();
		
		void Update();

		void Render();

		void GetBoundingBox(BOUNDINGBOX& outBox);

		FLOAT3 GetPosition();

		FLOAT3 GetPrevPosition();

		float InitialHealth() const;

	private:

		const float c_playerInitalHealth = 300.0f;

		void mFunction_Fire(float timeElapsed);

		void mFunction_UpdateMovement(float timeElapsed);
		

		//todo bullet func

		FLOAT3 mLastPos;

		FLOAT3 mCurrentPos;


		//dont't know what to do
		static const float	c_halfPlayerWidth;
		static const float	c_halfPlayerHeight;
		static const float	c_halfPlayerDepth;


	};
}