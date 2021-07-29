#include "Brick.h"
#include "Utils.h"

CBrick::CBrick(float x, float y, int typeBrick, int typeItem, int count) {
	
	brickType = typeBrick;
	this->type = ObjectType::BRICK;
	
	this->x = x;
	this->y = y;
	this->start_x = x;
	this->start_y = y;
	GetPosition(this->start_x, this->start_y);

	this->typeItem = typeItem;
	this->count = count;

	SetState(BRICK_STATE_ACTIVE);
}
void  CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	//DebugOut(L"type Brick %d \n", brickType);
	//DebugOut(L"staet Brick %d \n", state);
	//DebugOut(L"x Brick %f \n", x);
	//DebugOut(L"y Brick %f \n", y);

	if (y < (start_y - 5.0f) && vy < 0)
	{
		vy = -vy;
	}
	if (y > start_y)
	{
		y = start_y;
		SetState(BRICK_STATE_ACTIVE);
	}
	y += dy;
}


void CBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case BRICK_STATE_BOUNDING:
		if ((GetTypeItem() == CONTAIN_ITEM_UP || GetTypeItem() == CONTAIN_GREEN_MUSHROOM) && count == 1)
			isFallingItem = true;
		vy = -0.2f;
		DebugOut(L"CONTAIN: %d \n", count);
		break;

	case BRICK_STATE_HIDDEN:
		break;

	case BRICK_STATE_BROKEN:
		isBroken = true;
		break;

	case BRICK_STATE_ACTIVE:
		vy = 0;
		break;
	}
}

void CBrick::Render()
{
	if (GetBrickType() == BRICK_BROKEN || isBroken)
		ani = BRICK_ANI_BROKEN;
	else {
		if (state == BRICK_STATE_HIDDEN)
			ani = BRICK_ANI_HIDDEN;
		else
			ani = BRICK_ANI_ACTIVE;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIZE;
	b = y + BRICK_BBOX_SIZE;
}