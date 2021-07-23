#include "Plant.h"
#include "Utils.h"

Plant::Plant(float x, float y) { 
	this->x = x;
	this->y = y;
	GetPosition(this->start_x, this->start_y);
	SetState(PLANT_STATE_MOVING_UP);
}

void Plant::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	//case ENEMY_STATE_DESTROY: // hit by bullet
	//	break;
	case PLANT_STATE_ATTACK:
		vy = 0;
		attackTimer->Start();
		break;
	case PLANT_STATE_HIDDEN: // nam ngoai vung hoat dong -> tinh theo thoi gian de up/down 
		vy = 0;
		hiddenTimer->Start();
		break;
	case PLANT_STATE_MOVING_UP:
		vy = -PLANT_SPEED_Y;
		break;
	case PLANT_STATE_MOVING_DOWN:
		vy = PLANT_SPEED_Y;
		break;
	}
}


void Plant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR4 player) {
	
	CGameObject::Update(dt);
	this->y += dy; // di chuyen len xuong theo dy'

	// step 2: (state = attack) 
	if (attackTimer->GetStartTime() != 0 && attackTimer->IsTimeUp()) {
		attackTimer->Stop();
		SetState(PLANT_STATE_MOVING_DOWN);
	} 
	// step 3: (move down)  -> het attackTimer 
	if (GetState() == PLANT_STATE_MOVING_DOWN && y > start_y) {
		y = start_y + 1;
		SetState(PLANT_STATE_HIDDEN);
	}

	// step 4: (hidden)  -> het hiddenTimer -> moving up (step 1) 
	if (hiddenTimer->GetStartTime() != 0 && hiddenTimer->IsTimeUp() && !CheckPlayerInZone(player)) {
		hiddenTimer->Stop();
		SetState(PLANT_STATE_MOVING_UP);
	}
}


//#define FIRE_FLOWER_BBOX_WIDTH			16
//#define FIRE_FLOWER_RED_BBOX_HEIGHT		32
//#define FIRE_FLOWER_GREEN_BBOX_HEIGHT	24

#define NEAR_X	24
#define FAR_X	40


void Plant::GetZoneAtive(float& l, float& t, float& r, float& b){ // mario dung trong vung nay thi khong troi len
	l = this->start_x - 24;
	r = this->start_x + 40;
	t = this->start_y - 48;
	b = this->start_y + 48;
}

bool Plant::CheckPlayerInZone(D3DXVECTOR4 player){ // ktra mario co nam trong vung hoat dong cua cay hay k 
	float l_plant, t_plant, r_plant, b_plant; 
	GetZoneAtive(l_plant, t_plant, r_plant, b_plant); 
	return AABBCheck(l_plant, t_plant, r_plant, b_plant, player.x, player.y, player.z, player.w);
}