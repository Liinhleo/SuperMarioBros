#pragma once
#include "GameObject.h"
#include "Timer.h"

class CardItemEffect :
    public CGameObject
{
    LPANIMATION_SET aniCard;
public:
	CardItemEffect(D3DXVECTOR2 position);
	~CardItemEffect() {};

	Timer* effectTimer = new Timer(400);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

