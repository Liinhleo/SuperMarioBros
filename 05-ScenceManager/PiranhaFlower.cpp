#include "PiranhaFlower.h"

PiranhaFlower::PiranhaFlower(float x, float y):Plant(x, y) {
	type = ObjectType::PIRANHA_FLOWER;

	hiddenTimer = new Timer(HIDDEN_TIME);
	attackTimer = new Timer(ATTACK_TIME);
}

void PiranhaFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + PIRANHA_BBOX_WIDTH;
	bottom = top + PIRANHA_BBOX_HEIGHT;
}


void PiranhaFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player) {
	Plant::Update(dt, coObjects, player);
	// Xu ly logic sau
}


void PiranhaFlower::Render() {
	int ani = 0;
	animation_set->at(ani)->Render(x, y);
}

void PiranhaFlower::SetState(int state) {
	Plant::SetState(state);
}
