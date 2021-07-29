#include "SuperMushroom.h"
#include "Ground.h"
#include "Brick.h"
#include "Utils.h"

SuperMushroom::SuperMushroom(D3DXVECTOR2 position, int type) {
	this->x = position.x;
	this->y = position.y;

	ItemType = type;
	this->type = ObjectType::ITEM;

	vx = -ITEM_SPEED_Y;
}

void SuperMushroom::Render()
{
	if (ItemType == ITEM_RED_MUSHROOM) ani = ITEM_ANI_RED_MUSHROOM;
	else ani = ITEM_ANI_GREEN_MUSHROOM;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void SuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	Item::Update(dt, coObjects);
	this->vy += (ITEM_GRAVITY * dt); // Simple fall down

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
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

		if (ny != 0)//va cham theo truc y
		{
			vy = 0;
		}
	
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny > 0) {
				if (e->obj->GetType() == ObjectType::GROUND) {
					
					CGround* ground = dynamic_cast<CGround*>(e->obj);
					
					if (e->nx != 0) {
						if (ground->isInteract) {
							x += dx;
						}
						else {
							this->vx = -this->vx; // doi huong
						}
					}
				}

				else if (e->obj->GetType() == ObjectType::BRICK || e->obj->GetType() == ObjectType::PIPE) {
					if (e->nx != 0) {
						this->vx = -this->vx; // doi huong
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
