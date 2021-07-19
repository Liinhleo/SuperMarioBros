#include "Goomba.h"
#include "Ground.h"
#include "Brick.h"
#include "Mario.h"

CGoomba::CGoomba(int type, bool isWing){
	goombaType = type;
	this->type = ObjectType::GOOMBA;
	this->isWing = isWing;
	
	if (isWing == true) {
		SetState(GOOMBA_STATE_FLYING);
	}
	else {
		SetState(GOOMBA_STATE_WALKING);
	}
}


void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += 0.00005f * dt;

	if (timeDisappear->IsTimeUp()) {
		timeDisappear->Stop();
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0) // no collision
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.25f;

		if (ny != 0)
		{
			vy = 0;
			if (ny < 0) // va cham ground
			{
				isOnGround = true;
			}
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == ObjectType::GROUND)
			{
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0)
				{
					if (ground->isInteract)
					{
						x += dx; //di xuyen qua
					}
					else
						vx = -vx; // doi huong
				}
			}
			else if (e->obj->GetType() == ObjectType::BRICK || e->obj->GetType() == ObjectType::PIPE)
			{
				if (e->nx != 0)
				{
					vx = -vx; // doi huong
				}
			}
			else if (e->obj->GetType() == ObjectType::GOOMBA)
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					if (this->GetGoombaType() == goomba->GetGoombaType())
					{
						vx = -vx;
						goomba->vx = -vx;
					}
					else
						x += dx;
				}
			}
			else if (e->obj->GetType() == ObjectType::KOOPA)
			{
				if (e->nx != 0)
					x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = -1;
	int GType = GetGoombaType();

	switch (GType) {
	case GOOMBA_YELLOW:
		if (state == GOOMBA_STATE_FLYING) ani = GOOMBA_YELLOW_ANI_FLY;
		else if (state == GOOMBA_STATE_DIE) ani = GOOMBA_YELLOW_ANI_DIE;
		else  ani = GOOMBA_YELLOW_ANI_WALKING;

	case GOOMBA_RED:
		if (state == GOOMBA_STATE_FLYING) ani = GOOMBA_RED_ANI_FLY;
		else if (state == GOOMBA_STATE_DIE) ani = GOOMBA_RED_ANI_DIE;
		else  ani = GOOMBA_RED_ANI_WALKING;
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			timeDisappear->Start(); // bd tinh gio bay
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}


void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}
