#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	bool hasPortal; // 1: true
	int scene_id;	// target scene to switch to 

public:

	bool canMoveLeft;
	bool canMoveRight;
	bool canMoveUp;
	bool canMoveDown;

	CPortal(bool hasPortal, int IdScene, bool l, bool r, bool a, bool u);
	~CPortal(){}

	int GetSceneID() { return this->scene_id; }
	bool IsHasPortal() { return this->hasPortal; }

	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};