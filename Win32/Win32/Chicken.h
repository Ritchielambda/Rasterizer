#pragma once
namespace GamePlay
{
	enum CHICKEN_MATERIAL_TYPE
	{
		CHICKEN_MATERIAL_TYPE_COMMON = 0,
	CHICKEN_MATERIAL_TYPE_HIT = 1
	};
	enum CHICKEN_ATTACK_STATE
	{
		CHICKEN_ATTACK_STATE_CHASE_PLAYER = 0,
		CHICKEN_ATTACK_STATE_TYPE1 = 1,
		CHICKEN_ATTACK_STATE_TYPE2 = 2,
		CHICKEN_ATTACK_STATE_ULTIMATE_EXPLODE = 3,
	};


	class ChickenMonster :public Base_GameObject
	{
	public:
		ChickenMonster(BulletManager* pBulletMgr);

		void Init(SCENE_TYPE modelID);

		void Update(const FLOAT3& playpos);

		void Render();

		void GetBoundingBox( BOUNDINGBOX& outBox);

		void BeHitAndChangeColor();

		FLOAT3 Getposition();

		float GetInitialHealth() const;

	private:

		const float c_chickenInitialHealth = 2500.f;
		void		mFunction_UpdateMovement(const FLOAT3& shootDir);

		void		mFunction_Fire(FLOAT3 firePos);

		void		mFunction_UpdateTexture();

		BulletManager* const m_pBulletMgr;

		FLOAT3 mPos;

		FLOAT3 mLookat;

		Mesh	mMesh;

		Texture2D	mTexture_Common;
		CHICKEN_MATERIAL_TYPE mMatType;
		Material			mMat_Common;
		Material			mMat_Red;

		CHICKEN_ATTACK_STATE m_AttackState;
	};

}