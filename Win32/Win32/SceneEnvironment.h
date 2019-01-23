#pragma once
namespace GamePlay
{
	enum SCENE_TYPE
	{
		SCENE_COSMOS1 = 1,
		SCENE_CHECKERBORAR=2,
	};

	class SceneModelManager
	{
	public:
		void Init(SCENE_TYPE type);

		void Update();

		void Render();

		void GetAsteroidBoundingBox(UINT index, BOUNDINGBOX& outBox);

		UINT GetAsteroidCount() const;

	private:
		Material		mDefaultMaterial;
		Texture2D		mSkyTexture;
		Mesh			mSkyModel;

		static const UINT c_asteroidCount = 15;

		Asteroid		mAsteroid[c_asteroidCount];

		DirectionalLight	mSceneLight;
	};
}