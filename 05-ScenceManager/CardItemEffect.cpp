#include "CardItemEffect.h"
#include "Mario.h"

CardItemEffect::CardItemEffect(D3DXVECTOR2 position)
{
	x = position.x;
	y = position.y;
	aniCard = CAnimationSets::GetInstance()->Get(1130);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(43));
	effectTimer->Start();
}

void CardItemEffect::GetBoundingBox(float& l, float& t, float& r, float& b) {}

void CardItemEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (effectTimer && effectTimer->IsTimeUp())
		state = STATE_DESTROYED;
}

void CardItemEffect::Render()
{
	animation_set->at(ani)->Render(x, y);

	if (animation_set->at(ani)->getCurrentFrame() == 1)
	{
		int n = CMario::GetInstance()->cards.size();
		aniCard->at(CMario::GetInstance()->cards.at(n - 1))->Render(x + 128, y + 24);
	}
}
