#include "Koopas.h"
#include "Ground.h"
#include "Brick.h"

CKoopas::CKoopas() {
	this->type = ObjectType::KOOPA;
	SetState(KOOPAS_STATE_SHELL_IDLE);
	
}

CKoopas::CKoopas(int KoopaType, bool isWing, float x, float y)
{
	this->type = ObjectType::KOOPA;
	this->KoopaType = KoopaType;
	
	this->start_x = x;
	this->start_y = y;

	if (KoopaType == KOOPAS_TYPE_RED) {
		this->SetAnimationSet(CAnimationSets::GetInstance()->Get(4));
		SetState(KOOPAS_STATE_WALKING);

	}
	else {

		this->SetAnimationSet(CAnimationSets::GetInstance()->Get(5));
		if (isWing==true) {
			SetState(KOOPAS_STATE_FLYING);
		}
		else{
			SetState(KOOPAS_STATE_WALKING);
		}
	}


}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += MARIO_GRAVITY * dt;

	if (state == ENEMY_STATE_DAMAGE) {
		if (isWing) {
			isWing = false;
			SetState(KOOPAS_STATE_WALKING);
		}
		else if (!isWing) {
			SetState(KOOPAS_STATE_SHELL_IDLE);
		}
	}

	// XU LY VA CHAM
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0) // no collision
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

		if (ny != 0)
		{
			vy = 0;
			if (ny < 0) // va cham ground
			{
				isOnGround = true;
				if (isWing) {
					vy = -KOOPA_JUMP_SPEED;
				}
			}
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == ObjectType::GROUND) {
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0) { // va cham theo phuong x voi color box
					if (ground->isInteract) {
						x += dx; //di xuyen qua
					}
					else {
						vx = -vx; // doi huong
					}
				}
			}

			else if (e->obj->GetType() == ObjectType::BRICK || e->obj->GetType() == ObjectType::PIPE) {
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				if (e->nx != 0 || e->ny != 0) {

					// XU LY GLASS BRICK WHEN STATE HIDDEN
					if (brick->GetBrickType() == BRICK_GLASS && brick->GetState() == BRICK_STATE_HIDDEN) {
						x += dx;
						y += dy;
					}

					vx = -vx; // doi huong
					// CHUA XU LY KOOPA DI QUA LAI 1 DOAN BRICK
				}
			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int KType = GetKoopaType();

	if (state == STATE_DESTROYED)
		return;

	if (GetKoopaType() == KOOPAS_TYPE_RED) {
		switch (state) {
		case ENEMY_STATE_DAMAGE: // mario jump on
			ani = KOOPAS_ANI_RED_SHELL_DOWN_IDLE;
			break;

		case ENEMY_STATE_DIE_BY_ATTACK:
			ani = KOOPAS_ANI_RED_SHELL_UP_IDLE; // bi lat nguoc
			break;

		case KOOPAS_STATE_SHELL_RUNNING:
			if (isShellUp) ani = KOOPAS_ANI_RED_SHELL_UP_RUN;
			else ani = KOOPAS_ANI_RED_SHELL_DOWN_RUN;
			break;

		case KOOPAS_STATE_SHAKING:
			if (isShellUp) ani = KOOPAS_ANI_RED_SHELL_UP_SHAKING;
			else ani = KOOPAS_ANI_RED_SHELL_DOWN_SHAKING;
			break;

		case KOOPAS_STATE_WALKING:
			if (vx > 0) ani = KOOPAS_ANI_RED_WALKING_RIGHT;
			else ani = KOOPAS_ANI_RED_WALKING_LEFT;
			break;

		}
	}
	else {
		switch (state) {
		
		case ENEMY_STATE_DAMAGE: // mario jump on
			ani = KOOPAS_ANI_GREEN_SHELL_DOWN_IDLE;
			break;

		case ENEMY_STATE_DIE_BY_ATTACK:
			ani = KOOPAS_ANI_GREEN_SHELL_UP_IDLE; // bi lat nguoc
			break;
		case KOOPAS_STATE_FLYING:
			if (vx > 0) ani = KOOPAS_ANI_GREEN_FLYING_RIGHT;
			else ani = KOOPAS_ANI_GREEN_FLYING_LEFT;
			break;

		case KOOPAS_STATE_SHELL_RUNNING:
			if (isShellUp) ani = KOOPAS_ANI_GREEN_SHELL_UP_RUN;
			else ani = KOOPAS_ANI_GREEN_SHELL_DOWN_RUN;
			break;

		case KOOPAS_STATE_SHAKING:
			if (isShellUp) ani = KOOPAS_ANI_GREEN_SHELL_UP_SHAKING;
			else ani = KOOPAS_ANI_GREEN_SHELL_DOWN_SHAKING;
			break;

		case KOOPAS_STATE_WALKING:
			if (vx > 0) ani = KOOPAS_ANI_GREEN_WALKING_RIGHT;
			else ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
			break;

		}
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	// xu ly state bi thuong cua Koopa
	case STATE_DESTROYED: // bullet
		vx = 0;
		vy = 0;
		break;
	case ENEMY_STATE_DAMAGE: // mario jump on head
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_WIDTH + 1;
		vx = 0;
		vy = 0;
		break;
	case ENEMY_STATE_DIE_BY_ATTACK: // tail
		vy = -KOOPA_JUMP_SPEED;
		if (nx > 0) {
			vx = KOOPAS_WALKING_SPEED;
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	// xu ly cac state khac
	case KOOPAS_STATE_FLYING:
		isWing = true;
		isOnGround = false;
		vy = -KOOPAS_WALKING_SPEED ;
		vx = -KOOPA_JUMP_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		isWing = false;
		isOnGround = true;
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SHELL_IDLE:
		idleTimer->Start(); // bd tinh gio dung yen
		vx = 0;
		break;
	case KOOPAS_STATE_SHELL_RUNNING:
		vx = -nx * KOOPA_RUNNING_SPEED;
		isOnGround = true;
		break;
	case KOOPAS_STATE_SHAKING:
		vx = 0;
		isOnGround = true;
		break;

	}
}
//#define KOOPAS_STATE_SHELL_IDLE			200
//#define KOOPAS_STATE_SHELL_RUNNING		300
//#define KOOPAS_STATE_SHAKING			400

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	bottom = y + KOOPAS_BBOX_HEIGHT;

	if (state == ENEMY_STATE_DAMAGE) {
		right = x + KOOPAS_BBOX_WIDTH + 2; // shell width = 18
	}
	else {
		right = x + KOOPAS_BBOX_WIDTH;
	}
}
