#pragma once
#include "Item.h"

#define LEAF_SPEED_FALL	0.03f
#define LEAF_SPEED_X	0.08f


class SuperLeaf :
    public Item
{
	float left, right;

public:
	SuperLeaf(D3DXVECTOR2 position);
	~SuperLeaf() {}

	bool isFalling = false;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

};

