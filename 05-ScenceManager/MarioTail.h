#pragma once
#include "GameObject.h"

#define TAIL_STATE_HIT			0

#define TAIL_ANI_RIGHT			0	
#define TAIL_ANI_LEFT			1

#define TAIL_BBOX_HEIGHT		6
#define TAIL_BBOX_WIDTH			10
#define DISTANCE_XTAIL_MARIO	7 // kc tu vi tri x cua tail


class MarioTail : public CGameObject
{
	static MarioTail* __instance;

public:
	MarioTail();
	~MarioTail() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void SetState(int state);

	static MarioTail* GetInstance();
};

