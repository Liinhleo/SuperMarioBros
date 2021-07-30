#include "Koopas.h"
#include "Ground.h"
#include "Brick.h"
#include "Enemy.h"
#include "Utils.h"

#define DISTANCE_START_X		5.0f // kc 2 dau tru bot de rua k rot

CKoopas::CKoopas() {
	this->type = ObjectType::KOOPA;
	SetState(KOOPAS_STATE_SHELL_IDLE);
}

CKoopas::CKoopas(int KoopaType, bool isWing, float x, float y)
{
	this->type = ObjectType::KOOPA;
	this->KoopaType = KoopaType;
	
	this->start_x = x;
	this->start_y = y;

	if (KoopaType == KOOPAS_TYPE_RED) {
		SetState(KOOPAS_STATE_WALKING);

	}
	else {
		if (isWing==true) {
			SetState(KOOPAS_STATE_FLYING);
		}
		else{
			SetState(KOOPAS_STATE_WALKING);
		}
	}
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (GetState() == KOOPAS_STATE_SHAKING && idleTimer->IsTimeUp())
	{
		idleTimer->Stop();
		isRelife = false;
		SetState(KOOPAS_STATE_WALKING);

		if (isBeingHeld)
			isBeingHeld = false;
	}

	if (startRelifeTimer->IsTimeUp() && !idleTimer->IsTimeUp())
	{
		startRelifeTimer->Stop();
		isRelife = true;
		SetState(KOOPAS_STATE_SHAKING);
	}

	if (GetState() == KOOPAS_STATE_SHELL_RUNNING) {
		idleTimer->Stop();
		startRelifeTimer->Stop();
		isRelife = false;
		if (this->isOutOfCam())
			SetState(STATE_DESTROYED);
	}


#pragma region NOT IS BEING HELD

	if (!isBeingHeld) {

#pragma region XU LY RED KOOPA WALKING TREN LIST GACH -> START_X, END_X

		vector<LPGAMEOBJECT> glassBricks; // list chua gach -> xet collide 

		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetType() == ObjectType::BRICK)
			{
				CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
				if (brick->GetBrickType() == BRICK_GLASS && brick->GetState() != BRICK_STATE_HIDDEN)
					glassBricks.push_back(coObjects->at(i));
			}
		}

		// RED KOOPA STAND ON GLASS BRICK 
		if (collideGround && this->KoopaType == KOOPAS_TYPE_RED
			&& GetState() != KOOPAS_STATE_SHELL_RUNNING) {

			float kl, kt, kr, kb, gl, gt, gr, gb;
			collideGround->GetBoundingBox(gl, gt, gr, gb);
			GetBoundingBox(kl, kt, kr, kb);

			if (collideGround->GetType() == ObjectType::BRICK) {
				CBrick* brick = dynamic_cast<CBrick*>(collideGround);
				if (brick->GetBrickType() == BRICK_GLASS
					&& brick->GetState() != BRICK_STATE_HIDDEN) {

					// DUYET GLASS BRICK 
					for (int i = 0; i < glassBricks.size(); i++) {
						
						if (collideGround == glassBricks[i]) {

							bool isCheck = false;

							// KIEM TRA GACH CANH NHAU
							for (int j = 0; j < glassBricks.size(); j++) {

								float j_l, j_t, j_r, j_b;
								glassBricks.at(j)->GetBoundingBox(j_l, j_t, j_r, j_b);

								if (i != j && CheckAABB(j_l, j_t, j_r, j_b, gl - 1.0f, gt, gr + 1.0f, gb)) {
									isCheck = true;

									if (glassBricks.at(j)->start_x < collideGround->start_x) {
										if (this->start.x < 0) {
											this->start_x = glassBricks.at(j)->start_x - DISTANCE_START_X;
										}

										else if (this->start.x > glassBricks.at(j)->start_x - DISTANCE_START_X) {
											this->start_x = glassBricks.at(j)->start_x - DISTANCE_START_X;
										}

										if (this->end.x < (gr - DISTANCE_START_X) || end.x < 0) {
											this->end.x = gr - DISTANCE_START_X;
										}
									}
									else if (glassBricks.at(j)->start_x > collideGround->start_x) {
										if (this->end.x < 0) {
											this->end.x = glassBricks.at(j)->start_x + BRICK_BBOX_SIZE - DISTANCE_START_X;
										}
										else if (this->end.x < glassBricks.at(j)->start_x + BRICK_BBOX_SIZE - DISTANCE_START_X) {
											this->end.x = glassBricks.at(j)->start_x + BRICK_BBOX_SIZE - DISTANCE_START_X;
										}
										if (this->start.x > (gl - DISTANCE_START_X) || start.x < 0)
										{
											this->start.x = gl - DISTANCE_START_X;
										}
									}
								/*	DebugOut(L"start===========: %f\n", start.x);
									DebugOut(L"end==========: %f\n", end.x);*/
								}
							}
							// TRUONG HOP GACH CHI CO 1 CUC DUY NHAT
							if (!isCheck) {
								this->start.x = gl - DISTANCE_START_X;
								this->start.y = this->end.y = gb;
								this->end.x = gr - DISTANCE_START_X;
								/*DebugOut(L"1 start===========: %f\n", start.x);
								DebugOut(L"1 end==========: %f\n", end.x);*/
							}
						}
					}
				}
			}

			else { // color box, ground
				this->start.x = gl - DISTANCE_START_X;
				this->end.x = gr - DISTANCE_START_X;
			}
			// XET DOI HUONG CHO KOOPA NEU DI QUA START_X, END_X
			if (kl < this->start.x || kl > this->end.x) {
				if (kl < this->start.x)
					this->x = gl - DISTANCE_START_X +  1.0f;
				else
					this->x = gr - DISTANCE_START_X - 1.5f;
				this->vx = -vx;
			}
		}
