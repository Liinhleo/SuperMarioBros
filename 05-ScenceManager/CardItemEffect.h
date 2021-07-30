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

	Timer* effectTimer = new Timer(3000);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

