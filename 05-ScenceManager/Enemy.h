#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TIME_DESTROY_BY_WEAPON	300

#define ENEMY_STATE_DAMAGE				1
#define ENEMY_STATE_DIE_BY_ATTACK		2
#define ENEMY_STATE_DESTROY				3

class Enemy : public CGameObject
{

public:
	Timer* timeDestroy = new Timer(TIME_DESTROY_BY_WEAPON);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Render() = 0;

	virtual void attackOnTop(); 
	virtual void attackByWeapon(); // tail, fire


};

