#pragma once
#include "Plant.h"
#include "FireFlowerBall.h"

// FIRE FLOWER RED 
#define FIRE_FLOWER_ANI_RIGHT_DOWN		0
#define FIRE_FLOWER_ANI_RIGHT_UP		1
#define FIRE_FLOWER_ANI_LEFT_DOWN		2
#define FIRE_FLOWER_ANI_LEFT_UP			3

//FlowerType
#define FIRE_FLOWER_RED		0
#define FIRE_FLOWER_GREEN	1

// bbox
#define FIRE_FLOWER_BBOX_WIDTH			16
#define FIRE_FLOWER_RED_BBOX_HEIGHT		32
#define FIRE_FLOWER_GREEN_BBOX_HEIGHT	24

// zone of Mario
#define MARIO_LEFT_DOWN_FAR		1
#define MARIO_LEFT_DOWN_NEAR	2
#define MARIO_LEFT_UP_FAR		3
#define MARIO_LEFT_UP_NEAR		4

#define MARIO_RIGHT_DOWN_FAR	5
#define MARIO_RIGHT_DOWN_NEAR	6
#define MARIO_RIGHT_UP_FAR		7
#define MARIO_RIGHT_UP_NEAR		8

#define MAX_ZONE	48

class FireFlower : public Plant
{
	int FlowerType;
	int zoneOfMario = 0;
	FireFlowerBall* fireBall = nullptr;

public:
	bool isAttack;
	FireFlower(float x, float y, int type);
	
	int GetFlowerType() { return this->FlowerType; }

	bool CheckDistancePlayer(D3DXVECTOR4 player);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player);
	virtual void Render();
	virtual void SetState(int state);
};

