#include "LastItem.h"

LastItem::LastItem() {
	this->type = ObjectType::LAST_ITEM;
}

void LastItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

}
void LastItem::Render()
{
	//0: star || 1: mushroom || 2: flower
	animation_set->at(ani)->Render(x, y);
}

void LastItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == STATE_DESTROYED)
		l = t = b = r = 0;
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_SIZE;
		b = y + BRICK_BBOX_SIZE;
	}
}