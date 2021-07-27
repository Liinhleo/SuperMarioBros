#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TIME_SCORE_EFFECT	400
#define SCORE_SPEED_Y	0.08f

class FireBallEffect :
    public CGameObject
{
	
public:
	FireBallEffect(D3DXVECTOR2 position);
	~FireBallEffect() {};

	Timer* effectTimer = new Timer(TIME_SCORE_EFFECT);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};