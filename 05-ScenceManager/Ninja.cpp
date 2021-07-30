#include "Ninja.h"
#include "Ground.h"
#include "Mario.h"


Ninja::Ninja() {
	this->type = ObjectType::NINJA;
	this->start_x = x;
	this->start_y = y;

	SetState(NINJA_STATE_WALKING);
	isAttacking = false;
	timeWaitAttack->Start();

}

void Ninja::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == STATE_DESTROYED || state == ENEMY_STATE_DAMAGE || state == ENEMY_STATE_DIE_BY_ATTACK)
		left = top = right = bottom = 0;
	else
	{
		top = y;
		left = x;
		right = x + NINJA_BBOX_WIDTH;
		bottom = y + NINJA_BBOX_HEIGHT;
	}
}

void Ninja::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	
	vy += MARIO_GRAVITY * dt;
	
	DebugOut(L"Ninja POS=== %f \n", start_x);
	DebugOut(L"Ninja POS=== %f \n", start_y);

	//float m_x = CMario::GetInstance()->x;
	//// Lay huong cua mario de xet huong cua Ninja
	////if (CMario.x < this->start_x) { //mario o ben trai
	////	this->nx = 1;
	////}
	////else {
	////	this->nx = -1;
	////}

	////if (listBoomerang.size() == 0) {
	////	if (isAttacking && timeWaitAttack->GetStartTime() == 0) {
	////		timeWaitAttack->Start();
	////		isAttacking = false;
	////	}
	////}

	//// XET VI TRI DI CHUYEN QUA LAI LIMIT POSITION
	//if (x > start_x + LIMIT_POSITION) 
	//{
	//	x = start_x + LIMIT_POSITION;
	//	SetState(NINJA_STATE_IDLE);
	//}
	//if (x < start_x)
	//{
	//	x = start_x;
	//	SetState(NINJA_STATE_IDLE);
	//}

	//// XET THOI GIAN CHANGE STATE IDLE -> WALKING
	//if (timeIdle->GetStartTime() && timeIdle->IsTimeUp()) {
	//	timeIdle->Stop();
	//	SetState(NINJA_STATE_WALKING);
	//}

	//// XET THOI GIAN ATTACK WITH BOOMERANG
	//if (timeWaitAttack->GetStartTime() && timeWaitAttack->IsTimeUp()) { 
	//	isAttacking = true;
	//	timeWaitAttack->Stop();

	//	if (listBoomerang.size() < 2) {
	//		if (player.x > this->x)
	//			player.x = 1;
	//		else
	//			player.x = 0;
	//		// DANH THEO HUONG CO MARIO
	//		listBoomerang.push_back(CreateBoomerang(this->x, this->y, player.x));
	//	}

	//	// XOA BOOMERANG NEU VA CHAM NINJA
	//	for (size_t i = 0; i < listBoomerang.size(); i++)
	//	{
	//		listBoomerang[i]->Update(dt, coObjects);
	//		if (this->IsCollidingWithObjectNx(listBoomerang[i]) 
	//			|| this->IsCollidingWithObjectNy(listBoomerang[i]))

	//			listBoomerang[i]->SetState(STATE_DESTROYED);
	//	}
	//}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.25f;

		if (ny != 0) {
			vy = 0;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == ObjectType::GROUND) {
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0) {
					if (ground->isInteract)	{
						x += dx;
					}
					else
						vx = -vx;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	
}
void Ninja::Render() {
	if (state == STATE_DESTROYED)
		return;
	if (state == ENEMY_STATE_DAMAGE || state == ENEMY_STATE_DIE_BY_ATTACK) {
		ani = NINJA_ANI_DIE;
	}
	if (state == NINJA_STATE_WALKING) {
		if (nx > 0)	ani = NINJA_ANI_WALKING_RIGHT;
		else ani = NINJA_ANI_WALKING_LEFT;
	}
	if (state == NINJA_STATE_IDLE) {
		if (nx > 0)	ani = NINJA_ANI_IDLE_RIGHT;
		else ani = NINJA_ANI_WALKING_LEFT;
	}

	for (size_t i = 0; i < listBoomerang.size(); i++)
		listBoomerang[i]->Render();

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Ninja::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_DESTROYED: // bullet
		vx = 0;
		vy = 0;
		break;
	case ENEMY_STATE_DAMAGE:
	case ENEMY_STATE_DIE_BY_ATTACK:
		vy = -0.15f;
		vx = nx * 0.05f;
		break;

	case NINJA_STATE_IDLE:
		vx = 0;
		timeIdle->Start();
		break;
	case NINJA_STATE_WALKING:
		vx = -0.05f;
		break;
	}
}