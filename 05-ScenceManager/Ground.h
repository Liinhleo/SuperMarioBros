#pragma once
#include "GameObject.h"
#include "define.h"

class CGround : public CGameObject
{
public:
	CGround() {
		type = ObjectType::GROUND;
	};
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};