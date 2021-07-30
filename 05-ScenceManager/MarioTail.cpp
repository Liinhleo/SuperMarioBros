#include "MarioTail.h"
#include "Mario.h"
#include "Utils.h"
#include "Goomba.h"
#include "Plant.h"
#include "Pswitch.h"

MarioTail* MarioTail::__instance = nullptr;

MarioTail* MarioTail::GetInstance()
{
	if (__instance == NULL) __instance = new MarioTail();
	return __instance;
}
MarioTail::MarioTail(){

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 playerPos, int playerNx)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	this->x = playerPos.x;
	this->y = playerPos.y;
	this->nx = playerNx;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (this->isAABB(coObjects->at(i))) {
			switch (coObjects->at(i)->GetType()) {
			case ObjectType::GROUND:
			case ObjectType::PIPE:
				break;

			case ObjectType::BRICK:
				{
					CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));

					if (brick->GetBrickType() != BRICK_BROKEN) {

						// QUESTION BRICK
						if (brick->GetCountItem() == 1 && brick->GetBrickType() == BRICK_QUESTION)
							brick->SetBrickType(BRICK_BROKEN);

						else if (brick->GetCountItem() != BRICK_GLASS)
							brick->SetState(BRICK_STATE_BOUNDING);

						// GLASS BRICK
						else {
							if (!brick->isBroken) {
								if (brick->GetTypeItem() == CONTAIN_PSWITCH) {
									CGameObject* obj = new Pswitch(brick->start_x, brick->start_y - BRICK_BBOX_SIZE);
									coObjects->push_back(obj);
									brick->SetBrickType(BRICK_BROKEN);
								}
								else
									brick->SetState(BRICK_STATE_BROKEN);
								CMario::GetInstance()->AddScore(100);
							}
															
						}
						// chua nhieu coin
						if (brick->GetTypeItem() == CONTAIN_COIN)
						{
							CMario::GetInstance()->AddScore(100);
							CMario::GetInstance()->AddCoin();
						}
						brick->count--;
					}
					break;
				}
					
			case ObjectType::PIRANHA_FLOWER:
			case ObjectType::FIRE_FLOWER:
				{
					Plant* plant = dynamic_cast<Plant*>(coObjects->at(i));
					plant->damageByWeapon();
					CMario::GetInstance()->AddScore(100);
					break;
				}

			case ObjectType::GOOMBA:
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(coObjects->at(i));
					goomba->SetState(ENEMY_STATE_DIE_BY_ATTACK);
					CMario::GetInstance()->AddScore(100);
					break;
				}

			case ObjectType::KOOPA:
				{
					CKoopas* koopa = dynamic_cast<CKoopas*>(coObjects->at(i));
						
					this->GetPosition(x, y); // vi tri cua mario
					float k_x, k_y;
					koopa->GetPosition(k_x, k_y); // vi tri cua shell
					if ((k_x - this->x) > 0)
						koopa->nx = -1;
					else
						koopa->nx = 1;
					koopa->SetState(ENEMY_STATE_DIE_BY_ATTACK);
					CMario::GetInstance()->AddScore(100);

					break;
				}
			default:
				return;
					
			}
		}
	}
}

void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom){
	
	if (state == TAIL_STATE_HIT) {
		
		switch (animation_set->at(ani)->getCurrentFrame()) {
		case 0:
		case 4:
			if (nx > 0) {
				left = x - 5; 
			}

			else {
				left = x + DISTANCE_XTAIL_MARIO + TAIL_BBOX_WIDTH;
			}
			top = y + 19;

			break;

		case 2:
			if (nx > 0) {
				left = x + DISTANCE_XTAIL_MARIO + TAIL_BBOX_WIDTH;
			}
			else {
				left = x - 5;
			}
			top = y + 19;

			break;
		default:
			break;
		}
		right = left + TAIL_BBOX_WIDTH;
		bottom = top + TAIL_BBOX_HEIGHT;
	}

}

void MarioTail::Render() {
	if (state == TAIL_STATE_HIT) {
		if (nx > 0) ani = TAIL_ANI_RIGHT;
		else ani = TAIL_ANI_LEFT;
	}
	RenderBoundingBox();
}

void MarioTail::SetState(int state) {
	
	CGameObject::SetState(state);
}