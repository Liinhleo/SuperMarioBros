#pragma once
#include "GameObject.h"
#include "Timer.h"


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_STATE_FLYING 0
#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_YELLOW_ANI_FLY		0
#define GOOMBA_YELLOW_ANI_WALKING	1
#define GOOMBA_YELLOW_ANI_DIE		2

#define GOOMBA_RED_ANI_FLY		3
#define GOOMBA_RED_ANI_WALKING	4
#define GOOMBA_RED_ANI_DIE		5

// TypeGoomba
#define GOOMBA_YELLOW	1
#define GOOMBA_RED		2

#define TIME_DISAPPEAR	2000


class CGoomba : public CGameObject
{
	int goombaType;

public: 

	Timer* timeDisappear = new Timer(TIME_DISAPPEAR);

	bool isWing; // 0: non -> state walk || 1: wing ->state fly
	bool isOnGround = false;

	CGoomba(int type, bool isWing);

	int GetGoombaType() { return this->goombaType; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);
};