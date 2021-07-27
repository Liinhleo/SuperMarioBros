#include "Ninja.h"


#define NINJA_STATE_WALKING	0
#define NINJA_STATE_ATTACK	100

#define NINJA_ANI_WALKING_RIGHT		0
#define NINJA_ANI_WALKING_LEFT		1
#define NINJA_ANI_ATTACK_RIGHT		2
#define NINJA_ANI_ATTACK_LEFT		3

#define LIMIT_POSITION				32 // gioi han vi tri di chuyen

#define NINJA_BBOX_WIDTH	16
#define NINJA_BBOX_HEIGHT	24


Ninja::Ninja() {
	this->type = ObjectType::NINJA;
	this->start_x = x;
	this->start_y = y;
	nx = 1;
	SetState(NINJA_STATE_WALKING);
}


void Ninja::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == STATE_DESTROYED || state == ENEMY_STATE_DAMAGE || state == ENEMY_STATE_DIE_BY_ATTACK)
		left = top = right = bottom = 0;
	else
	{
		top = y;
		left = x;
		right = x + NINJA_BBOX_WIDTH;
		bottom = y + NINJA_BBOX_HEIGHT;
	}
}

void Ninja::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;
}
void Ninja::Render() {

}

void Ninja::SetState(int state) {

}