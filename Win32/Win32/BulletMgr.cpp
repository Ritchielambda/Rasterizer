#include"Game.h"

GamePlay::BulletManager::BulletManager() : m_pBulletList(new std::vector<Bullet>)
{
}

GamePlay::BulletManager::~BulletManager()
{
	delete m_pBulletList;
}

void GamePlay::BulletManager::SpawnBullet(const FLOAT3 & pos, const FLOAT3 velocity, const FLOAT3 color)
{
	Bullet bullet;
	bullet.pos = pos;
	bullet.velocity = velocity;
	bullet.color = color;
	bullet.travelledDistance = 0.0f;

	m_pBulletList->push_back(bullet);

	Vertex v;
	v.m_Position = {pos.x,pos.y,pos.z,1.0f};
	v.Color = { color.x,color.y,color.z,1.0f };
	mRenderPointList.AddPoint(v);
}

void GamePlay::BulletManager::killBullet(UINT index)
{
	if (index < m_pBulletList->size())
	{
		m_pBulletList->erase(m_pBulletList->begin() + index);
		mRenderPointList.DeletePoint(index);
	
	}
}

void GamePlay::BulletManager::killAllBullet()
{
	m_pBulletList->clear();
	mRenderPointList.ClearAll();
}

void GamePlay::BulletManager::collisionDection(BOUNDINGBOX & testBox, bool killCollideBullets, std::vector<FLOAT3>& outCollidePointList)
{
	outCollidePointList.clear();

	for (UINT i = 0; i < m_pBulletList->size(); ++i)
	{
		auto currBullet = m_pBulletList->at(i);
		FLOAT3 currentPos = currBullet.pos;
		FLOAT3 nextPos = currentPos + currBullet.velocity*gTimeElapsed;

		FLOAT3 outIntersetcPoint;

		if (Intersect_Ray_AABB(currentPos, nextPos, testBox, outIntersetcPoint, true))
		{
			//user might want to know the intersect point position
			outCollidePointList.push_back(outIntersetcPoint);
			//kill collide bullet
			if (killCollideBullets) killBullet(i);
		}
	}
}

void GamePlay::BulletManager::UpdateBullets()
{
	std::vector<UINT> bulletDeleteList;

	for (UINT i = 0; i < m_pBulletList->size();++i)
	{
		auto& currBullet = m_pBulletList->at(i);

		currBullet.pos += currBullet.velocity*gTimeElapsed;
		currBullet.travelledDistance += (currBullet.velocity.Length()*gTimeElapsed);

		mRenderPointList.SetPointPos(i, currBullet.pos);
		const float maxDistance = 500.0f;
		if (currBullet.travelledDistance >= maxDistance)
		{
			bulletDeleteList.push_back(i);
		}

	}

	for (auto idx : bulletDeleteList)
	{
		//deleting elements will cause list size to change
		static int offset = 0;

		killBullet(offset + idx);
		--offset;
	}
}

void GamePlay::BulletManager::Render()
{
	gRenderer.RenderPointCollection(mRenderPointList);
}
