#pragma once
#include "Enemy.h"
#include "NinjaBoomerang.h"

class Ninja :
    public Enemy
{
public:
	vector< LPGAMEOBJECT> listBoomerang;
	NinjaBoomerang* CreateBoomerang(float x, float y, int nx)
	{
		NinjaBoomerang* boomerang = new NinjaBoomerang({ x, y }, nx);
		return boomerang;
	}

	Timer* timeAttacking = new Timer(3000);

	bool isWing; // 0: non -> state walk || 1: wing ->state fly
	bool isOnGround = false;

	Ninja();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);
};

