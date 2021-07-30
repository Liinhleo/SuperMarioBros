#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Utils.h"
#include "Define.h"

#define CELL_WIDTH		8
#define CELL_HEIGHT		6

class Grid
{
	int map_width;
	int map_height;

	int num_col;
	int num_row;

	vector<vector<vector<LPGAMEOBJECT>>> cells;// {x,y,obj}

public:
	Grid(int map_width, int map_height);
	~Grid() {}

	void Clear(int num_row, int num_col);

	void UpdateGrid(vector<LPGAMEOBJECT> list);

	void ResetCamGrid(vector<LPGAMEOBJECT>& list);

	void GetObjFromGrid(vector<LPGAMEOBJECT>& list);
	void pushObjectIntoGrid(LPGAMEOBJECT obj, int row, int col);
	void pushNewObjIntoGrid(LPGAMEOBJECT obj);

}; 
typedef Grid* LPGRID;


