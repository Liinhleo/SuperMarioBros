#pragma once
#include "GameObject.h"

#define TAIL_STATE_HIT			0
#define TAIL_STATE_DISABLE		100

#define TAIL_ANI_RIGHT			0	
#define TAIL_ANI_LEFT			1

#define TAIL_BBOX_HEIGHT		6
#define TAIL_BBOX_WIDTH			9
#define DISTANCE_XTAIL_MARIO	7 // kc tu vi tri x cua tail


class MarioTail : public CGameObject
{
	float *xMario, *yMario;
	int *nxMario;
public:
	MarioTail(float& x, float& y, int& nx);
	~MarioTail() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	bool isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void SetState(int state);

};

