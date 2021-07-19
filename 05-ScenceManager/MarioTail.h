#pragma once
#include "GameObject.h"

#define TAIL_STATE_HIT			0
#define TAIL_STATE_DISABLE		100

#define TAIL_ANI_RIGHT			0	
#define TAIL_ANI_LEFT			1

#define TAIL_BBOX_HEIGHT		6
#define TAIL_BBOX_WIDTH			9
#define DISTANCE_XTAIL_MARIO	14 // kc tu vi tri x cua tail


class MarioTail : public CGameObject
{
	//bool isTail = false;

	MarioTail();
	~MarioTail() {};

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 playerPos, int playerNx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
};

