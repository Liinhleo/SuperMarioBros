#pragma once
#include "GameObject.h"
#include "Timer.h"

#define TIME_SCORE_EFFECT	400
#define SCORE_SPEED_Y	0.08f

class ScoreEffect :
    public CGameObject
{
	int score;
	bool beBounded; //ktra tien co nay len

public:
	ScoreEffect(D3DXVECTOR2 position, int score);
	~ScoreEffect() {};

	Timer* effectTimer = new Timer(TIME_SCORE_EFFECT);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};

