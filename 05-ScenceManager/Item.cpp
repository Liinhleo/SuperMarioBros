#include "Item.h"

Item::Item() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(30));
}


void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}