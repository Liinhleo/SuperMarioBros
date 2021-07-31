#include "GreenLandObject.h"

#include "GreeenLand.h"
#include "Utils.h"

GreenLandObject::GreenLandObject(D3DXVECTOR2 position, int t)
{
	this->GL_ObjectType = t;
	type = ObjectType::GL_OBJECT;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(3));
	this->start_x = position.x;
	this->start_y = position.y;

	if (GetObjectType() == GUARD_OBJECT)
	{
		vx = 0.015f;
	}

}

void GreenLandObject::Render()
{
	if (GetObjectType() == PLANT_OBJECT)
		ani = ANI_PLANT_OBJECT;
	else if (GetObjectType() == TEXT_OBJECT)
		ani = ANI_TEXT_HELP;
	else
	{
		if (vx < 0)
			ani = ANI_GUARD_WALK_LEFT;
		else
			ani = ANI_GUARD_WALK_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
}

void GreenLandObject::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	CGameObject::Update(dt);
	if (GetObjectType() == GUARD_OBJECT)
	{
		x += dx;
		if (x > start_x + BRICK_BBOX_SIZE|| x < start_x)
		{
			if (x > start_x + BRICK_BBOX_SIZE)
				x = start_x + BRICK_BBOX_SIZE;
			if (x < start_x)
				x = start_x;
			vx = -vx;
		}
	}

}