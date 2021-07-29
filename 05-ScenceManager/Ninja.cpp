#include "Ninja.h"
#include "Ground.h"

#define NINJA_STATE_IDLE	0
#define NINJA_STATE_WALKING	100
#define NINJA_STATE_ATTACK	200

#define NINJA_ANI_WALKING_RIGHT		0
#define NINJA_ANI_WALKING_LEFT		1
#define NINJA_ANI_DIE				2

#define LIMIT_POSITION				32 // gioi han vi tri di chuyen

#define NINJA_BBOX_WIDTH	16
#define NINJA_BBOX_HEIGHT	24


Ninja::Ninja() {
	this->type = ObjectType::NINJA;
	this->start_x = x;
	this->start_y = y;
	this->nx = 1;
	SetState(NINJA_STATE_WALKING);
}


void Ninja::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == STATE_DESTROYED || state == ENEMY_STATE_DAMAGE || state == ENEMY_STATE_DIE_BY_ATTACK)
		left = top = right = bottom = 0;
	else
	{
		top = y;
		left = x;
		right = x + NINJA_BBOX_WIDTH;
		bottom = y + NINJA_BBOX_HEIGHT;
	}
}

void Ninja::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;

	// turn off collision when die 
	if (state == ENEMY_STATE_DIE_BY_ATTACK || state == ENEMY_STATE_DAMAGE) {
		x += dx;
		y += dy;
	}

	if (x > start_x + LIMIT_POSITION)
	{
		x = start_x + LIMIT_POSITION;
		nx = -1;
		SetState(NINJA_STATE_IDLE);
	}
	if (x < start_x)
	{
		x = start_x;
		nx = 1;
		SetState(NINJA_STATE_IDLE);
	}

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.25f;

		if (ny != 0) {
			vy = 0;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == ObjectType::GROUND) {
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0) {
					if (ground->isInteract)	{
						x += dx;
					}
					else
						vx = -vx;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	
}
void Ninja::Render() {
	if (state == STATE_DESTROYED)
		return;
	if (state == ENEMY_STATE_DAMAGE || state == ENEMY_STATE_DIE_BY_ATTACK) {
		ani = NINJA_ANI_DIE;
	}
	if (state == NINJA_STATE_WALKING) {
		if (vx < 0)	ani = NINJA_ANI_WALKING_RIGHT;
		else ani = NINJA_ANI_WALKING_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Ninja::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_DESTROYED: // bullet
		vx = 0;
		vy = 0;
		break;
	case ENEMY_STATE_DAMAGE:
	case ENEMY_STATE_DIE_BY_ATTACK:
		vy = -0.05f;

		if (nx > 0)
		{
			vx = 0.05f;
		}
		else
		{
			vx = -0.05f;
		}
		break;
	case NINJA_STATE_IDLE:
		vx = nx * 0.003f;
		break;
	case NINJA_STATE_WALKING:
		vx = nx * 0.003f;
		break;
	}
}