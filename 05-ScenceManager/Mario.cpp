#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Ground.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	//Reset();
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	GetLevel();
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

int CMario::GetLevel() {
	return level;
}

void CMario::StartUntouchable() {
	untouchable = 1; 
	untouchable_start = GetTickCount(); 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	//DebugOut(L"current state: %d \n",state);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f; // nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty*dy + ny*0.4f;


		if (nx!=0) vx = 0;//va cham theo phuong x
		if (ny!=0) vy = 0;//va cham theo truc y
		

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


#pragma region COLLISION WITH GROUND
			if (dynamic_cast<CGround*>(e->obj)) // if e->obj is Ground 
			{
				if (e->ny < 0) {
					vy = 0; // cham dat
					isJump;
				}
				else if (e->ny > 0) {
					y += dy;
				}
			}
#pragma endregion

#pragma region COLLISION WITH GOOMBA
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba

#pragma endregion

#pragma region COLLISION WITH PORTAL
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
#pragma endregion

		}
		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// ngan Simon rot ra man hinh
	if (x <= 0)
		x = 0;

	/* HAM KTRA DE TRANH LAP LAI ANI LIEN TUC */
	// han che nhay lien tuc
	if (vy == 0) //va cham dat
		isJump = false;
}

void CMario::Render()
{
	int ani = -1;

	//if (state == MARIO_STATE_DIE)
	//{
	//	Die();
	//}

	// ====================== SMALL MARIO ======================
	if (GetLevel() == MARIO_LEVEL_SMALL) {
		if (state == MARIO_STATE_DIE) {
			ani = MARIO_ANI_DIE;
		}
		if (isWalking) {
			if (nx > 0) ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		else if (isStop) {
			if (nx > 0) ani = MARIO_ANI_SMALL_STOP_RIGHT;
			else ani = MARIO_ANI_SMALL_STOP_LEFT;
		}
		else if (isJump) {
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
		else if (isRun) {
			if (nx > 0) ani = MARIO_ANI_SMALL_RUN_RIGHT;
			else ani = MARIO_ANI_SMALL_RUN_LEFT;
		}
		else {
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
	}


	// ====================== BIG MARIO ======================
	if (GetLevel() == MARIO_LEVEL_BIG) {
		if (isWalking) {
			if (nx > 0) ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (isJump) {
			if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
			else ani = MARIO_ANI_BIG_JUMP_LEFT;
		}
		else if (isSitting) {
			if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
			else ani = MARIO_ANI_BIG_SIT_LEFT;
		}
		else if (isStop) {
			if (nx > 0) ani = MARIO_ANI_BIG_STOP_RIGHT;
			else ani = MARIO_ANI_BIG_STOP_LEFT;
		}
		else if (isRun) {
			if (nx > 0) ani = MARIO_ANI_BIG_RUN_RIGHT;
			else ani = MARIO_ANI_BIG_RUN_LEFT;
		}
		else if (isFalling) {
			if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
			else ani = MARIO_ANI_BIG_FALLING_LEFT;
		}
		else {
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
	}

	DebugOut(L"current ani: %d \n", ani);

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);


	RenderBoundingBox();
}


void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_DIE:
		Die();
		break;

	case MARIO_STATE_IDLE:
		if (isJump)
			return;
		isWalking = false;
		vx = 0;
		break;

	case MARIO_STATE_WALKING_RIGHT:
		if (isAttack)
			return;
		else if (isSitting) {
			nx = 1;
			return;
		}
		isWalking = true;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		if (isAttack)
			return;
		else if (isSitting) {
			nx = -1;
			return;
		}
		isWalking = true;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;

	case MARIO_STATE_JUMP:
		Jump();
		break;

	case MARIO_STATE_SIT:
		if (isSitting||isAttack) //dang ngoi -> return -> tranh tang y -> mario rot
			return;
		isSitting = true;
		isWalking = false;
		vx = 0;
		y = y + 5;
	/*	isSitting = true;
		if (vx > 0.08)
		{
			if (vx > 0) {
				vx -= 0.00054 * dt;
				if (vx < 0)
					vx = 0;
			}
			else if (vx < 0) {
				vx += 0.00054 * dt;
				if (vx > 0)
					vx = 0;
			}
		}
		else
		{
			if (vx > 0) {
				vx -= 0.00018 * dt;
				if (vx < 0)
					vx = 0;
			}
			else if (vx < 0) {
				vx += 0.00018 * dt;
				if (vx > 0)
					vx = 0;
			}
		}*/
		break;
	case MARIO_STATE_STAND_UP: // de tranh TH mario rot xuong(do chan dinh vien gach)
		y = y - 10;
		isSitting = false;
		vx = 0;
		break;

	case MARIO_STATE_STOP:
		isWalking = false;
		isRun = false;
		isStop = true;
		vx = 0;
		break;
	case MARIO_STATE_FALL:
		isFalling = true;
		break;
	case MARIO_STATE_RUN_RIGHT:
		vx = MARIO_RUN_SPEED;
		nx = 1;
		isRun = true;
		break;
	case MARIO_STATE_RUN_LEFT:
		vx = -MARIO_RUN_SPEED;
		nx = -1;
		isRun = true;
		break;

	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	
	if (GetLevel() == MARIO_LEVEL_SMALL){
		if (isSitting) {
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT - 5;
		}
		else {
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
	}
	else if (GetLevel()==MARIO_LEVEL_BIG)
	{
		if (isSitting){
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT - 10;
		}
		else {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		
	}
}

void CMario::Die() {
	switch (level) {
	case MARIO_LEVEL_FIRE:
		GetLevel() == MARIO_LEVEL_RACOON;
		StartUntouchable(); // set time untouchable

		break;
	case MARIO_LEVEL_RACOON:
		GetLevel() == MARIO_LEVEL_BIG;
		StartUntouchable(); // set time untouchable
		break;
	case MARIO_LEVEL_BIG:
		GetLevel() == MARIO_LEVEL_SMALL;
		StartUntouchable(); // set time untouchable
		break;
	case MARIO_LEVEL_SMALL:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::Jump() {
	// TH1: JUMP LOW 
	if (CGame::GetInstance()->IsKeyDown(DIK_S) || CGame::GetInstance()->IsKeyDown(DIK_X)  && isOnTheGround) {
		isJump = true;
		vy = -MARIO_JUMP_SPEED_Y * 0.75;
		y = y - 5;
	}

	// TH2: JUMP HIGHER (Nhan giu phim S)  
	else if (CGame::GetInstance()->IsKeyDown(DIK_S) && !isOnTheGround) {
		if (canFly) {

		}
		if (vy == -MARIO_JUMP_SPEED_Y) // max high can jump
		{
			isFalling = true;
			return;
		}
		else {
			vy = -MARIO_JUMP_SPEED_Y;
			y = y - 5;
		}

	}

}
