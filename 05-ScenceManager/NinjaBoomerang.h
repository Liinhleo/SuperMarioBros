#pragma once
#include "GameObject.h"

#define BOOMERANG_STATE_ACTIVE	0
#define BOOMERANG_STATE_RETURN	1

#define BOOMERANG_ANI_ACTIVE	0
#define BOOMERANG_STATE_RETURN	1

#define DISTANCE_TO_RETURN_X	128

#define BOOMERANG_BBOX_SIZE		16	

#define BOOMERANG_GRAVITY		0.0008f
#define BOOMERANG_SPEED_X		0.1f
#define BOOMERANG_SPEED_Y		0.01f


class NinjaBoomerang :
    public CGameObject
{
public:
	NinjaBoomerang(D3DXVECTOR2 position, int nx);
	~NinjaBoomerang() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	void SetState(int state);
};

