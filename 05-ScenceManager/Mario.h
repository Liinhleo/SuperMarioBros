#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Ground.h"
#include "Timer.h"
#include "Bullet.h"
#include "MarioTail.h"
#include "Koopas.h"

#define MARIO_TIME_ATTACK	400

class CMario : public CGameObject
{
	static CMario* __instance;

	int stage; 
	// HUD
	int score;
	int coin;

	int level;
	int untouchable;
	DWORD untouchable_start;

public: 
	static CMario* GetInstance();

	// Bullet
	vector< LPGAMEOBJECT> listBullet;
	Bullet* CreateBullet(float x, float y, int nx) {
		Bullet* bullet = new Bullet({ x, y }, nx);
		return bullet;
	}
	MarioTail* tail; // mario has a tail when level = RACOON
	CKoopas* shell = new CKoopas();


	LPGAMEOBJECT collideGround; // chua ground (doi tuong va cham theo truc y)
	float a; // accelerate

	Timer* flyTime = new Timer(TIME_FLY);
	Timer* attackStart = new Timer(MARIO_TIME_ATTACK);

	bool isOnGround = false;
	bool isAttack = false;
	bool isFlying = false;
	bool canHolding = false;

	// GREEN LAND SOLVING
	bool isIdling = false;
	bool canWalkLeft;
	bool canWalkRight;
	bool canWalkUp;
	bool canWalkDown ;
	bool canSwitchScene;

	D3DXVECTOR2 GreenLand_pos = { -1,-1 };

	void SetStage(int stage) { this->stage = stage; }
	int GetStage() { return this->stage; }

	void SetWorldMapPosition(float x, float y) { GreenLand_pos.x = x; GreenLand_pos.y = y; }
	void GetWorldMapPosition(float& x, float& y) { x = this->GreenLand_pos.x; y = this->GreenLand_pos.y; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);

	
	// HIDDEN MAP SOLVING
	bool isInHiddenMap = false;
	bool canGoThroughPipe_up = false;
	bool canGoThroughPipe_down = false;

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
	void DecreaseSpeed();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> * coObjects, vector <LPGAMEOBJECT>* coItem, vector <LPGAMEOBJECT>* listEffect);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void isDamaged();
	void StartUntouchable();

	void StartTimeFly();

	void Reset();
	void CollideWithItem(vector<LPGAMEOBJECT>* Item);

	void ToRight();
	void ToLeft();


	// HUD
	vector<int> cards;
	int GetCoin() { return this->coin; }
	void SetCoin(int x) { this->coin = x; }
	void AddCoin() { this->coin++; }

	int GetScore() { return  this->score; }
	void SetScore(int x) { this->score = x; }
	void AddScore(int x) { this->score += x; }
};