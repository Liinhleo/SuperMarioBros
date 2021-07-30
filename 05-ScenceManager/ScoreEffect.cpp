#include "ScoreEffect.h"
#include "Utils.h"

ScoreEffect::ScoreEffect(D3DXVECTOR2 position, int score)
{
	this->score = score;
	x = position.x;
	y = position.y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(42));
	vy = -SCORE_SPEED_Y;
	effectTimer->Start();
}

void ScoreEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void ScoreEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	if (effectTimer->IsTimeUp())
		state = STATE_DESTROYED;
}

void ScoreEffect::Render()
{
	if (score == 100)
		ani = 0;
	else if (score == 200)
		ani = 1;
	else if (score == 400)
		ani = 2;
	else if (score == 800)
		ani = 3;
	else if (score == 1000)
		ani = 4;
	else if (score == 2000)
		ani = 5;
	else if (score == 4000)
		ani = 6;
	else if (score == 8000)
		ani = 7;
	else ani = 8;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
