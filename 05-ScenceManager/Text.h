#pragma once
#include "Sprites.h"
#include "define.h"
#include "string"
#include "Textures.h"
#include "Game.h"
#include "Utils.h"
#include "map"
#include "Define.h"

#define CHAR_SIZE	8

//text
#define No_0	2110
#define No_1	2111
#define No_2	2112
#define No_3	2113
#define No_4	2114
#define No_5	2115
#define No_6	2116
#define No_7	2117
#define No_8	2118
#define No_9	2119


class Text
{
	unordered_map<char, int> spriteID{ 
		{ '0', No_0 }, 
		{ '1', No_1 }, 
		{ '2', No_2 }, 
		{ '3', No_3 }, 
		{ '4', No_4 }, 
		{ '5', No_5 }, 
		{ '6', No_6 }, 
		{ '7', No_7 }, 
		{ '8', No_8 }, 
		{ '9', No_9 }};

	LPSPRITE character;

public:
	Text() = default; 

	void RenderCharacter(char c, D3DXVECTOR2 position);
	void Render(char* s, D3DXVECTOR2 position);
}; 
typedef Text* LPTEXT;


