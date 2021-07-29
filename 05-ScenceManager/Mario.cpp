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
#include "Portal.h"
#include "Ninja.h"

CMario::CMario(float x, float y) : CGameObject()
{
	this->type = ObjectType::MARIO;
	this->score = 100;
	this->coin = 0;
	this->level = 1;
	
	this->SetState(MARIO_STATE_IDLE);
	this->level = MARIO_LEVEL_SMALL;
	this->untouchable = 0;

	this->start_x = x;
	this->start_y = y;

	this->x = x;
	this->y = y;

	this->a = 0;

	tail = new MarioTail();
}


void CMario::UpdateSpeed(DWORD dt) {
	//DebugOut(L"cur a: %f \n", a);

	if (a == 0) {
		vx = vx;
	}
	else {
		vx += a * dt; // bien doi van toc
		if (abs(vx) >= MARIO_MAX_SPEED) {
			this->vx = nx * MARIO_MAX_SPEED;
			//DebugOut(L"max speed max speed %f \n", vx);
		}
	}
}
void CMario::DecreaseSpeed() {
	if (abs(vx) > MARIO_WALKING_SPEED) { //RUN
		if (vx > 0) { // right
			vx -= 0.00006f * dt; // giam toc

			if (vx < 0) {
				vx = 0;
			}
		}
		else if (vx < 0) { // left
			vx += 0.00006f * dt; // giam toc
			if (vx < 0) {
				vx = 0;
			}

		}
	}
	else {
		if (vx > 0) { // right
			vx -= 0.00003f * dt; // giam toc

			if (vx < 0) {
				vx = 0;
			}
		}
		else if (vx < 0) { // left
			vx += 0.00003f * dt; // giam toc
			if (vx < 0) {
				vx = 0;
			}

		}
	}
}

