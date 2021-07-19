#pragma once
#include "GameObject.h"
#include "define.h"
 
class CBrick : public CGameObject
{
	int brickType; // 1=normal 	2=question	3=question_broken	4=bronze
	int typeItem; // 0=non 1= superLeaf 2=coin 3=p_switch 4=green_mushroom 
	int count; //number of coin 

public:

	bool isExplore = false; // if mario attack -> true

	CBrick(int typeBrick, int typeItem, int count);
	~CBrick() {};

	void SetBrickType(int type) { this->brickType = type; }
	int GetBrickType() { return this->brickType; }
	int GetTypeItem() { return this->typeItem; }
	int GetCountItem() { return this->count; }

	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};