#include "MarioTail.h"
#include "Mario.h"
#include "Utils.h"
#include "Goomba.h"
#include "Koopas.h"
#include "FireFlower.h"
#include "PiranhaFlower.h"

MarioTail::MarioTail(float& x, float& y, int& nx){
	xMario = &x;
	yMario = &y;
	nxMario = &nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
}

// Xet va cham theo 2 bbox CheckAABB is still ok
void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (animation_set->at(ani)->getCurrentFrame() >= 2 
		|| animation_set->at(ani)->getCurrentFrame() >= 4) {

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			float top, left, bottom, right; // bbox of object
			coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
			if (isCollision(left, top, right, bottom)) {
				if (coObjects->at(i)->type == ObjectType::GOOMBA
					|| coObjects->at(i)->type == ObjectType::KOOPA) {
					coObjects->at(i)->SetState(ENEMY_STATE_DIE_BY_ATTACK);
				}
				else if (coObjects->at(i)->type == ObjectType::FIRE_FLOWER 
					|| coObjects->at(i)->type == ObjectType::PIRANHA_FLOWER) {
					coObjects->at(i)->SetState(ENEMY_STATE_DESTROY);
				}
			}
		}
	}
	if (animation_set->at(ani)->getCurrentFrame() >= 2) {

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
				right = left + TAIL_BBOX_WIDTH + 2;
			}
			top = y + 19;
			break;

		case 2:
			left = x;
			top = y;
			if (nx > 0) {
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
			else {
				left = x + TAIL_BBOX_WIDTH - 2;
				right = left + MARIO_RACCOON_BBOX_WIDTH;
			}
			top = y + 19;
			break;
		default:
			break;
		}
		right = left + 10;
		bottom = top + 5;
	}

}

void MarioTail::Render() {
	int ani = -1;
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
