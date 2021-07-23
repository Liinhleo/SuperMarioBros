#include "FireFlowerBall.h"
#include "Mario.h"

FireFlowerBall::FireFlowerBall(D3DXVECTOR2 position, int nx, int zoneAttack) : Bullet(position, nx) {
	this->zone = zoneAttack;
	this->type = ObjectType::FIRE_BALL;
}

void FireFlowerBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	SetSpeed();
	// chi xet va cham voi Mario -> object khac xuyen qua bth
	x += dx;
	y += dy;
}

void FireFlowerBall::SetSpeed() {
	vx = nx * BULLET_SPEED_X * 0.5;

	switch (zone) {
	case RIGHT_UP_FAR:		// 8
	case LEFT_UP_FAR:		// 3
		vy = abs(vx);
		break;

	case LEFT_UP_NEAR:		//4
	case RIGHT_UP_NEAR:		//7
		vy = abs(vx / 2);
		break;

	case RIGHT_DOWN_FAR:	//6
	case LEFT_DOWN_FAR:		//1
		vy = -abs(vx);
		break;

	case RIGHT_DOWN_NEAR:	//5
	case LEFT_DOWN_NEAR:	//2
		vy = -abs(vx / 2);
		break;
	}

}
