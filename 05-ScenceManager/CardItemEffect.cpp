#include "CardItemEffect.h"
#include "Mario.h"

CardItemEffect::CardItemEffect(D3DXVECTOR2 position)
{
	x = position.x;
	y = position.y;
	aniCard = CAnimationSets::GetInstance()->Get(1130);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(42));
	effectTimer->Start();
}

void CardItemEffect::GetBoundingBox(float& l, float& t, float& r, float& b) {}

void CardItemEffect::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (effectTimer && effectTimer->IsTimeUp())
		state = STATE_DESTROYED;
}

void CardItemEffect::Render()
{
	int n = CMario::GetInstance()->cards.size();
	aniCard->at(CMario::GetInstance()->cards.at(n - 1))->Render(x + 128, y + 24);
	animation_set->at(ani)->Render(x, y);
}
