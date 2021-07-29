#include "SuperLeaf.h"
#include "Utils.h"

SuperLeaf::SuperLeaf(D3DXVECTOR2 position) {
	this->x = position.x;
	this->y = position.y;

	this->GetPosition(start_x, start_y);
	this->left = start_x - BRICK_BBOX_SIZE;
	this->right = start_x + BRICK_BBOX_SIZE;

	this->ItemType = ITEM_SUPERLEAF;
	this->vy = -ITEM_SPEED_Y;
 	this->type = ObjectType::ITEM;
	DebugOut(L"left============ x %f \n", left);
	DebugOut(L"right======== y %f \n", left);
}

void SuperLeaf::Render()
{
	if (vx <= 0) ani = ITEM_ANI_SUPERLEAF_LEFT;
	else ani = ITEM_ANI_SUPERLEAF_RIGHT;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void SuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"SuperLeafleft======= x %f \n", left);
	DebugOut(L"SuperLeafright========== y %f \n", right);

	DebugOut(L"SuperLeaf ======x %f \n", x);
	DebugOut(L"SuperLeaf========== y %f \n", y);

	Item::Update(dt, coObjects);

	x += dx;
	y += dy;

	// 1. nhay len brick
	if (!isFalling)
		vy += (ITEM_GRAVITY * dt);
	else
		vy = LEAF_SPEED_FALL;

	// 2. va cham voi brick 
	if (!isFalling && vy > 0) 
	{
		isFalling = true;
		vx = -LEAF_SPEED_X;
	}
	// 3. Roi xuong
	if (isFalling)
	{
		if (vx < 0 && x < left)
		{
			vx = -vx;
			x = left;
		}
		if (vx > 0 && x > right)
		{
			vx = -vx;
			x = right;
		}
	}

	DebugOut(L"Roi xuong x %f \n", x);
	DebugOut(L"Roi xuong y %f \n", y);
}
