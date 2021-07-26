#include "Grid.h"
#include "define.h"

// map 1-1 { width: 3360  || height: 432 }  => { col : 26.25 ~> 27 ||  row:  4.5 ~> 5 }
// map 1-3 { width: 3584  || height: 432 }  => { col : 28 ||  row:  4.5 ~> 5 }

Grid::Grid(int map_width, int map_height) {
	
	this->map_width = map_width; 
	this->map_height = map_height;

	this->num_col = int((this->map_width/ BRICK_BBOX_SIZE)/ CELL_WIDTH) + 1;
	this->num_row = int((this->map_height / BRICK_BBOX_SIZE) / CELL_HEIGHT) + 1;

	cells.resize(num_row);
	for (int i = 0; i < num_row; i++)
		cells[i].resize(num_col);

	Clear(num_row, num_col);
}

void Grid::Clear(int num_row, int nums_col) {
	for (int i = 0; i < num_row; i++)
		for (int j = 0; j < num_col; j++)
			cells[i][j].clear();
}

// update object (CASE: enemy can move out grid)
void Grid::UpdateGrid(vector<LPGAMEOBJECT> objects) {
	Clear(num_row, num_col);

	for (int i = 0; i < objects.size(); i++)
	{
		float l, t, r, b;
		objects[i]->GetBoundingBox(l, t, r, b);
		// objects[i] rot khoi map
		if (b > (map_height * BRICK_BBOX_SIZE)
			|| l < 0
			|| r > (map_width * BRICK_BBOX_SIZE))
		{
			objects[i]->SetState(STATE_DESTROYED);
		}
		int top = int(t / (CELL_HEIGHT * BRICK_BBOX_SIZE));
		int bottom = ceil(b / (CELL_HEIGHT * BRICK_BBOX_SIZE));
		int left = int(l / (CELL_WIDTH * BRICK_BBOX_SIZE));
		int right = ceil(r / (CELL_WIDTH * BRICK_BBOX_SIZE));

		if (objects[i]->GetState() != STATE_DESTROYED) {
			for (int row = top; row < bottom; row++)
				for (int col = left; col < right; col++)
					cells[row][col].push_back(objects[i]);
		}
	}
}

void Grid::ResetCamGrid(vector<LPGAMEOBJECT>& objects) {
	for (int i = 0; i < objects.size(); i++)
		objects[i]->isInCam = false;
}

void Grid::GetObjFromGrid(vector<LPGAMEOBJECT>& objects) {

	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / (CELL_WIDTH * BRICK_BBOX_SIZE));
	int lastCol = ceil((CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH) / (CELL_WIDTH * BRICK_BBOX_SIZE));
	
	DebugOut(L"first Col 1 -======== = %d \n", firstCol);
	DebugOut(L"last Col 1 -======== = %d \n", lastCol);

	// check firstCol co nam o dau map? ( 1 : 0) 
	firstCol = (firstCol >= 1) ? firstCol - 1 : firstCol;
	lastCol = (lastCol < num_col) ? lastCol + 1 : lastCol;

	DebugOut(L"first Col 2 -======== = %d \n", firstCol);
	DebugOut(L"last Col 2 -======== = %d \n", lastCol);

	for (int i = 0; i < num_row; i++) {
		for (int j = firstCol; j < lastCol; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (!cells[i][j][k]->isInCam 
					&& cells[i][j][k]->GetState() != STATE_DESTROYED)
				{
					cells[i][j][k]->isInCam = true;
					objects.push_back(cells[i][j][k]);
				}
			}
		}
	}

}

void Grid::pushObjectIntoGrid(LPGAMEOBJECT obj, int row, int col) {
	
	cells[row][col].push_back(obj);
}


void Grid::pushNewObjIntoGrid(LPGAMEOBJECT obj) {
	
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	int top = int(t / (CELL_HEIGHT * BRICK_BBOX_SIZE));
	int bottom = ceil(b / (CELL_HEIGHT * BRICK_BBOX_SIZE));
	int left = int(l / (CELL_WIDTH * BRICK_BBOX_SIZE));
	int right = ceil(r / (CELL_WIDTH * BRICK_BBOX_SIZE));

	if (obj->GetState() != STATE_DESTROYED) {
		for (int row = top; row < bottom; row++)
			for (int col = left; col < right; col++)
				cells[row][col].push_back(obj);
	}
}