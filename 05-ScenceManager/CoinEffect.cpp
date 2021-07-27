#include "CoinEffect.h"
#include "Utils.h"


CoinEffect::CoinEffect(D3DXVECTOR2 position)
{
	this->type = ObjectType::COIN;

	x = position.x;
	y = position.y - BRICK_BBOX_SIZE;

	GetPosition(start_x, start_y);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(32));
	vy = -COIN_SPEED_Y;
	beBounded = false;
}

void CoinEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CoinEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (state != STATE_DESTROYED)
		vy += MARIO_GRAVITY * dt;

	else
		vy = 0;

	if (vy >= 0)
		beBounded = true;

	if (beBounded && y > start_y)
		state = STATE_DESTROYED;
}

void CoinEffect::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}
