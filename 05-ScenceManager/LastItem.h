#pragma once
#include "GameObject.h"
class LastItem :
    public CGameObject
{
public:
	bool isHit = false;

	LastItem();
	~LastItem() {}

	void Hit() { this->isHit = true; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};

