#include "BrickBrokenEffect.h"


BrickBrokenEffect::BrickBrokenEffect(D3DXVECTOR2 position, int nx, float v)
{
	this->x = position.x;
	this->y = position.y;
	this->vx = BROKEN_SPEED_X * nx;
	this->vy = -BROKEN_SPEED_Y * v;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(40));
}

void BrickBrokenEffect::Render()
{
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}
void BrickBrokenEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
void BrickBrokenEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.00008f * dt;
	x += dx;
	y += dy;
	if (this->isOutOfCam())
		SetState(STATE_DESTROYED);
}

