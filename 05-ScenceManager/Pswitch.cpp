#include "Pswitch.h"
#include "Brick.h"

Pswitch::Pswitch(float x, float y) {
	this->type = ObjectType::P_SWITCH;
	this->x = x;
	this->y = y;
	this->start_y = y;

	state = SWITCH_STATE_ON;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(15));
}

void Pswitch::Render() {
	if (GetState() == SWITCH_STATE_ON)
		ani = SWITCH_ANI_ON;
	else
		ani = SWITCH_ANI_OFF;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Pswitch::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (state == SWITCH_STATE_OFF)
		l = t = b = r = 0;
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}

void Pswitch::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case SWITCH_STATE_OFF:
		changeTimer->Start();
		//gach bien thanh tien
		onChange = true;
		break;
	case SWITCH_STATE_ON:
		break;
	default:
		break;
	}
}

void Pswitch::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* colObj) {
	CGameObject::Update(dt);

}