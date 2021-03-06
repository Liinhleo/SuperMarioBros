#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 200);
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects 
	
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects, 
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy,
	LPGAMEOBJECT& objx, 
	LPGAMEOBJECT& objy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT coEvent = coEvents[i];

		if (coEvent->t < min_tx && coEvent->nx != 0) {
			min_tx = coEvent->t; nx = coEvent->nx; min_ix = i; rdx = coEvent->dx; objx = coEvent->obj;
		}

		if (coEvent->t < min_ty && coEvent->ny != 0) {
			min_ty = coEvent->t; ny = coEvent->ny; min_iy = i; rdy = coEvent->dy; objy = coEvent->obj;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT coEvent = coEvents[i];

		if (coEvent->t < min_tx && coEvent->nx != 0) {
			min_tx = coEvent->t; nx = coEvent->nx; min_ix = i; rdx = coEvent->dx;
		}

		if (coEvent->t < min_ty  && coEvent->ny != 0) {
			min_ty = coEvent->t; ny = coEvent->ny; min_iy = i; rdy = coEvent->dy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}


bool CGameObject::isOutOfCam() {
	CGame* game = CGame::GetInstance();
	return !(x >= game->GetCamPosX() && x < game->GetCamPosX() + (SCREEN_WIDTH) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}

bool CGameObject::CheckAABB(float left_a, float top_a, float right_a, float bottom_a, float left_b, float top_b, float right_b, float bottom_b){
	return (left_a < right_b && right_a > left_b && top_a < bottom_b && bottom_a > top_b);
}

bool CGameObject::isAABB(LPGAMEOBJECT object)
{
	float left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b;
	this->GetBoundingBox(left_a, top_a, right_a, bottom_a);
	object->GetBoundingBox(left_b, top_b, right_b, bottom_b);
	return CheckAABB(left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b);
}

bool CGameObject::IsCollidingWithObject(LPGAMEOBJECT object)
{
	if (!object)
		return false;
	if (this->isAABB(object))
		return true;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	delete e;
	return res;
}

bool CGameObject::IsCollidingWithObjectNy_1(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->ny == -1)
			return true;
	delete e;
	return false;
}

bool CGameObject::IsCollidingWithObjectNy1(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->ny == 1)
			return true;
	delete e;
	return false;
}
bool CGameObject::IsCollidingWithObjectNy(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->ny != 0)
			return true;
	delete e;
	return false;
}

bool CGameObject::IsCollidingWithObjectNx_1(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->nx == -1)
			return true;
	delete e;
	return false;
}

bool CGameObject::IsCollidingWithObjectNx1(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);

	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->nx == 1)
			return true;
	delete e;
	return false;
}

//WORLD MAP
bool CGameObject::IsCollidingWithObjectNx(LPGAMEOBJECT object) {
	if (!object)
		return false;
	LPCOLLISIONEVENT e = SweptAABBEx(object);
	//collision condition
	bool res = e->t > 0 && e->t <= 1.0f;
	if (res)
		if (e->nx != 0)
			return true;
	delete e;
	return false;
}


CGameObject::~CGameObject()
{

}