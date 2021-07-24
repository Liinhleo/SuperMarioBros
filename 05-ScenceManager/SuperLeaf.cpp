#include "SuperLeaf.h"


SuperLeaf::SuperLeaf(D3DXVECTOR2 position) {
	this->x = position.x;
	this->y = position.y;
	isFalling = false;

	ItemType = ITEM_SUPERLEAF;
	this->type = ObjectType::ITEM;

}

void SuperLeaf::Render()
{
	if (nx > 0) ani = LEAF_ANI_RIGHT;
	else ani = LEAF_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void SuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Item::Update(dt, coObjects);

	x += dx;
	y += dy;
}
