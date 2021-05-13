#pragma once
#include "GameObject.h"
#define MAX_HEIGHT 

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);

	bool isWalking = false;
	bool isJump = false;
	bool isJumpAsFly = false;
	bool isSitting = false;
	bool isRun = false;
	bool isStop = false;
	bool isAttack = false;
	bool isOnTheGround = true;
	bool isAutoWalk = false;
	bool isFalling = false;

	bool canSpeedUp = false;
	bool canHolding = false;
	bool canFly = false;



	void Walk();
	void Jump();


	void Die();
	

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	int GetState() { return state; }

	void SetLevel(int l) { level = l; }
	int GetLevel();

	void StartUntouchable();

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};