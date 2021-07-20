#include "Map.h"
#include "Utils.h"
#include <string> 
#include <fstream>
#include <sstream>
#include "Textures.h"
#include "Game.h"

// ANIMATION 
CMap::CMap(int mapID, LPCWSTR matrix_path, int widthMap, int heightMap) {
	this->mapID = mapID;
	this->matrix_path = matrix_path;
	this->widthMap = widthMap;
	this->heightMap = heightMap;

	getNumRow();
	getNumCol();
	LoadMap();
}

int CMap::getMapID() {
	return mapID;
}
int CMap::getWidthMap() {
	return widthMap;
}
int CMap::getHeighthMap() {
	return heightMap;
}

LPCWSTR CMap::getMatrixPath() {
	return matrix_path;
}

int CMap::getNumCol() {
	this->numCol = widthMap / TILE_SIZE;
	return numCol;
}
int CMap::getNumRow() {
	this->numRow = heightMap / TILE_SIZE;
	return numRow;
}

void CMap::LoadMap() {
	DebugOut(L"[INFO] Start loading matrix map from : %s \n", matrix_path);
	CSprites* sprites = CSprites::GetInstance();

	ifstream f;
	f.open(matrix_path, ios::in);
	if (f.fail()) {
		DebugOut(L"[INFO] loading matrix map failed with ID=  %s \n", mapID);
		f.close();
		return;
	}
	//read info from file
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++){
			f >> tileMaps[i][j];
			int a = tileMaps[i][j];
			sprites->Get(tileMaps[i][j]);
		}
	}		
	f.close();
	DebugOut(L"[INFO] Done loading map resources %s\n", matrix_path);

}


void CMap::Render()
{
	int col_begin = CGame::GetInstance()->GetCamPosX() / TILE_SIZE;
	int col_end = col_begin + (SCREEN_WIDTH / TILE_SIZE) + 1; // col_end + 1 
	
	CGame* game = CGame::GetInstance();

	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j <= col_end; j++) {
			int x = TILE_SIZE * (j - col_begin) - (int)game->GetCamPosX() % TILE_SIZE + (int)game->GetCamPosX() ;
			int y = TILE_SIZE * i;

			CSprites::GetInstance()->Get(tileMaps[i][j])->Draw(x, y, 255);
			//CSprites::GetInstance()->Get(tileMaps[6][2])->Draw(x, y, 255);
		}
	}
}

// =========== CMaps == CAnimations = Store all map ================ 

CMaps* CMaps::__instance = NULL;

CMaps* CMaps::GetInstance()
{
	if (__instance == NULL) __instance = new CMaps();
	return __instance;
}

void CMaps::Add(int id, LPMAP map)
{
	maps[id] = map;
}

LPMAP CMaps::Get(int id)
{
	LPMAP map = maps[id];
	if (map == NULL)
		DebugOut(L"[ERROR] Failed to find map id: %d\n", id);
	return map;
}

//void CMaps::Clear()
//{
//	//for (auto x : maps)
//	//{
//	//	LPMAP map = x.second;
//	//	delete map;
//	//}
//
//	maps.clear();
//}

