#pragma once
#include "GameObject.h"
#define COIN_SPEED_Y	0.2f

class CoinEffect :
    public CGameObject
{
    bool beBounded; //ktra tien co nay len
	DWORD timeStartEffect;

public:
	CoinEffect(D3DXVECTOR2 position);
	~CoinEffect() {};

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};

