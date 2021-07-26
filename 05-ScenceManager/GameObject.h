#pragma once
#include "define.h"
#include "AnimationSet.h"

#define STATE_DESTROYED		99

using namespace std;


class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	float start_x; // initial position of Object at scene
	float start_y; 

	int state;
	int type;
	int ani;
	
	bool isInCam; // check objs co nam trong camera?

	DWORD dt; 
	
	LPANIMATION_SET animation_set;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetPosX(float& x) { x = this->x; }
	void GetPosY(float& y) { y = this->y; }

	int GetType() { return this->type; }
	void SetType(int type) { this->type = type; }

	virtual void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void RenderBoundingBox();
	bool isOutOfCam();
	
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, 
		float& min_ty,
		float& nx, 
		float& ny, 
		float& rdx, 
		float& rdy, 
		LPGAMEOBJECT& objx, 
		LPGAMEOBJECT& objy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;

	bool CheckAABB(float left_a, float top_a, float right_a, float bottom_a, float left_b, float top_b, float right_b, float bottom_b);
	bool isAABB(LPGAMEOBJECT object);
	bool IsCollidingWithObject(LPGAMEOBJECT object);	
	bool IsCollidingWithObjectNx(LPGAMEOBJECT object);		// nx != 0
	bool IsCollidingWithObjectNx1(LPGAMEOBJECT object);		// nx = 1
	bool IsCollidingWithObjectNx_1(LPGAMEOBJECT object);	// nx = -1
	bool IsCollidingWithObjectNy(LPGAMEOBJECT object);		// ny != 0
	bool IsCollidingWithObjectNy1(LPGAMEOBJECT object);		// ny = 1
	bool IsCollidingWithObjectNy_1(LPGAMEOBJECT object);	// ny = -1

	~CGameObject();
};

