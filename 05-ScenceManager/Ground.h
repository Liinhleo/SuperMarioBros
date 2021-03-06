#pragma once
#include "GameObject.h"
#include "define.h"

class CGround : public CGameObject
{
	int width;
	int height;
public:
	bool isInteract; // isInteract -> 1: color box -> cho phep di xuyen

	CGround(int w, int h, bool isInteract);
	~CGround();

	int GetWidth() { return this->width;}
	int GetHeight() { return this->height; }
	
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};