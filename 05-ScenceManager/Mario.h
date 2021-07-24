#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Ground.h"
#include "Timer.h"
#include "Bullet.h"
#include "MarioTail.h"

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

public: 


	Bullet* CreateBullet(float x, float y, int nx)
	{
		Bullet* bullet = new Bullet({ x, y }, nx);
		return bullet;
	}
	vector< LPGAMEOBJECT> listBullet;


	float a;
	MarioTail* tail; // mario has a tail when level = RACOON

	Timer* flyTime = new Timer(TIME_FLY);

	bool isOnGround = false;
	bool isAttack = false;
	bool isInHiddenMap = false;
	bool isFlying = false;

	CMario(float x = 0.0f, float y = 0.0f);
	~CMario() {};

	// Van toc
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	// Gia toc x
	void SetAccelerate(float accelerate) { this->a = accelerate; };
	float GetAccelerate() { return a; }

	// Trang thai
	void SetState(int state);
	int GetState() { return state; }

	// Level
	void SetLevel(int l) { level = l; }
	int GetLevel() {return level; }

	void UpdateSpeed(DWORD dt);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects, vector <LPGAMEOBJECT>* coItem);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void isDamaged();
	void StartUntouchable();

	void StartTimeFly();

	void Reset();
	void CollideWithItem(vector<LPGAMEOBJECT>* Item);


};