#pragma once
#include "Bullet.h"

// zone: 8 huong ban
// 
#define LEFT_DOWN_FAR		1
#define LEFT_DOWN_NEAR		2
#define LEFT_UP_FAR			3
#define LEFT_UP_NEAR		4

#define RIGHT_DOWN_FAR		5
#define RIGHT_DOWN_NEAR		6
#define RIGHT_UP_FAR		7
#define RIGHT_UP_NEAR		8


class FireFlowerBall : public Bullet
{
	int zone;
public:
	FireFlowerBall(D3DXVECTOR2 position, int nx, int zoneAttack);
	~FireFlowerBall() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetSpeed();
};

