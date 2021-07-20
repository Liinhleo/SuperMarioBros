#include "Plant.h"

Plant::Plant(float x, float y) { 
	this->x = x;
	this->y = y;
	GetPosition(this->start_x, this->start_y);
	SetState(PLANT_STATE_MOVING_UP);
}

void Plant::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_DESTROY: // hit by weapon
		break;
	case PLANT_STATE_ATTACK:
		vy = 0;
		attackTimer->Start();
		break;
	case PLANT_STATE_HIDDEN: // nam ngoai vung hoat dong -> tinh theo thoi gian de up/down 
		vy = 0;
		hiddenTimer->Start();
		break;
	case PLANT_STATE_MOVING_UP:
		vy = -PLANT_SPEED_Y;
		break;
	case PLANT_STATE_MOVING_DOWN:
		vy = PLANT_SPEED_Y;
		break;
	}
}


void Plant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player) {
	CGameObject::Update(dt);
	y += dy; // di chuyen len xuong theo dy 

	// xu ly logic up -> down 
}
