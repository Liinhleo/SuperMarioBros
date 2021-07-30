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

void Enemy::damageOnTop() {
	SetState(ENEMY_STATE_DAMAGE);
}

void Enemy::damageByWeapon() {
	if (this->GetType()== ObjectType::PIRANHA_FLOWER
		|| this->GetType()== ObjectType::FIRE_FLOWER)
		SetState(STATE_DESTROYED);
	else
		SetState(ENEMY_STATE_DIE_BY_ATTACK);
}

