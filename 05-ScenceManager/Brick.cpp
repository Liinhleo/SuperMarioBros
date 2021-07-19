#include "Brick.h"

CBrick::CBrick(int typeBrick, int typeItem, int count) {
	
	brickType = type;
	this->type = ObjectType::BRICK;
	this->typeItem = typeItem;
	this->count = count;
	SetState(BRICK_STATE_ACTIVE);
}
void  CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void CBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case BRICK_STATE_BROKEN:
		break;
	case BRICK_STATE_INACTIVE:
		isExplore = true;
		// xu ly logic sau
		break;
	case BRICK_STATE_ACTIVE:
		vy = 0;
		break;
	}
}


void CBrick::Render()
{
	int ani = -1;
	int Btype = GetBrickType();
	switch (Btype){
		case BRICK_NORMAL:
			ani = BRICK_ANI_NORMAL_ACTIVE;
			break;
		case BRICK_MUSIC:
			ani = BRICK_ANI_MUSIC_ACTIVE;
			break;
		case BRICK_QUESTION:
			if (state == BRICK_STATE_BROKEN) ani = BRICK_ANI_QUESTION_BROKEN;
			else ani = BRICK_ANI_QUESTION_ACTIVE;
			break;
		case BRICK_GLASS:
			if (state == BRICK_STATE_BROKEN) ani = BRICK_ANI_GLASS_ACTIVE;
			else ani = BRICK_ANI_GLASS_BROKEN;
			break;

	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}