#include "FireFlower.h"


FireFlower::FireFlower(float x, float y, int type) :Plant(x, y) {
	FlowerType = type;
	this->type = ObjectType::PIRANHA_FLOWER;

	hiddenTimer = new Timer(HIDDEN_TIME);
	attackTimer = new Timer(ATTACK_TIME);
}

void FireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + FIRE_FLOWER_BBOX_WIDTH;
	if (GetFlowerType() == FIRE_FLOWER_RED) {
		bottom = top + FIRE_FLOWER_RED_BBOX_HEIGHT;
	}
	else {
		bottom = top + FIRE_FLOWER_GREEN_BBOX_HEIGHT;
	}
}


void FireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player) {
	Plant::Update(dt, coObjects, player);
	// Xu ly logic sau
}


void FireFlower::Render() {
	int ani = -1;
	// xu ly render sau
	animation_set->at(ani)->Render(x, y);
}

void FireFlower::SetState(int state) {
	Plant::SetState(state);
	// xu ly sau
}
