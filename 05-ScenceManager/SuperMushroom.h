#pragma once
#include "Item.h"

#define ITEM_ANI_RED_MUSHROOM		0
#define ITEM_ANI_GREEN_MUSHROOM		1

class SuperMushroom :  public Item
{
public:

	SuperMushroom(D3DXVECTOR2 position, int type);
	~SuperMushroom() {}

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

