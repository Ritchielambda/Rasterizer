#pragma once

namespace GamePlay
{
	struct Bullet
	{
		FLOAT3 pos;
		FLOAT3 velocity;
		FLOAT3 color;
		float travelledDistance;
	};

	class BulletManager
	{
	public:
		BulletManager();

		~BulletManager();


		//generate a new bullets

		void SpawnBullet(const FLOAT3& pos, const FLOAT3 velocity, const FLOAT3 color);

		void killBullet(UINT index);

		void killAllBullet();

		void collisionDection(BOUNDINGBOX& testBox, bool killCollideBullets, std::vector<FLOAT3>& outCollidePointList);

		void UpdateBullets();

		void Render();

	private:

		std::vector<Bullet>* m_pBulletList;

		PointCollection		mRenderPointList;
	};
}
