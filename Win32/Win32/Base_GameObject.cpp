#include"Game.h"


GamePlay::Base_GameObject::Base_GameObject(float initialHealth):mHP(initialHealth)
{
}

void GamePlay::Base_GameObject::SetHP(float hp)
{
	mHP = hp;
}

float GamePlay::Base_GameObject::GetHP()
{
	return mHP;
}

void GamePlay::Base_GameObject::ReduceHP(float deltaHP)
{
	mHP -= deltaHP;
}

bool GamePlay::Base_GameObject::IsDead()
{
	return (mHP <= 0.0f);
}