// GREENLAND 
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	

	UpdateSpeed(dt);

	CGameObject::Update(dt);
	x += dx;
	y += dy;


	for (int i = 0; i < coObjects->size(); i++) {
		if (coObjects->at(i)->GetType() == ObjectType::PORTAL) {
			CPortal* point = dynamic_cast<CPortal*>(coObjects->at(i));

			if (this->IsCollidingWithObjectNx(point) || this->IsCollidingWithObjectNy(point)) {
				SetState(MARIO_STATE_IDLE_GREENLAND);

				this->x = point->x;
				this->y = point->y;

				DebugOut(L"point->x = %d \n", point->x);
				DebugOut(L"point->y = %d \n", point->y);

				DebugOut(L"Mar_x = %d \n", x);
				DebugOut(L"Mar_y = %d \n", y);
			}
			if (this->isAABB(point)) {

				this->canWalkLeft = point->canMoveLeft;
				this->canWalkRight = point->canMoveRight;
				this->canWalkUp = point->canMoveUp;
				this->canWalkDown = point->canMoveDown;

				SetWorldMapPosition(point->start_x, point->start_y);

				if (canSwitchScene) {
					if (point->IsHasPortal())
						CGame::GetInstance()->SwitchScene(point->GetSceneID());
				
				}
			}
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector <LPGAMEOBJECT>* coItem, vector<LPGAMEOBJECT>* listEffect) {


	UpdateSpeed(dt);

	// Calculate dx, dy 
	CGameObject::Update(dt);
	this->vy += MARIO_GRAVITY * dt;// Simple fall down


	if (flyTime->IsTimeUp())
		flyTime->Stop();

	if (!isAttack && tail) {
		tail->SetState(STATE_DESTROYED);
	}

	if (level == MARIO_LEVEL_FIRE && isAttack) {
		// Tao bullet
		if (listBullet.size() < 2) {
			if (nx > 0)
				listBullet.push_back(CreateBullet(x + 6, y + 6, nx));
			else
				listBullet.push_back(CreateBullet(x - 6, y + 6, nx));
		}
		isAttack = false;
	}

	if (level == MARIO_LEVEL_RACOON) {
		if (isAttack && !attackStart->IsTimeUp()) {
			this->state = MARIO_STATE_ATTACK;
			tail->SetState(TAIL_STATE_HIT);
			// Xet lai huong cho tail khi o frame 2
			if (animation_set->at(ani)->getCurrentFrame() == 2) {
				this->nx = -CMario::GetInstance()->nx;
			}
		}
		else {
			if (!isOnGround) {
				if (isFlying && !flyTime->IsTimeUp()) {
					this->state = MARIO_STATE_FLY;
				}
				else {
					isFlying = false;
					this->state = MARIO_STATE_JUMP;
				}
				isAttack = false;
				attackStart->Stop();
			}
		}
	}
	

	//update listBullet
	tail->Update(dt, coObjects);

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
	
	CollideWithObject(coObjects, listEffect);

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

			// MARIO'S HEAD COLLIDE WITH OBJECT // ny > 0
			if (e->ny > 0) { 
				// BRICK
				if (e->obj->GetType() == ObjectType::BRICK) {
					vy = 0;
					CBrick* brick = dynamic_cast<CBrick*>(e->obj); // if e->obj is Brick 
					if (brick->GetBrickType()!= BRICK_BROKEN) {

						// BRICK_GLASS
						if (brick->GetBrickType() == BRICK_GLASS) {
							
							if (!brick->isBroken) {
								brick->SetState(BRICK_STATE_BOUNDING);

								switch (brick->GetTypeItem()) {
								case CONTAIN_NONE:
									{
										//brick->SetState(BRICK_STATE_BOUNDING);
										if (brick->GetCountItem() == 0)
											brick->SetState(BRICK_STATE_BROKEN);
										break;
									}
									
								case CONTAIN_ITEM_UP:
									{
										//brick->SetState(BRICK_STATE_BOUNDING);
										if (brick->GetCountItem() > 0)
											brick->count--;
										if (brick->GetCountItem() == 0)
											brick->SetState(BRICK_STATE_BROKEN);
										break;
									}
									
								case CONTAIN_COIN: 
									{
										//brick->SetState(BRICK_STATE_BOUNDING);
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
								default:
									break;
								}
								
							}
						}
						// BRICK_QUESTION
						else {
							if (brick->GetCountItem() == 1) {
								if (brick->GetBrickType() == BRICK_QUESTION)
									brick->SetBrickType(BRICK_BROKEN);
							}
							if (brick->GetTypeItem() == CONTAIN_COIN) {
								AddScore(100);
								AddCoin();

								float b_x, b_y;
								brick->GetPosition(b_x, b_y); // vi tri cua brick
								CGameObject* effect = new CoinEffect({ b_x,b_y });
								listEffect->push_back(effect);
							}
							brick->SetState(BRICK_STATE_BOUNDING);
							if (brick->GetCountItem() > 0)
								brick->count--;
						}
					}

				}
				
				// GROUND (color box)
				if (e->obj->GetType() == ObjectType::GROUND) {
					CGround* ground = dynamic_cast<CGround*>(e->obj); // if e->obj is Ground 
					if (ground->isInteract) {
						x += dx;
						y += dy;
					}
				}
			}

			// MARIO COLLIDE WITH OBJECT THEO PHUONG X
			if (e->nx != 0) {
				if (e->obj->GetType() == ObjectType::GROUND) {
					CGround* ground = dynamic_cast<CGround*>(e->obj); // if e->obj is Ground 
					if (ground->isInteract) {
						x += dx; // duoc phep di xuyen
					}
					else {
						if (state == MARIO_STATE_RUN) { // dang chay va cham tuong thi di bo
							if (vx > 0)
								SetAccelerate(0);
							else
								SetAccelerate(0);
						}
					}
				}
				else {
					if (state == MARIO_STATE_RUN) { // dang chay va cham tuong thi di bo
						if (vx > 0)
							SetAccelerate(0);
						else
							SetAccelerate(0);
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];

	CollideWithItem(coItem);
}

void CMario::CollideWithObject(vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* listEffect) {

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
		case ObjectType::P_SWITCH:
			if (this->IsCollidingWithObjectNy(coObjects->at(i)))
				coObjects->at(i)->SetState(SWITCH_STATE_OFF);
			break;
		case ObjectType::GOOMBA:
			if (untouchable == 0 && GetState() != MARIO_STATE_DIE) {
				if (this->IsCollidingWithObjectNy_1(coObjects->at(i))) {
					CGoomba* goomba = dynamic_cast<CGoomba*>(coObjects->at(i));

					if (goomba->GetState() != STATE_DESTROYED
						&& goomba->GetState() != ENEMY_STATE_DAMAGE
						&& goomba->GetState() != ENEMY_STATE_DIE_BY_ATTACK)
					{
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
		case ObjectType::NINJA:
			
			// CHUA XU LY BOOMERANG

			if (untouchable == 0 && GetState() != MARIO_STATE_DIE) {
				if (this->IsCollidingWithObjectNy_1(coObjects->at(i))) {
					Ninja* ninja = dynamic_cast<Ninja*>(coObjects->at(i));

					if (ninja->GetState() != STATE_DESTROYED
						&& ninja->GetState() != ENEMY_STATE_DAMAGE
						&& ninja->GetState() != ENEMY_STATE_DIE_BY_ATTACK)
					{
						ninja->damageOnTop();
						this->vy = -MARIO_JUMP_DEFLECT_SPEED;

						// them effect
						this->AddScore(100);
						float g_x, g_y;
						ninja->GetPosition(g_x, g_y);
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

					if (koopa->GetState() != KOOPAS_STATE_SHELL_IDLE) 
					{
						if (koopa->isWing) {
							koopa->SetState(KOOPAS_STATE_WALKING);
						}
						else {
							koopa->SetState(KOOPAS_STATE_SHELL_IDLE);
							this->vy = -MARIO_JUMP_DEFLECT_SPEED;
							// them effect
							this->AddScore(100);
							float k_x, k_y;
							koopa->GetPosition(k_x, k_y);
							CGameObject* effect = new ScoreEffect({ k_x, k_y }, 100);
							listEffect->push_back(effect);
						}
					}
					else { // TH : KOOPA LA SHELL IDLE

						float m_x, m_y, k_x, k_y;
						this->GetPosition(m_x, m_y); // vi tri cua mario
						koopa->GetPosition(k_x, k_y); // vi tri cua shell
						if ((k_x -m_x) > 0)
							koopa->nx = -1;
						else
							koopa->nx = 1;
						koopa->SetState(KOOPAS_STATE_SHELL_RUNNING);
						this->vy = -MARIO_JUMP_DEFLECT_SPEED;

					}
				}
				// Va cham theo phuong ngang
				else if (this->IsCollidingWithObjectNx(coObjects->at(i))) {
					CKoopas* koopa_x = dynamic_cast<CKoopas*>(coObjects->at(i));
					
					if (koopa_x->GetState() == KOOPAS_STATE_SHELL_IDLE) {
						if (canHolding) {
							this->shell = koopa_x;
							koopa_x->isBeingHeld = true;
					
						}
						else {
							koopa_x->nx = -this->nx;
							koopa_x->SetState(KOOPAS_STATE_SHELL_RUNNING);
						}
					}
					else if(koopa_x->GetState() != KOOPAS_STATE_SHELL_IDLE) {
						koopa_x->vx = -koopa_x->vx; // di xuyen qua
						this->isDamaged(); // xu ly mario bi thuong
					}
				}
			}
			else if (isAABB(coObjects->at(i)) && !canHolding) {
				this->isDamaged(); // xu ly mario bi thuong
			}
			break;
		}
	}
}

void CMario::CollideWithItem(vector<LPGAMEOBJECT>* coItem) {
	
	for (UINT i = 0; i < coItem->size(); i++) {
		
		if (isAABB(coItem->at(i))) {
			if (coItem->at(i)->GetType() == ObjectType::LAST_ITEM)
			{
				cards.push_back(coItem->at(i)->animation_set->at(ani)->getCurrentFrame());
				SetState(MARIO_STATE_WALKING_RIGHT);
				isAutoGo = true;
			}
			else if (coItem->at(i)->GetType() == ObjectType::BRICK)//brick khi chuyen thanh tien
			{
				AddScore(100);
				AddCoin();
			}
			else if (coItem->at(i)->GetType() == ObjectType::COIN)//brick khi chuyen thanh tien
			{
				AddScore(100);
				AddCoin();
			}
			else {
				Item* item = dynamic_cast<Item*>(coItem->at(i));
				switch (item->GetItemType())
				{
				case ITEM_GREEN_MUSHROOM:
					// up effect?
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
	switch (stage) {
	case ID_SCENE_GREENLAND:
		if (GetLevel() == MARIO_LEVEL_FIRE)
			ani = MARIO_ANI_FIRE_GREENLAND;
		else if (GetLevel() == MARIO_LEVEL_RACOON)
			ani = MARIO_ANI_RACOON_GREENLAND;
		else if (GetLevel() == MARIO_LEVEL_BIG)
			ani = MARIO_ANI_BIG_GREENLAND;
		else
			ani = MARIO_ANI_SMALL_GREENLAND;
		break;
	default:

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
	}


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
	// GREEN LAND STATE
	case MARIO_STATE_IDLE_GREENLAND:
		vx = 0;
		vy = 0;
		break;
	case MARIO_STATE_WALKING_UP:
		vy = -MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_WALKING_DOWN:
		vy = MARIO_WALKING_SPEED;
		break;
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
	case MARIO_STATE_IDLE:
		//DecreaseSpeed();
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
		attackStart->Start(); //bd tinh gio danh
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (GetStage() == ID_SCENE_GREENLAND) {
		right = x + 16;
		bottom = y + 16;
	}
	else {
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
					right = left + MARIO_RACCOON_BBOX_WIDTH;
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

void CMario::Relife() {
	SetLevel(MARIO_LEVEL_SMALL);
	SetState(MARIO_STATE_IDLE);
	SetPosition(x, start_y);
	SetSpeed(0, 0);
}

void CMario::Reset() {
	GetLevel();
	SetState(MARIO_STATE_IDLE);
	SetPosition(x, y - MARIO_RACCOON_BBOX_WIDTH); //tang y tranh mario rot
	SetSpeed(0, 0);
}

void  CMario::RefreshState() {
	isOnGround = false;
	isAttack = false;
	isFlying = false;
	canHolding = false;
	isAutoGo = false;

// GREEN LAND SOLVING
	canWalkLeft = false;
	canWalkRight = false;
	canWalkUp = false;
	canWalkDown = false;

	canSwitchScene = false;

// HIDDEN MAP SOLVING
	isInHiddenMap = false;
	canGoThroughPipe_up = false;
	canGoThroughPipe_down = false;
	
	collideGround = NULL;

	if (GetStage() == ID_SCENE_GREENLAND) {
		SetState(MARIO_STATE_IDLE_GREENLAND);
	}
	else {
		SetState(MARIO_STATE_IDLE);
	}
}
void CMario::StartUntouchable() {
	this->untouchable = 1;
	this->untouchable_start = GetTickCount64();
}

void CMario::ToRight() {
	nx = 1;
	/*if (vx == 0)
		vx = MARIO_WALKING_SPEED;
	if (vx <= 0)
		vx = 0;*/
	if (vx == 0) {
		vx = MARIO_WALKING_SPEED;
	}
	if (vx <= 0)
		SetState(MARIO_STATE_STOP);

}
void CMario::ToLeft() {
	nx = -1;
	if (vx == 0) {
		vx = -MARIO_WALKING_SPEED;
	}
	if (vx >= 0)
		SetState(MARIO_STATE_STOP);
}

CMario* CMario::__instance = nullptr;
CMario* CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario();
	return __instance;
}