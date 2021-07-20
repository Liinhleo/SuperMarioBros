#include "Enemy.h"

/* ATTACK ON TOP (MARIO JUMP ON HEAD)
* koopa: damage -> shell down
* goomba: damage -> destroy
* ninja: damage -> lat ng va bay khoi ground -> die
*/

/*
* ATACK BY WEAPON
* Plant: attack by tail, fire -> destroy
* koopa: attack by tail -> shell up || attack by fire -> destroy
* goomba: attack by tail -> lat ng va bay khoi ground -> die || attack by fire -> destroy
* ninja: attack by tail  -> lat ng va bay khoi ground -> die || attack by fire -> destroy
*/

void Enemy::attackOnTop() {
	SetState(ENEMY_STATE_DAMAGE);
	timeDestroy->Start();
}

void Enemy::attackByWeapon() {
	if (this->GetType()== ObjectType::PIRANHA_FLOWER|| this->GetType()== ObjectType::FIRE_FLOWER)
		SetState(ENEMY_STATE_DESTROY);
	else
		SetState(ENEMY_STATE_DIE_BY_ATTACK);
}

