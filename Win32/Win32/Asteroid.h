#pragma once
namespace GamePlay
{
	enum AsteroidType
	{
		AsteroidType_Rock1 = 0,
		AsteroidType_Rock2 = 1,
		AsteroidType_Rock3 = 2,
		AsteroidType_Box = 3,
	};
	
	class Asteroid
	{
	public:

		void Init(UINT AsteroidType);

		void Update();

		void Render();

		void GetBoundingBox(BOUNDINGBOX& outBox);
		
	private:
		Texture2D			mTexture;
		Mesh				mMesh;
		FLOAT3				mRotateSpeed;
		FLOAT3				mMoveSpeed;
	};
}