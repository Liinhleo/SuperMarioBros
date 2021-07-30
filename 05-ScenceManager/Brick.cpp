#include "Brick.h"
#include "Utils.h"
#include "BrickBrokenEffect.h"

#define BROKEN_DISTANCE_X	8.0f // khoang cach piece of brick so vs brick
#define BRICK_BOUNDING_X	5.0f // khoang cach dich chuyen cua brick

CBrick::CBrick(float x, float y, int typeBrick, int typeItem, int count) {
	
	brickType = typeBrick;
	this->type = ObjectType::BRICK;
	
	this->x = x;
	this->y = y;
	this->start_x = x;
	this->start_y = y;
	GetPosition(this->start_x, this->start_y);

	this->typeItem = typeItem;
	this->count = count;

	if (isHidden) { // Check glass brick
		SetState(BRICK_STATE_HIDDEN);
		canPushUp = true;
	}
	else {
		SetState(BRICK_STATE_ACTIVE);
	}
}
void  CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	
	// UPDATE LIST PIECES 
	if (listPiece.size() >0) { // state broken
		if (listPiece.size() == 4) {
			for (auto piece : listPiece)
				piece->Update(dt, coObjects);
		}
		else {
			this->SetState(STATE_DESTROYED);
		}
	}
	for (size_t i = 0; i < listPiece.size(); i++){
		if (listPiece[i]->GetState() == STATE_DESTROYED) {
			listPiece.erase(listPiece.begin() + i);
			i--;
		}
	}

	if (y < (start_y - BRICK_BOUNDING_X) && vy < 0) {
		vy = -vy;
	}
	if (y > start_y) {
		y = start_y;
		SetState(BRICK_STATE_ACTIVE);
	}
	y += dy;
}


void CBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case BRICK_STATE_BOUNDING:
		if ((GetTypeItem() == CONTAIN_ITEM_UP 
			|| GetTypeItem() == CONTAIN_GREEN_MUSHROOM) && count == 1){}
			isFallingItem = true;
		vy = -0.2f;
		DebugOut(L"CONTAIN: %d \n", count);
		break;

	case BRICK_STATE_HIDDEN:
		break;

	case BRICK_STATE_BROKEN:
	{
		isBroken = true;
		BrickBrokenEffect* piece1 = new BrickBrokenEffect({ start_x + BROKEN_DISTANCE_X, start_y }, 1, 2.2); //PHAI DUOI
		BrickBrokenEffect* piece2 = new BrickBrokenEffect({ start_x + BROKEN_DISTANCE_X, start_y + BROKEN_DISTANCE_X }, 1, 1); // PHAI TREN
		BrickBrokenEffect* piece3 = new BrickBrokenEffect({ start_x, start_y }, -1, 2.2); // TRAI DUOI
		BrickBrokenEffect* piece4 = new BrickBrokenEffect({ start_x, start_y + BROKEN_DISTANCE_X }, -1, 1); // TRAI TREN
		listPiece = { piece1, piece2, piece3, piece4 };
	}		
	break;
		
	case BRICK_STATE_ACTIVE:
		vy = 0;
		break;
	}
}

void CBrick::Render()
{
	if (listPiece.size() > 0) {
		for (auto piece : listPiece)
			piece->Render();
		return;
	}

	if (GetBrickType() == BRICK_BROKEN)
		ani = BRICK_ANI_BROKEN;
	else {
		if (state == BRICK_STATE_HIDDEN && GetBrickType() == BRICK_GLASS)
			ani = BRICK_ANI_HIDDEN;
		else if (state == BRICK_STATE_HIDDEN && GetBrickType() == BRICK_MUSIC)
			return;
		else
			ani = BRICK_ANI_ACTIVE;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == BRICK_STATE_BROKEN || state == STATE_DESTROYED)
		l = t = b = r = 0;
	else {
		l = x;
		t = y;
		r = x + BRICK_BBOX_SIZE;
		b = y + BRICK_BBOX_SIZE;
	}

}