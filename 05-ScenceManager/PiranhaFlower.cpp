#include "PiranhaFlower.h"
#include "Utils.h"
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
	// step 1: (state = moving up)
	if (GetState() == PLANT_STATE_MOVING_UP && y < start_y - PIRANHA_BBOX_HEIGHT) {
		y = start_y - PIRANHA_BBOX_HEIGHT;
		SetState(PLANT_STATE_ATTACK);
	}
}


void PiranhaFlower::Render() {
	animation_set->at(0)->Render(x, y);
}

void PiranhaFlower::SetState(int state) {
	Plant::SetState(state);
}
