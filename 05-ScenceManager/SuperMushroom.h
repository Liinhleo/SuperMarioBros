#pragma once
#include "Item.h"

class SuperMushroom :  public Item
{
public:

	SuperMushroom(D3DXVECTOR2 position, int type);
	~SuperMushroom() {}

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

