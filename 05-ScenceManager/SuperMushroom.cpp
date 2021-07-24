#include "SuperMushroom.h"
#include "Ground.h"
#include "Brick.h"

SuperMushroom::SuperMushroom(D3DXVECTOR2 position, int type) {
	this->x = position.x;
	this->y = position.y;

	ItemType = type;
	this->type = ObjectType::ITEM;
	vx = -ITEM_SPEED_Y;
}

void SuperMushroom::Render()
{
	if (ItemType == ITEM_RED_MUSHROOM)
		ani = ITEM_ANI_RED_MUSHROOM;
	else
		ani = ITEM_ANI_GREEN_MUSHROOM;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void SuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	Item::Update(dt, coObjects);
	vy += (ITEM_GRAVITY * dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.4f;

		if (ny != 0) // va cham voi ground thi giu lai
		{
			vy = 0;
		}
		//
	}
}
