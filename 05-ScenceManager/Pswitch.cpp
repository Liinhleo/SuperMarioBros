#include "Pswitch.h"
#include "Brick.h"

Pswitch::Pswitch(float x, float y) {
	this->type = ObjectType::P_SWITCH;
	this->x = x;
	this->y = y;
	this->start_y = y;

	state = SWITCH_STATE_ON;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(31));
}

void Pswitch::Render() {
	if (GetState() == SWITCH_STATE_ON)
		ani = SWITCH_ANI_ON;
	else
		ani = SWITCH_ANI_OFF;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Pswitch::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (state == SWITCH_STATE_OFF)
		l = t = b = r = 0;
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_SIZE;
		b = y + BRICK_BBOX_SIZE;
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
		y = y + 10;
		break;
	case SWITCH_STATE_ON:
		break;
	default:
		break;
	}
}

void Pswitch::Update(DWORD dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* items) {
	
	CGameObject::Update(dt);

	vector<LPGAMEOBJECT> glassBrick;
	if (onChange) {// ktra nhan nut
		// gach bien mat -> state coin
		for (int i = 0; i < objects->size(); i++) {
			if (objects->at(i)->GetType() == ObjectType::BRICK) {

				CBrick* brick = dynamic_cast<CBrick*>(objects->at(i));

				if (brick->GetBrickType() == BRICK_GLASS && brick->GetState() != BRICK_STATE_HIDDEN) {
					brick->SetState(BRICK_STATE_HIDDEN);
				}
			}
		}
		onChange = false;
	}

	if (changeTimer && changeTimer->IsTimeUp()) {
		//tien bien lai thanh gach
		for (int i = 0; i < items->size(); i++) {
			if (dynamic_cast<CBrick*>(items->at(i))) {

				CBrick* brick = dynamic_cast<CBrick*>(items->at(i));

				if (brick->GetBrickType() == BRICK_GLASS && brick->GetState() == BRICK_STATE_HIDDEN) {
					
					brick->SetState(BRICK_STATE_ACTIVE);

				}
			}
		}
		changeTimer->Stop();
	}

}