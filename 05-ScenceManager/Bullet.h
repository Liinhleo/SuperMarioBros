#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH	8
#define BULLET_BBOX_HEIGHT	8


#define BULLET_GRAVITY 0.0008f //gravity
#define BULLET_SPEED_X	0.2f 
#define BULLET_SPEED_Y	0.1f 

class Bullet :  public CGameObject
{
public:
	Bullet(D3DXVECTOR2 position, int nx);
	~Bullet() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};