#pragma endregion


#pragma region COLLISION SOLVING

		if (this->GetState() == KOOPAS_STATE_SHELL_RUNNING)
			this->vy += MARIO_GRAVITY * 10 * dt;
		else {
			this->vy += MARIO_GRAVITY * dt;
		}

		// XU LY VA CHAM
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

			LPGAMEOBJECT objectX = NULL;
			LPGAMEOBJECT objectY = NULL;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy, objectX, objectY);

			y += min_ty * dy + ny * 0.4f;
			x += min_tx * dx + nx * 0.25f;

			if (ny != 0)
			{
				vy = 0;
				if (ny < 0) // va cham ground
				{
					collideGround = objectY;
					if (isWing) {
						vy = -KOOPA_JUMP_SPEED;
					}
				}
				// ktra co bi attack by tail again?
			}
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				//=============== GROUND========================
				if (e->obj->GetType() == ObjectType::GROUND) {
					CGround* ground = dynamic_cast<CGround*>(e->obj);
					if (e->nx != 0) { // va cham theo phuong x voi color box
						if (ground->isInteract) {
							x += dx; //di xuyen qua
						}
						else {
							vx = -vx; // doi huong
						}
					}
				}

				//=============== BRICK========================
				else if (e->obj->GetType() == ObjectType::BRICK) {

					CBrick* brick = dynamic_cast<CBrick*>(e->obj);

					if (e->nx != 0 || e->ny != 0) {

						// XU LY GLASS BRICK WHEN STATE HIDDEN (thanh COIN)
						if (brick->GetBrickType() == BRICK_GLASS 
							&& brick->GetState() == BRICK_STATE_HIDDEN) {
							x += dx;
							y += dy;
						}
					}

					// CASE KOOPA BI ROT XUONG GACH BEN DUOI (VA CHAM NX CUA GACH KHAC)
					if (e->nx != 0 && brick->start_y < (y + KOOPAS_BBOX_HEIGHT)) {
						vx = -vx; 
						if (state == KOOPAS_STATE_SHELL_RUNNING
							&& brick->GetBrickType() == BRICK_GLASS
							&& brick->GetState() != BRICK_STATE_HIDDEN)
						{
							brick->SetState(BRICK_STATE_BROKEN);
						}
						else if (state == KOOPAS_STATE_SHELL_RUNNING
							&& brick->GetBrickType() == BRICK_MUSIC
							&& brick->GetState() != BRICK_STATE_HIDDEN) {
							brick->SetState(BRICK_STATE_BOUNDING);
							this->vx = -this->vx; //doi chieu
							this->y += 5;
						}
						else if (brick->GetBrickType() == BRICK_QUESTION) {
							brick->SetBrickType(BRICK_BROKEN);
						}
					}
				}
				//=============== GOOMBA========================
				else if (e->obj->GetType() == ObjectType::GOOMBA) {
					if (e->nx != 0) {
						x += dx; //di xuyen qua
						y += dy;
					}
				}
				//=============== PIPE========================
				else if (e->obj->GetType() == ObjectType::PIPE) {
					if (e->nx != 0) {
						x += dx;
						y += dy;
					}
				}
				//=============== KOOPA========================
				else if (e->obj->GetType() == ObjectType::KOOPA 
					&& e->obj->GetState() == KOOPAS_STATE_WALKING 
					&& state == KOOPAS_STATE_WALKING) {
					
					CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
					
					if (e->nx != 0) {
						this->vx = -this->vx;
						koopa->vx = -vx;
					}
				}
				else if (dynamic_cast<Enemy*>(e->obj) && this->GetState() == KOOPAS_STATE_SHELL_RUNNING) {
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj); // flower
					enemy->nx = this->nx;
					enemy->damageByWeapon();
				}

			}
		}
