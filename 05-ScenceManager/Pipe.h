#pragma once
#include "GameObject.h"

#define PIPE_LONG   1
#define PIPE_SHORT  2

#define PIPE_ANI_LONG		0
#define PIPE_ANI_SHORT		1
#define PIPE_ANI_BLACK_UP	2 // black pipe in hidden map

#define PIPE_BBOX_HEIGHT_LONG	48	
#define PIPE_BBOX_HEIGHT_SHORT	32
#define PIPE_BBOX_WIDTH			32
	


class Pipe :  public CGameObject
{
	int PipeType;
	bool hasPortal; // 1: true -> destination

	D3DXVECTOR2 destination; // vi tri 
	bool direction; // huong cua ong (0: down/ 1: up)

public:
	Pipe(int type, bool hasPortal, D3DXVECTOR2 des, bool dir);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	
	D3DXVECTOR2 GetDestination() { return destination; }
	bool GetDirection() { return this->direction; }
	bool IsHasPortal() { return this->hasPortal; }

};

