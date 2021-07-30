#pragma once
#include "Plant.h"

#define PIRANHA_BBOX_WIDTH		16
#define PIRANHA_BBOX_HEIGHT		24

class PiranhaFlower :
    public Plant
{
public:
	PiranhaFlower(float x, float y);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player);
	virtual void Render();
	virtual void SetState(int state);
};

