#pragma once
#include "GameObject.h"


#define PLANT_OBJECT	1
#define TEXT_OBJECT		2
#define GUARD_OBJECT	3

#define ANI_GUARD_WALK_RIGHT		0
#define ANI_GUARD_WALK_LEFT			1
#define ANI_PLANT_OBJECT			2
#define ANI_TEXT_HELP				3

class GreenLandObject :
    public CGameObject
{
	int GL_ObjectType;
public:
	GreenLandObject(int t);
	~GreenLandObject() {}

	int GetObjectType() { return GL_ObjectType; }

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};

};

