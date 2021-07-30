#include "SpeedBar.h"
#include "Define.h"

SpeedBar::SpeedBar() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(1121));
}

void SpeedBar::Render(float x, float y, float vx) {

	float temp = (MARIO_MAX_SPEED - MARIO_WALKING_SPEED) / 7; //0.0114

	if (abs(vx) > (MARIO_WALKING_SPEED + temp * 6))
		ani = Speed_7;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 5))
		ani = Speed_6;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 4))
		ani = Speed_5;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 3))
		ani = Speed_4;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 2))
		ani = Speed_3;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 1))
		ani = Speed_2;
	else if (abs(vx) > (MARIO_WALKING_SPEED + temp * 0))
		ani = Speed_1;
	else
		ani = Speed_0;
	animation_set->at(ani)->Render(x, y);
}