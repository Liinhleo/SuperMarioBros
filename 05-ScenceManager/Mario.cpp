#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Ground.h"
#include "Brick.h"
#include "Pswitch.h"
#include "Item.h"
#include "Pipe.h"
#include "CoinEffect.h"
#include "ScoreEffect.h"

CMario::CMario(float x, float y) : CGameObject()
{
	this->level = MARIO_LEVEL_SMALL;
	this->untouchable = 0;

	SetState(MARIO_STATE_IDLE);

	this->start_x = x;
	this->start_y = y;

	this->x = x;
	this->y = y;

	this->a = 0;

	tail = new MarioTail(x, y, nx);
}


void CMario::UpdateSpeed(DWORD dt) {
	//DebugOut(L"cur a: %f \n", a);

	if (a == 0) {
		vx = vx;
	}
	else {
		vx += a * dt; // bien doi van toc
		if (abs(vx) >= MARIO_MAX_SPEED) {
			this->vx = nx *MARIO_MAX_SPEED;
			//DebugOut(L"max speed max speed %f \n", vx);
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector <LPGAMEOBJECT>* coItem, vector<LPGAMEOBJECT>* listEffect) {

	UpdateSpeed(dt);

	// Calculate dx, dy 
	CGameObject::Update(dt);
	this->vy += MARIO_GRAVITY * dt;// Simple fall down

	if (flyTime->IsTimeUp()) {
		flyTime->Stop();
	}

	//DebugOut(L"current state: %d \n", state);
	if (isAttack) {
		if (level == MARIO_LEVEL_FIRE) {
			// Tao bullet
			if (listBullet.size() < 2) {
				if (nx > 0)
					listBullet.push_back(CreateBullet(x + 6, y + 6, nx));
				else
					listBullet.push_back(CreateBullet(x - 6, y + 6, nx));
			}
		}
		if (level == MARIO_LEVEL_RACOON) {
			tail->SetState(TAIL_STATE_HIT);
			tail->SetPosition(x, y);
			tail->nx;
			tail->Update(dt, coObjects);
		}
		isAttack = false;
	}

	//update listBullet
	for (size_t i = 0; i < listBullet.size(); i++)
	{
		listBullet[i]->Update(dt, coObjects);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) {
		this->untouchable_start = 0;
		this->untouchable = 0;
	}

	// falling down
	if (isOnGround && collideGround) {
		float ml, mt, mr, mb, gl, gt, gr, gb;
		collideGround->GetBoundingBox(gl, gt, gr, gb);
		GetBoundingBox(ml, mt, mr, mb);
		if (mr < gl || ml > gr)
		{
			isOnGround = false;
			SetState(MARIO_STATE_FALL);
		}
	}
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size()==0){
		// khong bi can boi object nao -> thoai mai di chuyen
		x += dx; 
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		LPGAMEOBJECT objectX = NULL; // lay doi tuong co kha nang va cham vs mario theo chieu x
		LPGAMEOBJECT objectY = NULL; // lay doi tuong co kha nang va cham vs mario theo phuong y

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy, objectX, objectY);

		// block every object first!
		x += min_tx * dx + nx * 0.1f; // nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0; //va cham theo phuong x
		//if (ny!=0) vy = 0; //va cham theo truc y

		if (ny == -1)// mario jump on top: mario dap len object
		{
			vy = 0;
			isOnGround = true;
			collideGround = objectY;
			//DebugOut(L"on ground NY=: %d \n", ny);
		}


#pragma region COLLIDE WITH OBJECT
		for (int i = 0; i < coObjects->size(); i++)
		{
			switch (coObjects->at(i)->GetType()) {
			case ObjectType::GROUND:
			case ObjectType::PIPE:
			case ObjectType::BRICK:
				break;
			case ObjectType::PIRANHA_FLOWER:
			case ObjectType::FIRE_FLOWER:
			case ObjectType::FIRE_BALL:
				if (untouchable == 0 && GetState() != MARIO_STATE_DIE) {
					if (this->IsCollidingWithObject(coObjects->at(i))) {
						this->isDamaged(); // xu ly mario bi thuong
					}
				}
				break;

			case ObjectType::GOOMBA:
				if (untouchable == 0 && GetState() != MARIO_STATE_DIE) {
					if (this->IsCollidingWithObjectNy_1(coObjects->at(i))) {
						if (coObjects->at(i)->GetState() != STATE_DESTROYED
							&& coObjects->at(i)->GetState() != ENEMY_STATE_DAMAGE
							&& coObjects->at(i)->GetState() != ENEMY_STATE_DIE_BY_ATTACK)
						{
							CGoomba* goomba = dynamic_cast<CGoomba*>(coObjects->at(i));
							goomba->damageOnTop();
							this->vy = -MARIO_JUMP_DEFLECT_SPEED;

							// them effect
							this->AddScore(100);
							float g_x, g_y;
							goomba->GetPosition(g_x, g_y);
							CGameObject* effect = new ScoreEffect({ g_x, g_y }, 100);
							listEffect->push_back(effect);
						}
					}
					else if (isAABB(coObjects->at(i))) {
						this->isDamaged(); // xu ly mario bi thuong
					}
				}
				break;
				case ObjectType::KOOPA:
					if (untouchable == 0 && GetState() != MARIO_STATE_DIE) {
						if (this->IsCollidingWithObjectNy_1(coObjects->at(i))) { // jump on top
							CKoopas* koopa = dynamic_cast<CKoopas*>(coObjects->at(i));

							if (koopa->GetState() != KOOPAS_STATE_SHELL_IDLE) {
								koopa->damageOnTop();
								this->vy = -MARIO_JUMP_DEFLECT_SPEED;

								// them effect
								this->AddScore(100);
								float k_x, k_y;
								koopa->GetPosition(k_x, k_y);
								CGameObject* effect = new ScoreEffect({ k_x, k_y }, 100);
								listEffect->push_back(effect);

							}
							else{ // TH : KOOPA LA SHELL IDLE
								this->GetPosition(x, y); // vi tri cua mario

								float k_x, k_y; 
								koopa->GetPosition(k_x, k_y); // vi tri cua shell
								if ((k_x - this->x) > 0)
									koopa->nx = -1;
								else
									koopa->nx = 1;
								koopa->SetState(KOOPAS_STATE_SHELL_RUNNING);
								this->vy = -MARIO_JUMP_DEFLECT_SPEED;

							}
						}
						// Va cham theo phuong ngang
						else if (this->IsCollidingWithObjectNx(coObjects->at(i))) {
							CKoopas* koopa = dynamic_cast<CKoopas*>(coObjects->at(i));
							if (koopa->GetState() != KOOPAS_STATE_SHELL_IDLE) {
								koopa->vx = -koopa->vx; // di xuyen qua
								this->isDamaged(); // xu ly mario bi thuong
							}
							else {
								if (this->canHolding) {
									this->shell = koopa;
									koopa->isBeingHeld = true;
								}
								else {
									koopa->nx = -this->nx;
									koopa->SetState(KOOPAS_STATE_SHELL_RUNNING);
								}
							}
						}
					}
					else if (isAABB(coObjects->at(i)) && !canHolding) {
						this->isDamaged(); // xu ly mario bi thuong
					}
					break;

			}
		}

#pragma endregion
	
		for (UINT i = 0; i < coEventsResult.size(); i++){
			LPCOLLISIONEVENT e = coEventsResult[i];



			//Mario can go through pipe if it has Portal
			if (e->ny != 0) { // collide from 2 direct (up/down)
				if (e->obj->GetType() == ObjectType::PIPE) {
					Pipe* pipe = dynamic_cast<Pipe*>(e->obj); // if e->obj is Pipe 
					if (pipe->IsHasPortal()) {
						if (pipe->GetDirection()&& this->canGoThroughPipe_up) {

							this->isInHiddenMap = !this->isInHiddenMap;
							this->SetPosition(pipe->GetDestination().x, pipe->GetDestination().y);
						}
						else if (!pipe->GetDirection() && this->canGoThroughPipe_down) {
							
							this->isInHiddenMap = !this->isInHiddenMap;
							this->SetPosition(pipe->GetDestination().x, pipe->GetDestination().y);
						}
					}
				}
			}


			if (e->ny > 0) { // mario's head collide brick 
				// BRICK
				if (e->obj->GetType() == ObjectType::BRICK) {
					vy = 0;
					CBrick* brick = dynamic_cast<CBrick*>(e->obj); // if e->obj is Brick 
					if (brick->GetBrickType()!= BRICK_BROKEN) {
						if (brick->GetBrickType() == BRICK_GLASS 
							|| brick->GetBrickType() == BRICK_QUESTION) {
							if (!brick->isBroken) {

								switch (brick->typeItem) {
								case CONTAIN_NONE:
									{
										brick->SetState(BRICK_STATE_BOUNDING);

										if (brick->GetCountItem() == 0)
											brick->SetState(BRICK_STATE_BROKEN);
										break;
									}
									
								case CONTAIN_ITEM_UP:
									{
										brick->SetState(BRICK_STATE_BOUNDING);
										AddScore(1000);
										AddCoin();
										if (brick->GetCountItem() > 0)
											brick->count--;
										if (brick->GetCountItem() == 0)
											brick->SetState(BRICK_STATE_BROKEN);
										break;
									}
									
								case CONTAIN_COIN: 
									{
										brick->SetState(BRICK_STATE_BOUNDING);
										AddScore(100);
										AddCoin();
										float b_x, b_y;
										brick->GetPosition(b_x, b_y); // vi tri cua brick
										CGameObject* effect = new CoinEffect({ b_x,b_y });
										listEffect->push_back(effect);

										if (brick->GetCountItem() > 0)
											brick->count--;
										if (brick->GetCountItem() == 0)
											brick->SetState(BRICK_STATE_BROKEN);
										break;
									}
								case CONTAIN_PSWITCH:
									{
										CGameObject* pswitch = new Pswitch(brick->start_x, brick->start_y - 16);
										coObjects->push_back(pswitch);
										brick->SetBrickType(BRICK_BROKEN);
										brick->SetState(BRICK_STATE_BOUNDING);
										break;
									}
								}
							}
						}
					}

				}
				// GROUND
				if (e->obj->GetType() == ObjectType::GROUND) {
					CGround* ground = dynamic_cast<CGround*>(e->obj); // if e->obj is Ground 
					if (ground->isInteract) {
						x += dx;
						y += dy;
					}
				}
			}

			if (e->nx != 0) { //va cham theo phuong x
				if (e->obj->GetType() == ObjectType::GROUND) {
					CGround* ground = dynamic_cast<CGround*>(e->obj); // if e->obj is Ground 
					if (ground->isInteract) {
						x += dx; // duoc phep di xuyen
					}
					else {
						if (state == MARIO_STATE_RUN) { // dang chay va cham tuong thi di bo
							if (vx > 0)
								state = MARIO_STATE_WALKING_RIGHT;
							else
								state = MARIO_STATE_WALKING_LEFT;
						}
					}
				}
			}

#pragma region COLLISION WITH GOOMBA

			//if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0) { // xet mario va cham theo phuong y -> nhay len dau -> Goomba die
			//		if (goomba->isWing && goomba->GetState() != ENEMY_STATE_DAMAGE) {
			//			goomba->SetState(GOOMBA_STATE_WALKING);
			//		}
			//		if (goomba->GetState() != GOOMBA_STATE_WALKING && goomba->GetState() != ENEMY_STATE_DAMAGE){
			//			goomba->SetState(ENEMY_STATE_DAMAGE);
			//			vy = -MARIO_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0) {	// xet mario va cham theo phuong x -> mario die
			//		if (untouchable == 0){
			//			if (goomba->GetState() != ENEMY_STATE_DAMAGE){
			//				isDamaged(); // xu ly mario bi thuong
			//			}
			//		}
			//	}
			//} // if Goomba

#pragma endregion


#pragma region COLLISION WITH PORTAL
			/*if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}*/
#pragma endregion

		}
	}

	// han che lap frame attack
	if (level == MARIO_LEVEL_RACOON && isAttack 
		&& animation_set->at(ani)->getCurrentFrame() >= 4) {
		isAttack = false;
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}

void CMario::CollideWithItem(vector<LPGAMEOBJECT>* coItem) {
	for (UINT i = 0; i < coItem->size(); i++) {
		if (isAABB(coItem->at(i))) {
			if (coItem->at(i)->GetType() == ObjectType::BRICK)//brick khi chuyen thanh tien
			{
				AddScore(100);
				AddCoin();
			}
			if (coItem->at(i)->GetType() == ObjectType::COIN)//brick khi chuyen thanh tien
			{
				AddScore(100);
				AddCoin();
			}
			else {
				Item* item = dynamic_cast<Item*>(coItem->at(i));
				switch (item->GetItemType())
				{
				case ITEM_GREEN_MUSHROOM: 
					if(GetLevel() == MARIO_LEVEL_SMALL)
					break;
				case ITEM_SUPERLEAF:
					this->SetPosition(x, y - 1.0f);
					this->SetLevel(MARIO_LEVEL_RACOON);
					break;
				case ITEM_RED_MUSHROOM:
					this->SetPosition(x, y - 12.0f);
					this->SetLevel(MARIO_LEVEL_BIG);
					break;
				}
				AddScore(1000);
			}
			coItem->at(i)->SetState(STATE_DESTROYED);
		}
	}
}

void CMario::Render()
{
#pragma region SMALL MARIO
	if (GetLevel() == MARIO_LEVEL_SMALL) {
		switch (state) {
		case MARIO_STATE_DIE:
			ani = MARIO_ANI_DIE;
			break;
		case MARIO_STATE_STOP:
			if (nx > 0) ani = MARIO_ANI_SMALL_STOP_RIGHT;
			else ani = MARIO_ANI_SMALL_STOP_LEFT;
			break;
		case MARIO_STATE_JUMP_LOW:
		case MARIO_STATE_JUMP:
			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_SMALL_SIT_RIGHT;
				else ani = MARIO_ANI_SMALL_SIT_LEFT;
			}
			else {
				if (vy < 0) {
					if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
					else ani = MARIO_ANI_SMALL_JUMP_LEFT;
				}
				else {
					if (nx > 0) ani = MARIO_ANI_SMALL_FALLING_RIGHT;
					else ani = MARIO_ANI_SMALL_FALLING_LEFT;
				}
			}
			break;

		case MARIO_STATE_RUN:
			if (nx > 0) {
				if (vx >= MARIO_MAX_SPEED) ani = MARIO_ANI_SMALL_RUN_RIGHT;
				else ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			}
			else {
				if (abs(vx) >= MARIO_MAX_SPEED) ani = MARIO_ANI_SMALL_RUN_LEFT;
				else ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
			break;
		case MARIO_STATE_SIT:
			if (nx > 0) ani = MARIO_ANI_SMALL_SIT_RIGHT;
			else ani = MARIO_ANI_SMALL_SIT_LEFT;
			break;
		case MARIO_STATE_FALL:
			if (nx > 0) ani = MARIO_ANI_SMALL_FALLING_RIGHT;
			else ani = MARIO_ANI_SMALL_FALLING_LEFT;
			break;

		default:
			if (!isOnGround) {
				if (nx > 0) ani = MARIO_ANI_SMALL_FALLING_RIGHT;
				else ani = MARIO_ANI_SMALL_FALLING_LEFT;
			}
			else {
				if (nx > 0) {
					if (vx > 0) ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				}
				else {
					if (vx < 0) ani = MARIO_ANI_SMALL_WALKING_LEFT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
			}
			break;
		}
	}
#pragma endregion

#pragma region BIG MARIO
	if (GetLevel() == MARIO_LEVEL_BIG) {
		switch (state) {
		case MARIO_STATE_STOP:
			if (nx > 0) ani = MARIO_ANI_BIG_STOP_LEFT;
			else ani = MARIO_ANI_BIG_STOP_RIGHT;
			break;
		case MARIO_STATE_JUMP_LOW:
		case MARIO_STATE_JUMP:
			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
				else ani = MARIO_ANI_BIG_SIT_LEFT;
			}
			else {
				if (vy < 0) {
					if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
					else ani = MARIO_ANI_BIG_JUMP_LEFT;
				}
				else {
					if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
					else ani = MARIO_ANI_BIG_FALLING_LEFT;
				}
			}
			break;

		case MARIO_STATE_RUN:
			if (abs(vx) >= MARIO_MAX_SPEED) {
				if (nx > 0) ani = MARIO_ANI_BIG_RUN_RIGHT;
				else ani = MARIO_ANI_BIG_RUN_LEFT;
			}
			else {
				if (nx > 0) ani = MARIO_ANI_BIG_WALKING_RIGHT;
				else ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
			break;
		case MARIO_STATE_SIT:
			if (nx > 0) ani = MARIO_ANI_BIG_SIT_RIGHT;
			else ani = MARIO_ANI_BIG_SIT_LEFT;
			break;
		case MARIO_STATE_FLY:
			if (nx > 0) ani = MARIO_ANI_BIG_FLY_RIGHT;
			else ani = MARIO_ANI_BIG_FLY_LEFT;
			break;
		case MARIO_STATE_FALL:
			if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
			else ani = MARIO_ANI_BIG_FALLING_LEFT;
			break;
		default:
			if (!isOnGround) {
				if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
				else ani = MARIO_ANI_BIG_FALLING_LEFT;
			}
			else {
				if (nx > 0) {
					if (vx > 0) ani = MARIO_ANI_BIG_WALKING_RIGHT;
					else ani = MARIO_ANI_BIG_IDLE_RIGHT;
				}
				else {
					if (vx < 0) ani = MARIO_ANI_BIG_WALKING_LEFT;
					else ani = MARIO_ANI_BIG_IDLE_LEFT;
				}
			}
			break;
		}
	}
#pragma endregion

#pragma region RACOON MARIO
	if (GetLevel() == MARIO_LEVEL_RACOON) {
		switch (state) {
		case MARIO_STATE_ATTACK:
			if (nx > 0) ani = MARIO_ANI_RACOON_ATTACK_RIGHT;
			else ani = MARIO_ANI_RACOON_ATTACK_LEFT;
			break;
		case MARIO_STATE_STOP:
			if (nx > 0) ani = MARIO_ANI_RACOON_STOP_RIGHT;
			else ani = MARIO_ANI_RACOON_STOP_LEFT;
			break;
		case MARIO_STATE_JUMP_LOW:
		case MARIO_STATE_JUMP:
			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_RACOON_SIT_RIGHT;
				else ani = MARIO_ANI_RACOON_SIT_LEFT;
			}
			else {
				if (vy < 0) {
					if (nx > 0) ani = MARIO_ANI_RACOON_JUMP_RIGHT;
					else ani = MARIO_ANI_RACOON_JUMP_LEFT;
				}
				else {
					if (nx > 0) ani = MARIO_ANI_RACOON_FALLING_RIGHT;
					else ani = MARIO_ANI_RACOON_FALLING_LEFT;
				}
			}
			break;
		case MARIO_STATE_RUN:
			if (nx > 0) {
				if (vx >= MARIO_MAX_SPEED) ani = MARIO_ANI_RACOON_RUN_RIGHT;
				else ani = MARIO_ANI_RACOON_WALKING_RIGHT;
			}
			else {
				if (abs(vx) >= MARIO_MAX_SPEED) ani = MARIO_ANI_RACOON_RUN_LEFT;
				else ani = MARIO_ANI_RACOON_WALKING_LEFT;
			}
			break;
		case MARIO_STATE_SIT:
			if (nx > 0) ani = MARIO_ANI_RACOON_SIT_RIGHT;
			else ani = MARIO_ANI_RACOON_SIT_LEFT;
			break;
		case MARIO_STATE_FLY:
			if (nx > 0) ani = MARIO_ANI_RACOON_FLY_RIGHT;
			else ani = MARIO_ANI_RACOON_FLY_LEFT;
			break;
		case MARIO_STATE_FALL:
			if (nx > 0) ani = MARIO_ANI_RACOON_FALLING_RIGHT;
			else ani = MARIO_ANI_RACOON_FALLING_LEFT;
			break;
		default:
			if (!isOnGround) {
				if (nx > 0) ani = MARIO_ANI_RACOON_FALLING_RIGHT;
				else ani = MARIO_ANI_RACOON_FALLING_LEFT;
			}

			else {
				if (nx > 0) {
					if (vx > 0) ani = MARIO_ANI_RACOON_WALKING_RIGHT;
					else ani = MARIO_ANI_RACOON_IDLE_RIGHT;
				}
				else {
					if (vx < 0) ani = MARIO_ANI_RACOON_WALKING_LEFT;
					else ani = MARIO_ANI_RACOON_IDLE_LEFT;
				}
			}
			break;
		}
	}
#pragma endregion

#pragma region FIRE MARIO
	if (GetLevel() == MARIO_LEVEL_FIRE) {
		switch (state) {
		case MARIO_STATE_ATTACK:
			if (nx > 0) ani = MARIO_ANI_FIRE_ATTACK_RIGHT;
			else ani = MARIO_ANI_FIRE_ATTACK_LEFT;
			break;
		case MARIO_STATE_STOP:
			if (nx > 0) ani = MARIO_ANI_FIRE_STOP_RIGHT;
			else ani = MARIO_ANI_FIRE_STOP_LEFT;
			break;
		case MARIO_STATE_JUMP_LOW:
		case MARIO_STATE_JUMP:
			if (state == MARIO_STATE_SIT) {
				if (nx > 0) ani = MARIO_ANI_FIRE_SIT_RIGHT;
				else ani = MARIO_ANI_FIRE_SIT_LEFT;
			}
			else {
				if (vy < 0) {
					if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_RIGHT;
					else ani = MARIO_ANI_FIRE_JUMP_LEFT;
				}
				else {
					if (nx > 0) ani = MARIO_ANI_FIRE_FALLING_RIGHT;
					else ani = MARIO_ANI_FIRE_FALLING_LEFT;
				}
			}
			break;
		case MARIO_STATE_RUN:
			if (nx > 0) {
				if (vx >= MARIO_MAX_SPEED) ani = MARIO_ANI_FIRE_RUN_RIGHT;
				else ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			}
			else {
				if (abs(vx) >= MARIO_MAX_SPEED) ani = MARIO_ANI_FIRE_RUN_LEFT;
				else ani = MARIO_ANI_FIRE_WALKING_LEFT;
			}
			break;
		case MARIO_STATE_SIT:
			if (nx > 0) ani = MARIO_ANI_FIRE_SIT_RIGHT;
			else ani = MARIO_ANI_FIRE_SIT_LEFT;
			break;
		case MARIO_STATE_FLY:
			if (nx > 0) ani = MARIO_ANI_FIRE_FLY_RIGHT;
			else ani = MARIO_ANI_FIRE_FLY_LEFT;
			break;
		case MARIO_STATE_FALL:
			if (nx > 0) ani = MARIO_ANI_FIRE_FALLING_RIGHT;
			else ani = MARIO_ANI_FIRE_FALLING_LEFT;
			break;
		default:
			if (!isOnGround) {
				if (nx > 0) ani = MARIO_ANI_FIRE_FALLING_RIGHT;
				else ani = MARIO_ANI_FIRE_FALLING_LEFT;
			}

			else {
				if (nx > 0) {
					if (vx > 0) ani = MARIO_ANI_FIRE_WALKING_RIGHT;
					else ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				}
				else {
					if (vx < 0) ani = MARIO_ANI_FIRE_WALKING_LEFT;
					else ani = MARIO_ANI_FIRE_IDLE_LEFT;
				}
			}
			break;
		}
	}
#pragma endregion

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);

	for (size_t i = 0; i < listBullet.size(); i++)
		listBullet[i]->Render();
	
	if (tail) tail->Render();

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)	{
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (a != 0)
			return;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;

	case MARIO_STATE_WALKING_LEFT:
		if (a != 0)
			return;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;

	case MARIO_STATE_RUN:
		vx = nx * MARIO_WALKING_SPEED; // vx_bandau
		break;
	case MARIO_STATE_STOP:
		vx = 0;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_JUMP:
		isOnGround = false;
		y -= 5;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_JUMP_LOW:
		isOnGround = false;
		vy = -MARIO_JUMP_SPEED_Y * 0.75;
		y -= 5;
		break;
	case MARIO_STATE_SIT:
		if (state == MARIO_STATE_SIT) //dang ngoi -> return -> tranh tang y -> mario rot
			return;
		vx = 0;
		y = y + 10;
		break;
	case MARIO_STATE_STAND_UP: // de tranh TH mario rot xuong(do chan dinh vien gach)
		y = y - 10;
		vx = 0;
		break;
	case MARIO_STATE_FLY:
		isFlying = true;
		isOnGround = false;
		flyTime->Start(); // bd tinh gio bay
		if (nx > 0)
			vx = MARIO_WALKING_SPEED;
		else
			vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_JUMP_SPEED_Y * 0.75;
		//y -= 5;
		break;
	case MARIO_STATE_ATTACK:
		isAttack = true;
		break;
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
	case MARIO_LEVEL_RACOON:
		if (state == MARIO_STATE_SIT) { // sitting
			right = x + MARIO_RACCOON_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT - 10;
		}
		else { 
			if (nx > 0) {
				left = x + TAIL_BBOX_WIDTH - 2;
				right = left  + MARIO_RACCOON_BBOX_WIDTH;
			}
			else {
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		break;

	case MARIO_LEVEL_BIG:
	case MARIO_LEVEL_FIRE:
		left = x;

		if (state == MARIO_STATE_SIT) {
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT - 10;
		}
		else {
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		break;
	}
}

void CMario::isDamaged() {

	if (level != MARIO_LEVEL_SMALL) {
		if (level == MARIO_LEVEL_BIG)
			SetLevel(MARIO_LEVEL_SMALL);
		else if (level == MARIO_LEVEL_RACOON)
			SetLevel(MARIO_LEVEL_BIG);
		else if (level == MARIO_LEVEL_FIRE)
			SetLevel(MARIO_LEVEL_RACOON);

		Reset();
		StartUntouchable(); // set time untouchable
	}
	else
		SetState(MARIO_STATE_DIE);
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset() {
	SetState(MARIO_STATE_IDLE);
	GetLevel();
	SetPosition(x, y - MARIO_RACCOON_BBOX_WIDTH); //tang y tranh mario rot
	SetSpeed(0, 0);
}

void CMario::StartUntouchable() {
	this->untouchable = 1;
	this->untouchable_start = GetTickCount64();
}