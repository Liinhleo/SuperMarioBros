#pragma once
#include "GameObject.h"
#include "Timer.h"

#define ITEM_SUPERLEAF			0
#define ITEM_RED_MUSHROOM		1
#define ITEM_GREEN_MUSHROOM		2

#define ITEM_STATE_HIDDEN	0	// hidden ben trong brick
#define ITEM_STATE_ABLE		1	// rot ra
#define ITEM_STATE_DESTROY	2	// 

#define ITEM_SPEED_Y	0.05f
#define ITEM_GRAVITY	0.0003f

#define ITEM_ANI_SUPERLEAF_RIGHT	0
#define ITEM_ANI_SUPERLEAF_LEFT		1
#define ITEM_ANI_RED_MUSHROOM		2
#define ITEM_ANI_GREEN_MUSHROOM		3

#define ITEM_DISAPPEAR_TIME  5000

class Item :   public CGameObject
{
public:
	int ItemType;

	Item();
	~Item() {};

	Timer* disapperTimer = new Timer(ITEM_DISAPPEAR_TIME);
	int GetItemType() { return ItemType; }

	virtual void Render() = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

