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
	this->level = MARIO_LEVEL_BIG;
	this->untouchable = 0;

	SetState(MARIO_STATE_IDLE);

	this->start_x = x;
	this->start_y = y;

	this->x = x;
	this->y = y;
	this->a = 0;
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


void CMario::StartUntouchable() {
	this->untouchable = 1;
	this->untouchable_start = GetTickCount();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (a == 0) {
		vx = vx; // vx be define in SetState
	}
	else {
		vx = vx + nx * a * dt; // vx = vx + at -> chuyen dong nhanh dan deu 
		// nx l� huong chuyen dong
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);
	//DebugOut(L"current state: %d \n",state);

	// Simple fall down
	this->vy += MARIO_GRAVITY*dt;

	// Neu mario do not walk : vx = 0;
	DebugOut(L"current vy %f \n", vy);
	DebugOut(L"current vx: %f \n", vx);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);


	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		this->untouchable_start = 0;
		this->untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		// khong bi can boi object nao -> thoai mai di chuyen
		x += dx; 
		y += dy;		// xuyen qua ground
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
			//if (dynamic_cast<CGround*>(e->obj)) { // if e->obj is Ground 
			//	if (e->ny < 0) { // mario va cham ground
			//		isOnTheGround = true;
			//		vy = dy = 0;
			//	}
			//	else if (e->ny > 0) {
			//		y += dy;
			//		isFalling = true;
			//	}
			//}
#pragma endregion

#pragma region COLLISION WITH GOOMBA
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0) // xet mario va cham theo phuong y -> nhay len dau -> Goomba die
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)	// xet mario va cham theo phuong x -> mario die
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

			//else if (dynamic_cast<CBrick*>(e->obj))
			//{
			//	DebugOut(L"Brick");
			//}
			//else if (dynamic_cast<CGround*>(e->obj))
			//{
			//	DebugOut(L"CGround");
			//}
