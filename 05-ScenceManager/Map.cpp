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
	//set size of map
	tileMaps.resize(numRow);
	for (int i = 0; i < numRow; i++)
		tileMaps[i].resize(numCol);	

	//read info from file
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++){
			f >> tileMaps[i][j];
			sprites->Get(tileMaps[i][j]);
		}
	}		
	f.close();
}


void CMap::Render()
{
	CSprites* sprites = CSprites::GetInstance();

	int firstCol = (int)CGame::GetInstance()->GetCamPosX() / TILE_SIZE;
	int lastCol = firstCol + (SCREEN_WIDTH / TILE_SIZE);
	
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++) {
			float x = TILE_SIZE * (j - firstCol) + CGame::GetInstance()->GetCamPosX() - (int)CGame::GetInstance()->GetCamPosX() % TILE_SIZE;
			float y = TILE_SIZE * i ;
			sprites->Get(tileMaps[i][j])->Draw(x, y);
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

void CMaps::Clear()
{
	//for (auto x : maps)
	//{
	//	LPMAP map = x.second;
	//	delete map;
	//}

	maps.clear();
}

