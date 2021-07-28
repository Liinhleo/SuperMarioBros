#include "Coin.h"

Coin::Coin() {
	this->type = ObjectType::COIN;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(32));
	//DebugOut
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
}


void Coin::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIZE;
	b = y + BRICK_BBOX_SIZE;
}

void Coin::Render() {
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}