#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Ground.h"

#define MAX_HEIGHT 

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 

public: 
	float a; // gia toc cua Mario
	float gravity; // trong luc cua Mario

	bool isOnTheGround = true;

	CMario(float x = 0.0f, float y = 0.0f);

	// Van toc
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	// Gia toc
	void SetAccelerate(float a) { this->a = a; }
	void GetAccelerate(float& a) { a = this->a; }

	// Trong luc
	void SetGravity(float gravity) { this->gravity = gravity; }
	void GetGravity(float& gravity) { gravity = this->gravity; }

	// Trang thai
	void SetState(int state);
	int GetState() { return state; }

	// Level
	void SetLevel(int l) { level = l; }
	int GetLevel() {return level; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void StartUntouchable();
	void Reset();

	// Collision
	void CollisonGround(vector<LPGAMEOBJECT>* coObjects);
	void CollisonEnemy(vector<LPGAMEOBJECT>* coObjects);
	void CollisonPortal(vector<LPGAMEOBJECT>* coObjects);

};