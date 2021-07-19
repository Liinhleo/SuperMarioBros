#include "MarioTail.h"
#include "Mario.h"


MarioTail::MarioTail(){
	SetState(TAIL_STATE_DISABLE);
}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, D3DXVECTOR2 playerPos, int playerNx) {
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

}

void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom){
	left = x;
	top = y;
	right = x + TAIL_BBOX_WIDTH;
	bottom = y + TAIL_BBOX_HEIGHT;
}

void MarioTail::Render() {
	int ani = -1;
	if (state == TAIL_STATE_HIT) {
		if (nx > 0) ani = TAIL_ANI_RIGHT;
		else ani = TAIL_ANI_LEFT;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void MarioTail::SetState(int state) {
	switch (state)
	{
	case TAIL_STATE_HIT:
		//isTail = true;
		break;

	case TAIL_STATE_DISABLE:
		//isTail = false;
		break;
	}
}