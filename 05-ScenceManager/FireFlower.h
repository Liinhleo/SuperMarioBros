#pragma once
#include "Plant.h"

// FIRE FLOWER RED 
#define FIRE_FLOWER_ANI_RED_RIGHT_DOWN	0
#define FIRE_FLOWER_ANI_RED_RIGHT_UP	1
#define FIRE_FLOWER_ANI_RED_LEFT_DOWN	2
#define FIRE_FLOWER_ANI_RED_LEFT_UP		3

// FIRE FLOWER GREEN
#define FIRE_FLOWER_ANI_GREEN_RIGHT_DOWN	4
#define FIRE_FLOWER_ANI_GREEN_RIGHT_UP		5
#define FIRE_FLOWER_ANI_GREEN_LEFT_DOWN		6
#define FIRE_FLOWER_ANI_GREEN_LEFT_UP		7

//FlowerType
#define FIRE_FLOWER_RED		1
#define FIRE_FLOWER_GREEN	2

// bbox
#define FIRE_FLOWER_BBOX_WIDTH			16
#define FIRE_FLOWER_RED_BBOX_HEIGHT		32
#define FIRE_FLOWER_GREEN_BBOX_HEIGHT	24


class FireFlower : public Plant
{
	int FlowerType;
public:
	FireFlower(float x, float y, int type);
	
	int GetFlowerType() { return this->FlowerType; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player);
	virtual void Render();
	virtual void SetState(int state);

};

