#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Ground.h"

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	float jumpStartY;
	float a; // gia toc cua Mario
	float g; // gia toc trong truong cua Mario

	bool isOnGround = false;
	bool isJumpHigh = false;

	CMario(float x = 0.0f, float y = 0.0f);

	// Van toc
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	// Gia toc theo phuong x
	void SetAccelerate(float a) { this->a = a; }
	void GetAccelerate(float& a) { a = this->a; }

	// Gia toc trong truong theo phuong y
	void SetGravity(float gravity) { this->g = gravity; }
	void GetGravity(float& gravity) { gravity = this->g; }

	// Trang thai
	void SetState(int state);
	int GetState() { return state; }

	// Level
	void SetLevel(int l) { level = l; }
	int GetLevel() {return level; }

	void UpdateSpeed(DWORD dt);
	void UpdateHeight(DWORD dt);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void isDamaged();
	void StartUntouchable();
	void Reset();

};