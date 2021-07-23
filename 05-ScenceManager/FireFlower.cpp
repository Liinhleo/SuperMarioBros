#include "FireFlower.h"
#include "Utils.h"

FireFlower::FireFlower(float x, float y, int type) :Plant(x, y) {
	FlowerType = type;
	this->type = ObjectType::FIRE_FLOWER;

	hiddenTimer = new Timer(HIDDEN_TIME);
	attackTimer = new Timer(ATTACK_TIME);

	if (FlowerType == FIRE_FLOWER_RED) {
		this->SetAnimationSet(CAnimationSets::GetInstance()->Get(6));
	}
	else {
		this->SetAnimationSet(CAnimationSets::GetInstance()->Get(7));
	}
}

bool FireFlower::CheckDistancePlayer(D3DXVECTOR4 player)
{
	if (abs(player.x - start_x) < MAX_ZONE) // trong vung gan hoa
		return true;
	return false;
}

void FireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player) {
	Plant::Update(dt, coObjects, player);
	
	DebugOut(L"State ===== %d \n", state);
	DebugOut(L"y ===== %f \n", y);

	// Xet vi tri ban cua dan theo zone of Mario
	if (player.x < start_x) {		
		// mario_x o ben TRAI fireflower
		if (player.y < this->y) {	
			// mario_y THAP hon fireflower
			if (CheckDistancePlayer(player))
				zoneOfMario = MARIO_LEFT_DOWN_NEAR;
			else
				zoneOfMario = MARIO_LEFT_DOWN_FAR;
		}
		else {						
			// mario_y CAO hon fireflower
			if (CheckDistancePlayer(player))
				zoneOfMario = MARIO_LEFT_UP_NEAR;
			else
				zoneOfMario = MARIO_LEFT_UP_FAR;
		}
		nx = -1;
	}
	else {	
		// mario_x o ben PHAI fireflower
		if (player.y < this->y) {	
			// mario_y THAP hon fireflower
			if (CheckDistancePlayer(player))
				zoneOfMario = MARIO_RIGHT_DOWN_NEAR;
			else
				zoneOfMario = MARIO_RIGHT_DOWN_FAR;
		}
		else {	
			// mario_y CAO hon fireflower
			if (CheckDistancePlayer(player))
				zoneOfMario = MARIO_RIGHT_UP_NEAR;
			else
				zoneOfMario = MARIO_RIGHT_UP_FAR;
		}
		nx = 1;
	}

	if (isAttack) {
		if (nx > 0) { // PHAI
			fireBall = new FireFlowerBall({ x + 6, y + 6 }, nx, zoneOfMario);
		}
		else {
			fireBall = new FireFlowerBall({ x - 6, y + 6 }, nx, zoneOfMario);
		}
		isAttack = false;
		coObjects->push_back(fireBall);
	}

	// step 1: (state = moving up)
	if (GetFlowerType() == FIRE_FLOWER_RED) {
		if (GetState() == PLANT_STATE_MOVING_UP && y < start_y - FIRE_FLOWER_RED_BBOX_HEIGHT){
			y = start_y - FIRE_FLOWER_RED_BBOX_HEIGHT; // vi tri y tang len maximum ~ flower height
			SetState(PLANT_STATE_ATTACK);
		}
	}
	else {
		if (GetState() == PLANT_STATE_MOVING_UP && y < start_y - FIRE_FLOWER_GREEN_BBOX_HEIGHT) {
			y = start_y - FIRE_FLOWER_GREEN_BBOX_HEIGHT;
			SetState(PLANT_STATE_ATTACK);
		}
	}

}


void FireFlower::Render() {
	switch (zoneOfMario)
	{
	case MARIO_LEFT_DOWN_FAR:
	case MARIO_LEFT_DOWN_NEAR:
		ani = FIRE_FLOWER_ANI_LEFT_DOWN;
		break;
	case MARIO_LEFT_UP_FAR:
	case MARIO_LEFT_UP_NEAR:
		ani = FIRE_FLOWER_ANI_LEFT_UP;
		break;
	case MARIO_RIGHT_DOWN_FAR:
	case MARIO_RIGHT_DOWN_NEAR:
		ani = FIRE_FLOWER_ANI_RIGHT_DOWN;
		break;
	case MARIO_RIGHT_UP_FAR:
	case MARIO_RIGHT_UP_NEAR:
		ani = FIRE_FLOWER_ANI_RIGHT_UP;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}

void FireFlower::SetState(int state) {
	Plant::SetState(state);
	if (state == PLANT_STATE_ATTACK) {
		isAttack = true;
	}
}

void FireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + FIRE_FLOWER_BBOX_WIDTH;
	if (GetFlowerType() == FIRE_FLOWER_RED) {
		bottom = top + FIRE_FLOWER_RED_BBOX_HEIGHT;
	}
	else {
		bottom = top + FIRE_FLOWER_GREEN_BBOX_HEIGHT;
	}
}