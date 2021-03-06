#pragma once
#include "Enemy.h"
#include "Timer.h"

#define PLANT_STATE_HIDDEN			0
#define PLANT_STATE_MOVING_UP		100
#define PLANT_STATE_ATTACK			200
#define PLANT_STATE_MOVING_DOWN		300

#define HIDDEN_TIME		2500
#define ATTACK_TIME		2500

#define PLANT_SPEED_Y  0.05f

class Plant : public Enemy
{
public:

	Plant(float x, float y);
	~Plant() {};

	Timer* hiddenTimer;
	Timer* attackTimer;

	virtual void GetZoneAtive(float& left, float& top, float& right, float& bottom);
	bool CheckPlayerInZone(D3DXVECTOR4 player);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Render() = 0;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player);
	virtual void SetState(int state) ;
};

