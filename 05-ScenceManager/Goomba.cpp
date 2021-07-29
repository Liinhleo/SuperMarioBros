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
	vy += MARIO_GRAVITY * dt;

	
	if (state == ENEMY_STATE_DAMAGE) { 
		if (isWing) {
			isWing = false;
			SetState(GOOMBA_STATE_WALKING);
		}
		else if (!isWing && timeDestroy->IsTimeUp()) {
				SetState(STATE_DESTROYED);
				timeDestroy->Stop();
		}
	}

	// Die -> disappear
	if (GetState()==ENEMY_STATE_DAMAGE && timeDisappear->IsTimeUp()) {
		timeDisappear->Stop();
		SetState(STATE_DESTROYED);
		// Xu ly khong ve nua -> xu ly ben playScence
	
	}

	if (isWing) {
		if (timeStartJump->IsTimeUp() && timeStartJump->GetStartTime()) { // bd tinh time nhay
			timeStartJump->Stop();
			SetState(GOOMBA_STATE_JUMP);
		}
		if (state == GOOMBA_STATE_JUMP && isOnGround) {
			SetState(GOOMBA_STATE_FLYING);
		}
	}


	// XU LY VA CHAM
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	//CalcPotentialCollisions(coObjects, coEvents);

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

			if (e->obj->GetType() == ObjectType::GROUND) {
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0) { // va cham theo phuong x voi color box
					if (ground->isInteract){
						x += dx; //di xuyen qua
					}
					else {
						vx = -vx; // doi huong
					}
				}
			}

			else if (e->obj->GetType() == ObjectType::BRICK || e->obj->GetType() == ObjectType::PIPE) {
				if (e->nx != 0) {
					vx = -vx; // doi huong
				}
			}

			else if (e->obj->GetType() == ObjectType::GOOMBA) { //dung nhau thi doi huong
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)	{
					if (this->GetGoombaType() == goomba->GetGoombaType()) {
						vx = -vx;
						goomba->vx = -vx;
					}
					else {
						x += dx; // khac loai thi di xuyen qua nhau dc
					}
				}
			}

			else if (e->obj->GetType() == ObjectType::KOOPA) { //dung nhau thi doi huong
				if (e->nx != 0) {
					x += dx; //di xuyen qua
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int GType = GetGoombaType();
	if (state == STATE_DESTROYED)
		return;

	switch (GType) {
	case GOOMBA_YELLOW:
		if (state == GOOMBA_STATE_FLYING || state == GOOMBA_STATE_JUMP) ani = GOOMBA_YELLOW_ANI_FLY;
		else if (state == ENEMY_STATE_DAMAGE) ani = GOOMBA_YELLOW_ANI_DIE; // bep di -> bien mat sau 2s 
		else if (state == ENEMY_STATE_DIE_BY_ATTACK) ani = GOOMBA_YELLOW_ANI_INVERSE; // bi lat nguoc va bay khoi screen
		else  ani = GOOMBA_YELLOW_ANI_WALKING;
		break;
	case GOOMBA_RED:
		if (state == GOOMBA_STATE_FLYING || state == GOOMBA_STATE_JUMP) ani = GOOMBA_RED_ANI_FLY;
		else if (state == ENEMY_STATE_DAMAGE) ani = GOOMBA_RED_ANI_DIE; // bep di -> bien mat sau 2s 
		else if (state == ENEMY_STATE_DIE_BY_ATTACK){	// bi lat nguoc va bay khoi screen
			if(isWing) ani = GOOMBA_RED_ANI_FLY_INVERSE;
			else ani = GOOMBA_RED_ANI_INVERSE; 
		}
		else  ani = GOOMBA_RED_ANI_WALKING;
		break;
	}

	animation_set->at(ani)->Render(x,y);

	// RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		// xu ly state bi thuong cua Goomba
		case STATE_DESTROYED: // bullet
			vx = 0;
			vy = 0;
			break;
		case ENEMY_STATE_DAMAGE: // jump on top and die
			timeDisappear->Start(); // bd tinh gio disappear
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = -GOOMBA_DIE_DEFLECT_SPEED;
			break;

		case ENEMY_STATE_DIE_BY_ATTACK: // by weapon -> out of map -> xoa
			vy = -GOOMBA_JUMP_SPEED ;
			vx = nx * GOOMBA_WALKING_SPEED;
			break;

		case GOOMBA_STATE_WALKING: 
			isWing = false;
			vx = -GOOMBA_WALKING_SPEED;
			break;

		case GOOMBA_STATE_FLYING:
			timeStartJump->Start();
			isOnGround = false;
			vy = -GOOMBA_JUMP_SPEED * 0.6;
			vx = -GOOMBA_WALKING_SPEED;

		case GOOMBA_STATE_JUMP:
			isOnGround = false;
			vy =  -GOOMBA_JUMP_SPEED;
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_FLYING) {
		left = x;
		top = y;
		right = x + GOOMBA_WING_BBOX_WIDTH;
		bottom = y + GOOMBA_WING_BBOX_HEIGHT;
	}
	else {
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == ENEMY_STATE_DAMAGE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}

}
