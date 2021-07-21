#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TIME_DESTROY_BY_WEAPON	300 // time bien mat

#define ENEMY_STATE_DAMAGE				1 // bi thuong khi nhay len dau
#define ENEMY_STATE_DIE_BY_ATTACK		2 // bi thuong khi khi bi attack by weapon
#define ENEMY_STATE_DESTROY				3 // chet lun

class Enemy : public CGameObject
{
public:
	Timer* timeDestroy = new Timer(TIME_DESTROY_BY_WEAPON);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	virtual void attackOnTop(); 
	virtual void attackByWeapon(); // tail, fire
};

