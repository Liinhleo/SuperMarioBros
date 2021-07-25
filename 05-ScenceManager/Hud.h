#pragma once
#include "Game.h"
#include "Mario.h"
#include "define.h"
#include "Text.h"
#include "SpeedBar.h"

class Hud
{
	LPANIMATION_SET board;
	LPSPRITE background;
	LPANIMATION_SET card;

	Text text;
	SpeedBar* speedBar = nullptr;

public:
	Hud();
	~Hud() {}

	void Render(D3DXVECTOR2 position, CMario* mario, int reamainingTime, int stage);
};
typedef Hud* LPHUD;

