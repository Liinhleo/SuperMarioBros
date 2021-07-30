#include "Pipe.h"

Pipe::Pipe(int type, bool hasPortal, D3DXVECTOR2 des, bool dir) {
	
	this->type = ObjectType::PIPE;
	this->PipeType = type;
	this->hasPortal = hasPortal;
	this->destination.x = des.x;
	this->destination.y = des.y;
	this->direction = dir;
}

void Pipe::Render() {
	if (PipeType == PIPE_LONG) ani = PIPE_ANI_LONG;
	else {
		if (!direction) ani = PIPE_ANI_BLACK_UP; // direction = 1: pipe huong len -> hidden map
		else
			return;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
void Pipe::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + PIPE_BBOX_WIDTH;
	if (PipeType == PIPE_LONG) {
		bottom = top + PIPE_BBOX_HEIGHT_LONG;
	}
	else {
		bottom = top + PIPE_BBOX_HEIGHT_SHORT;
	}
}
