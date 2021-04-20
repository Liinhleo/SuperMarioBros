#pragma once
#include "Sprites.h"
/*
* an Animation Frame = Sprite + frame time  
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time);
	DWORD GetTime();
	LPSPRITE GetSprite();
};

typedef CAnimationFrame* LPANIMATION_FRAME;
