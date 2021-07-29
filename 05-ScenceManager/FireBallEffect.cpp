#include "FireBallEffect.h"

FireBallEffect::FireBallEffect(D3DXVECTOR2 position) {
	x = position.x;
	y = position.y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(41));
	effectTimer->Start();
}



void FireBallEffect::GetBoundingBox(float& l, float& t, float& r, float& b) {}

void FireBallEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	y += dy;

	if (effectTimer->IsTimeUp())
		state = STATE_DESTROYED;
}

void FireBallEffect::Render() {
	animation_set->at(ani)->Render(x, y);
}