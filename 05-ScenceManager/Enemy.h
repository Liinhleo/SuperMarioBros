#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TIME_DESTROYED	500 // time bien mat

#define ENEMY_STATE_DAMAGE				1 // bi thuong khi nhay len dau
#define ENEMY_STATE_DIE_BY_ATTACK		2 // bi thuong khi khi bi attack by weapon

class Enemy : public CGameObject
{
public:
	Timer* timeDestroy = new Timer(TIME_DESTROYED);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	virtual void damageOnTop(); 
	virtual void damageByWeapon(); // tail, fire
};

