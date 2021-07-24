#pragma once
#include "GameObject.h"
#include "define.h"
 
// brickType
#define BRICK_BROKEN		1 // gach question/glass after broken 
#define BRICK_QUESTION		2
#define BRICK_GLASS			3
#define BRICK_MUSIC			4

/*  ========== STATE BRICK ========== */
#define BRICK_STATE_BOUNDING	0	// nay tung tung
#define BRICK_STATE_ACTIVE		1
#define BRICK_STATE_BROKEN		2  // broken -> gach cung
#define BRICK_STATE_HIDDEN		3  // bien mat

/*  ========== ANI BRICK ==========  */
#define BRICK_ANI_ACTIVE		0	
#define BRICK_ANI_BROKEN		1  // va cham: question & glass
#define BRICK_ANI_HIDDEN		2  // bien thanh item

// Type Item that brick contain 
#define CONTAIN_NONE			0
#define CONTAIN_ITEM			1
#define CONTAIN_COIN			2
#define CONTAIN_PSWITCH			3
#define CONTAIN_GREEN_MUSHROOM	4

class CBrick : public CGameObject
{
public:
	int brickType;	// 1=normal 	2=question	3=question_broken	4=bronze
	int typeItem;	// 0=none 1= superLeaf 2=coin 3=p_switch 4=mushroom
	int count;		//number of item 

	bool isBroken = false; // if mario attack -> true
	bool isFallingItem = false;

	CBrick(float x, float y,int typeBrick, int typeItem, int count);
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