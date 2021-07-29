#include "MarioTail.h"
#include "Mario.h"
#include "Utils.h"
#include "Goomba.h"
#include "Plant.h"
#include "Pswitch.h"

MarioTail::MarioTail(float& x, float& y, int& nx){
	xMario = &x;
	yMario = &y;
	nxMario = &nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	// chi xet va cham khi tail o frame 0 || 2 || 4
	
	if (animation_set->at(ani)->getCurrentFrame() == 0
		||animation_set->at(ani)->getCurrentFrame() == 2
		|| animation_set->at(ani)->getCurrentFrame() == 4) {

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
								if (brick->GetTypeItem() == CONTAIN_PSWITCH) {
									CGameObject* obj = new Pswitch(brick->start_x, brick->start_y - BRICK_BBOX_SIZE);
									coObjects->push_back(obj);
									brick->SetBrickType(BRICK_BROKEN);
								}
								else
									brick->SetState(BRICK_STATE_BROKEN);
								CMario::GetInstance()->AddScore(100);							
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
}

void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom){
	if (state == TAIL_STATE_HIT) {
		switch (animation_set->at(ani)->getCurrentFrame()) {
		case 0:
		case 4:
			if (nx > 0) {
				left = x - 2; 
				right = left + TAIL_BBOX_WIDTH;

			}

			else {
				left = x + DISTANCE_XTAIL_MARIO;
				right = left + TAIL_BBOX_WIDTH + 5;
			}
			top = y + 19;
			break;

		case 2:
			if (nx > 0) {
				left = x + DISTANCE_XTAIL_MARIO;
				right = left + TAIL_BBOX_WIDTH + 5;
			}
			else {
				left = x - 2;
				right = left + TAIL_BBOX_WIDTH;
			}
			top = y + 19;
			break;
		default:
			break;
		}
		bottom = top + 6;
	}

}

void MarioTail::Render() {
	if (state == TAIL_STATE_HIT) {
		if (nx > 0) ani = TAIL_ANI_RIGHT;
		else ani = TAIL_ANI_LEFT;
	}
	//animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void MarioTail::SetState(int state) {
	
	CGameObject::SetState(state);
}

bool MarioTail::isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float tail_left,
		tail_top,
		tail_right,
		tail_bottom;

	GetBoundingBox(tail_left, tail_top, tail_right, tail_bottom);
	return CGameObject::CheckAABB(tail_left, tail_top, tail_right, tail_bottom, obj_left, obj_top, obj_right, obj_bottom);
}
