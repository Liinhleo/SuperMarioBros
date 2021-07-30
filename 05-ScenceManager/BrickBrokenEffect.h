#pragma once
#include "GameObject.h"

#define BROKEN_SPEED_Y	0.1f
#define BROKEN_SPEED_X	0.03f

class BrickBrokenEffect :
    public CGameObject
{
public:
	BrickBrokenEffect(D3DXVECTOR2 position, int nx, float v);
	~BrickBrokenEffect() {}

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};

