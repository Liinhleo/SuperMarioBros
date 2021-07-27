#include "Portal.h"
#define POINT_DISTANCE	6

CPortal::CPortal(bool hasPortal, int sceneID, bool l, bool r, bool u, bool d)
{
	this->type = ObjectType::PORTAL;
	this->hasPortal = hasPortal;
	this->scene_id = sceneID;
	this->canMoveLeft = l;
	this->canMoveRight = r;
	this->canMoveUp = u;
	this->canMoveDown = d;
}


void CPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 10;
	top = y + 10;
	right = x + POINT_DISTANCE;
	bottom = y + POINT_DISTANCE;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CGameObject::Update(dt);
}

void CPortal::SetState(int state) {
	CGameObject::SetState(state);
}

void CPortal::Render() {
	RenderBoundingBox();
}