#pragma endregion

		}
		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CMario::Render()
{
	int ani = -1;

	switch (level) {
	// ====================== SMALL MARIO ======================
	case MARIO_LEVEL_SMALL:
		if (state == MARIO_STATE_DIE) {
			ani = MARIO_ANI_DIE;
		}
		// STOP
		else if (state==MARIO_STATE_STOP) {
			if (nx > 0) ani = MARIO_ANI_SMALL_STOP_RIGHT;
			else ani = MARIO_ANI_SMALL_STOP_LEFT;
		}
		// JUMP
		else if (state == MARIO_STATE_JUMP) {
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
		// RUN 
		else if (state == MARIO_STATE_RUN) {
			if (nx > 0) ani = MARIO_ANI_SMALL_RUN_RIGHT;
			else ani = MARIO_ANI_SMALL_RUN_LEFT;
		}
		// IDLE
		else if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		//WALKING
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;

		break;

	// ====================== BIG MARIO ======================
	case MARIO_LEVEL_BIG:
		// SITTING
		if (state == MARIO_STATE_SIT) {
			if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
			else ani = MARIO_ANI_BIG_SIT_LEFT;
		}
		// FALLING 
		if (state == MARIO_STATE_FALL) {
			if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
			else ani = MARIO_ANI_BIG_FALLING_LEFT;
		}
		// STOP
		if (state == MARIO_STATE_STOP) {
			if (nx > 0) ani = MARIO_ANI_BIG_STOP_RIGHT;
			else ani = MARIO_ANI_BIG_STOP_LEFT;
		}
		// JUMP
		else if (state == MARIO_STATE_JUMP) {
			if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
			else ani = MARIO_ANI_BIG_JUMP_LEFT;
		}
		// RUN 
		else if (state == MARIO_STATE_RUN) {
			if (nx > 0) ani = MARIO_ANI_BIG_RUN_RIGHT;
			else ani = MARIO_ANI_BIG_RUN_LEFT;
		}
		// IDLE
		else if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		//WALKING
		else if (vx > 0)
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		else ani = MARIO_ANI_BIG_WALKING_LEFT;
		break;	

	// ====================== RACOON MARIO ======================
	case MARIO_LEVEL_RACOON:
		// SITTING
		if (state == MARIO_STATE_SIT) {
			if (nx > 0) ani = MARIO_ANI_RACOON_SIT_RIGHT;
			else ani = MARIO_ANI_RACOON_SIT_LEFT;
		}
		// FALLING 
		if (state == MARIO_STATE_FALL) {
			if (nx > 0) ani = MARIO_ANI_RACOON_FALLING_RIGHT;
			else ani = MARIO_ANI_RACOON_FALLING_LEFT;
		}
		// STOP
		if (state == MARIO_STATE_STOP) {
			if (nx > 0) ani = MARIO_ANI_RACOON_STOP_RIGHT;
			else ani = MARIO_ANI_RACOON_STOP_LEFT;
		}
		// JUMP
		else if (state == MARIO_STATE_JUMP) {
			if (nx > 0) ani = MARIO_ANI_RACOON_JUMP_RIGHT;
			else ani = MARIO_ANI_RACOON_JUMP_LEFT;
		}
		// RUN 
		else if (state == MARIO_STATE_RUN) {
			if (nx > 0) ani = MARIO_ANI_RACOON_RUN_RIGHT;
			else ani = MARIO_ANI_RACOON_RUN_LEFT;
		}
		// IDLE
		else if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_RACOON_IDLE_RIGHT;
			else ani = MARIO_ANI_RACOON_IDLE_LEFT;
		}
		//WALKING
		else if (vx > 0)
			ani = MARIO_ANI_RACOON_WALKING_RIGHT;
		else ani = MARIO_ANI_RACOON_WALKING_LEFT;
			break;
	}
	//DebugOut(L"current ani: %d \n", ani);

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
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;


	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;


	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;
		break;


	//case MARIO_STATE_SIT:
	//	//if (isSitting||isAttack) //dang ngoi -> return -> tranh tang y -> mario rot
	//	//	return;
	//	vx = 0;
	//	y = y + 5;
	//	break;
	//case MARIO_STATE_STAND_UP: // de tranh TH mario rot xuong(do chan dinh vien gach)
	//	y = y - 10;
	//	vx = 0;
	//	break;

	//case MARIO_STATE_SIT:
	//	if (isSitting||isAttack) //dang ngoi -> return -> tranh tang y -> mario rot
	//		return;
	//	isSitting = true;
	//	isWalking = false;
	//	vx = 0;
	//	y = y + 5;
	//	isSitting = true;
	//	if (vx > 0.08)
	//	{
	//		if (vx > 0) {
	//			vx -= 0.00054 * dt;
	//			if (vx < 0)
	//				vx = 0;
	//		}
	//		else if (vx < 0) {
	//			vx += 0.00054 * dt;
	//			if (vx > 0)
	//				vx = 0;
	//		}
	//	}
	//	else
	//	{
	//		if (vx > 0) {
	//			vx -= 0.00018 * dt;
	//			if (vx < 0)
	//				vx = 0;
	//		}
	//		else if (vx < 0) {
	//			vx += 0.00018 * dt;
	//			if (vx > 0)
	//				vx = 0;
	//		}
	//	}*/
	//	break;
	//case MARIO_STATE_STAND_UP: // de tranh TH mario rot xuong(do chan dinh vien gach)
	//	y = y - 10;
	//	isSitting = false;
	//	vx = 0;
	//	break;

	//case MARIO_STATE_RUN:
	//	if (abs(vx) <= MARIO_MAX_SPEED) // neu toc do <= MAX_SPEED -> state walking nhanh bth
	//		return;
	//	vx = MARIO_MAX_SPEED;
	//	break;


	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	
	switch (level) {
	case MARIO_LEVEL_SMALL:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		break;

	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_RACOON:
		if (state == MARIO_STATE_SIT) {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT - 10;
		}
		else {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		break;
	}
}


//void CMario::isDamaged() {
//	if (untouchable_start > 0)
//		return;
//	if (!level == MARIO_LEVEL_SMALL) {
//		if (level == MARIO_LEVEL_BIG) {
//			GetLevel() == MARIO_LEVEL_SMALL;
//		}
//		else if (level == MARIO_LEVEL_RACOON) {
//			GetLevel() == MARIO_LEVEL_BIG;
//		}
//		StartUntouchable(); // set time untouchable
//		state = MARIO_STATE_IDLE;
//		vx = 0;
//		untouchable_start = GetTickCount();
//	}
//	else {
//		vy = -MARIO_DIE_DEFLECT_SPEED;
//		isOnTheGround = false;
//		state = MARIO_STATE_DIE;
//	}
//}
//
////void CMario::Walk() {
////	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT) || CGame::GetInstance()->IsKeyDown(DIK_LEFT)) {
////		isWalking = true; 
////		canSpeedUp = false;
////		isFalling = false;
////		if (nx > 0) {
////			vx = MARIO_WALKING_SPEED;
////
////		}
////		
////	}
////
////	//if (vx < 0) {
////	//	nx = 1;
////	//	vx = 0;
////	//	SetState(MARIO_STATE_STOP);
////	//}
////	//isFalling = false;
////	//isWalking = true;
////	//vx = MARIO_WALKING_SPEED;
////	//nx = 1;
////	//break;
////
////}
//
//
//void CMario::JumpX() {
//	if (isJump) // han che nhay lien tuc
//		return;
//	isJump = true;
//	isOnTheGround = false;
//	vy = -MARIO_JUMP_SPEED_Y * 0.75;
//	y = y - 5;
//}
//
//
// 
//void CMario::Jump() {
//	//// TH2: JUMP HIGHER (Nhan giu phim S)  
//	//if (CGame::GetInstance()->IsKeyDown(DIK_S)) {
//	//	if (isJump) // han che nhay lien tuc
//	//		return;
//	//	isJump = true;
//	//	vy = -MARIO_JUMP_SPEED_Y;
//	//	y = y - 5;
//	//	if (vy <= 0) {
//	//		isFalling = true;
//	//		isJump = false;
//	//	}
//	//}
//}
