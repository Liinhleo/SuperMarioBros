#pragma once
#include "Item.h"

#define LEAF_ANI_RIGHT	0
#define LEAF_ANI_LEFT	1


class SuperLeaf :
    public Item
{

public:
	SuperLeaf(D3DXVECTOR2 position);
	~SuperLeaf() {}

	bool isFalling;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

};

