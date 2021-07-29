#pragma once
#include "Enemy.h"
#include "Timer.h"

#define KOOPAS_WALKING_SPEED	0.03f;
#define KOOPA_JUMP_SPEED		0.16f
#define KOOPA_RUNNING_SPEED		0.2f

#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			27
#define KOOPAS_BBOX_SHELL_WIDTH		18

#define KOOPAS_STATE_FLYING				0		// isWing = 1
#define KOOPAS_STATE_WALKING			100
#define KOOPAS_STATE_SHELL_IDLE			200
#define KOOPAS_STATE_SHELL_RUNNING		300
#define KOOPAS_STATE_SHAKING			400

#define KOOPAS_ANI_RED_WALKING_RIGHT		0
#define KOOPAS_ANI_RED_WALKING_LEFT			1
#define KOOPAS_ANI_RED_SHELL_DOWN_IDLE		2 // Shell khi bi damage (jump on head)
#define KOOPAS_ANI_RED_SHELL_UP_IDLE		3 // Shell bi ngua len khi bi attack (tail)
#define KOOPAS_ANI_RED_SHELL_DOWN_RUN		4	
#define KOOPAS_ANI_RED_SHELL_UP_RUN			5
#define KOOPAS_ANI_RED_SHELL_DOWN_SHAKING	6	
#define KOOPAS_ANI_RED_SHELL_UP_SHAKING		7	

#define KOOPAS_ANI_GREEN_FLYING_RIGHT		0
#define KOOPAS_ANI_GREEN_FLYING_LEFT		1
#define KOOPAS_ANI_GREEN_WALKING_RIGHT		2
#define KOOPAS_ANI_GREEN_WALKING_LEFT		3
#define KOOPAS_ANI_GREEN_SHELL_DOWN_IDLE	4 // Shell khi bi damage (jump on head)
#define KOOPAS_ANI_GREEN_SHELL_UP_IDLE		5 // Shell bi ngua len khi bi attack (tail)
#define KOOPAS_ANI_GREEN_SHELL_DOWN_RUN		6	
#define KOOPAS_ANI_GREEN_SHELL_UP_RUN		7 
#define KOOPAS_ANI_GREEN_SHELL_DOWN_SHAKING	8	
#define KOOPAS_ANI_GREEN_SHELL_UP_SHAKING	9	

#define KOOPA_TIME_RELIFE			1000 // thoi gian hoi sinh
#define KOOPA_TIME_SHELL_IDLE		8000 // thoi gian shell dung yen

// KoopaType
#define KOOPAS_TYPE_RED		0	// ani_set_id: 4
#define KOOPAS_TYPE_GREEN	1	// ani_set_id: 5


class CKoopas : public Enemy
{
	int KoopaType;
	
	D3DXVECTOR2 start = { -1,-1 }; // (x,y) bat dau di chuyen: red, green
	D3DXVECTOR2 end = { -1,-1 }; // (x,y) de quay dau: red

public:
	bool isOnGround;
	bool isShellUp; // check shell up? 
	bool isWing; // 0: non -> state walk || 1: wing ->state jump fly
	bool isBeingHeld = false; // check mario co dang holding shell
	bool isRelife = false;
	
	Timer* idleTimer = new Timer(KOOPA_TIME_SHELL_IDLE);
	// before relife, koopa shaking ->3s
	Timer* startRelifeTimer = new Timer(KOOPA_TIME_SHELL_IDLE - KOOPA_TIME_RELIFE);

	//CKoopas();
	CKoopas(int KoopaType, bool isWing,  float x, float y);
	~CKoopas() {}

	void SetKoopaType(int type) { this->KoopaType = type; }
	int GetKoopaType() { return this->KoopaType; }

	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};