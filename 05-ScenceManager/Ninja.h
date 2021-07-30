#pragma once
#include "Enemy.h"
#include "NinjaBoomerang.h"

#define NINJA_STATE_IDLE	0
#define NINJA_STATE_ATTACK	100
#define NINJA_STATE_FORWARD	300
#define NINJA_STATE_GO_BACK	400

#define NINJA_ANI_IDLE_RIGHT		0
#define NINJA_ANI_IDLE_LEFT			1
#define NINJA_ANI_WALKING_RIGHT		2
#define NINJA_ANI_WALKING_LEFT		3
#define NINJA_ANI_DIE				4

#define LIMIT_POSITION				80 // gioi han vi tri di chuyen giua x -> end_x (5brick)
#define NEAREST_DISTANCE_MARIO		16 // 1 brick

#define NINJA_BBOX_WIDTH	16
#define NINJA_BBOX_HEIGHT	24

class Ninja :
    public Enemy
{
public:
	float end_x; 

	vector< LPGAMEOBJECT> listBoomerang;
	NinjaBoomerang* CreateBoomerang(float x, float y, int nx)
	{
		NinjaBoomerang* boomerang = new NinjaBoomerang({ x, y }, nx);
		return boomerang;
	}

	Timer* timeIdle = new Timer(300); 
	Timer* timeAttacking = new Timer(1200);
	Timer* timeWaitAttack = new Timer(1000); // thoi gian cho truoc khi attack lai

	bool isAttacking;

	Ninja(D3DXVECTOR2 position);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);
};

