#pragma once
#include "GameObject.h"
#include "Timer.h"

#define SWITCH_STATE_ON		0
#define SWITCH_STATE_OFF	1


#define SWITCH_ANI_ON	0
#define SWITCH_ANI_OFF	1

class Pswitch :
    public CGameObject
{
public:
	Pswitch(float x, float y);
	~Pswitch() {};

	bool onChange = false; // ktra nhan nut -> brick -> coin

	Timer* changeTimer = new Timer(12000);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* colliable_objects, vector<LPGAMEOBJECT>* coObj);

	void SetState(int state);
};

