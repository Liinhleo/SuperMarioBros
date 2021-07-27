#include "Bullet.h"
#include "Brick.h"
#include "Ground.h"
#include "Enemy.h"
#include "Pswitch.h"
#include "Mario.h"

Bullet::Bullet(D3DXVECTOR2 position, int nx) {
	x = position.x;
	y = position.y;
	this->nx = nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	CGameObject::Update(dt);

	this->vy += BULLET_GRAVITY * dt;	// Simple fall down
	this->vx = nx * BULLET_SPEED_X; 

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.4f;

		if (ny < 0) // va cham ground theo phuong y
		{
			this->vy = -BULLET_SPEED_Y; // nhay tung tung len
		}

		for (UINT i = 0; i < coEventsResult.size(); i++) 
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == ObjectType::GROUND) {
				CGround* ground = dynamic_cast<CGround*>(e->obj);
				if (e->nx != 0){ // va cham theo phuong x
					if (ground->isInteract) {
						this->x += dx; // xuyen qua dc
					}

					else
						this->state = STATE_DESTROYED; // va cham-> not render bullet
				}
			}

			else if (e->obj->GetType() == ObjectType::BRICK || e->obj->GetType() == ObjectType::PIPE) {
				if (e->nx != 0){ // va cham theo phuong x
					//this->state = STATE_DESTROYED; // va cham-> not render bullet

					// TEST THU P_SWITCH
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (brick->GetBrickType() != BRICK_BROKEN && brick->GetTypeItem() == CONTAIN_PSWITCH) {
						CGameObject* obj = new Pswitch(brick->start_x, brick->start_y - BRICK_BBOX_SIZE);
						coObjects->push_back(obj);
						brick->SetBrickType(BRICK_BROKEN);
					}
					this->state = STATE_DESTROYED; // va cham-> not render bullet

				}
			}
			else if (e->obj->GetType() == ObjectType::PIRANHA_FLOWER || e->obj->GetType() == ObjectType::FIRE_FLOWER) {
				e->obj->SetState(STATE_DESTROYED);
				state = STATE_DESTROYED;
				CMario::GetInstance()->AddScore(100);
			}
		
			else {
					if (e->nx != 0 || e->ny != 0) { // va cham theo phuong x, y
						Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
						enemy->nx = this->nx;
						enemy->SetState(STATE_DESTROYED);
						CMario::GetInstance()->AddScore(100);
					}
					this->state = STATE_DESTROYED; // va cham-> not render bullet

			}

		}
	}
}

void Bullet::Render() {
	int alpha = 255;
	animation_set->at(0)->Render(x, y, alpha);
	//RenderBoundingBox();
}

void Bullet::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}