#pragma endregion
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
#pragma endregion

#pragma region IS BEING HELD BY MARIO 
	else {

		vy = 0;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// block every object first!
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;///chinh lai xet va cham koopas voi brick -> cai nay do no xet y trung hop r

			//if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;

			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Enemy*>(e->obj) && state == KOOPAS_STATE_SHELL_IDLE)
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
					enemy->nx = -nx;
					enemy->damageByWeapon();

					this->nx = -nx;
					this->damageByWeapon();
				}
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
#pragma endregion

}


void CKoopas::Render()
{
	int KType = GetKoopaType();

	if (state == STATE_DESTROYED)
		return;

	if (GetKoopaType() == KOOPAS_TYPE_RED) {
		switch (state) {
		case KOOPAS_STATE_SHELL_IDLE: // mario jump on
			ani = KOOPAS_ANI_RED_SHELL_DOWN_IDLE;
			break;

		case ENEMY_STATE_DIE_BY_ATTACK:
			ani = KOOPAS_ANI_RED_SHELL_UP_IDLE; // bi lat nguoc
			break;

		case KOOPAS_STATE_SHELL_RUNNING:
			if (isShellUp) ani = KOOPAS_ANI_RED_SHELL_UP_RUN;
			else ani = KOOPAS_ANI_RED_SHELL_DOWN_RUN;
			break;

		case KOOPAS_STATE_SHAKING:
			if (isShellUp) ani = KOOPAS_ANI_RED_SHELL_UP_SHAKING;
			else ani = KOOPAS_ANI_RED_SHELL_DOWN_SHAKING;
			break;

		case KOOPAS_STATE_WALKING:
			if (vx > 0) ani = KOOPAS_ANI_RED_WALKING_RIGHT;
			else ani = KOOPAS_ANI_RED_WALKING_LEFT;
			break;

		}
	}
	else {
		switch (state) {
		
		case KOOPAS_STATE_SHELL_IDLE: // mario jump on
			ani = KOOPAS_ANI_GREEN_SHELL_DOWN_IDLE;
			break;

		case ENEMY_STATE_DIE_BY_ATTACK:
			ani = KOOPAS_ANI_GREEN_SHELL_UP_IDLE; // bi lat nguoc
			break;
		case KOOPAS_STATE_FLYING:
			if (vx > 0) ani = KOOPAS_ANI_GREEN_FLYING_RIGHT;
			else ani = KOOPAS_ANI_GREEN_FLYING_LEFT;
			break;

		case KOOPAS_STATE_SHELL_RUNNING:
			if (isShellUp) ani = KOOPAS_ANI_GREEN_SHELL_UP_RUN;
			else ani = KOOPAS_ANI_GREEN_SHELL_DOWN_RUN;
			break;

		case KOOPAS_STATE_SHAKING:
			if (isShellUp) ani = KOOPAS_ANI_GREEN_SHELL_UP_SHAKING;
			else ani = KOOPAS_ANI_GREEN_SHELL_DOWN_SHAKING;
			break;

		case KOOPAS_STATE_WALKING:
			if (vx > 0) ani = KOOPAS_ANI_GREEN_WALKING_RIGHT;
			else ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
			break;

		}
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//// xu ly state bi thuong cua Koopa
	//case STATE_DESTROYED: // bullet
	//	vx = 0;
	//	vy = 0;
	//	break;

	case ENEMY_STATE_DIE_BY_ATTACK: // tail
		isShellUp = true;
		vy = -KOOPA_JUMP_SPEED;
		if (nx > 0) {
			vx = KOOPAS_WALKING_SPEED;
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	// xu ly cac state khac
	case KOOPAS_STATE_FLYING:
		isWing = true;
		vy = -KOOPAS_WALKING_SPEED;
		vx = -0.008f;
		break;
	case KOOPAS_STATE_WALKING:
		isWing = false;
		vx = -0.03f;
		break;
	case KOOPAS_STATE_SHELL_IDLE:
		//DebugOut(L"VO DAY NE");
		idleTimer->Start(); // bd tinh gio dung yen
		startRelifeTimer->Start();
		vy = 0;
		vx = 0;
		break;
	case KOOPAS_STATE_SHELL_RUNNING:
		vx = -nx * KOOPA_RUNNING_SPEED;

		break;
	case KOOPAS_STATE_SHAKING:
		vx = 0;
		vy = 0;
		isOnGround = true;
		break;

	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + KOOPAS_BBOX_WIDTH;
	top = y;
	bottom = y + KOOPAS_BBOX_HEIGHT;

	if (state == KOOPAS_STATE_SHELL_IDLE 
		|| state == KOOPAS_STATE_SHELL_RUNNING 
		|| state == KOOPAS_STATE_SHAKING){

		top = y + KOOPAS_BBOX_HEIGHT;
	}
}
