#pragma once
#include "AnimationFrame.h"
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);

	void Render(float x, float y, int alpha = 255);
};

typedef CAnimation* LPANIMATION;
