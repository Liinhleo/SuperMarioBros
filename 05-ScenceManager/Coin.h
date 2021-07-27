#pragma once
#include "GameObject.h"
class Coin :
    public CGameObject
{
public:
	Coin();
	~Coin() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};

