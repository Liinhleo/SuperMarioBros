#pragma once
#include "Sprites.h"
#include "define.h"

class CMap
{
	int mapID;
	LPCWSTR matrix_path;
	int widthMap;
	int heightMap;

	//vector<vector<int>> tileMaps; // arr 2D of tile maps 
	
	int tileMaps[300][300];
	int numCol;
	int numRow;

public:
	CMap(int mapID, LPCWSTR matrix_path, int widthMap, int heightMap);
	//CMap();

	int getMapID();
	LPCWSTR getMatrixPath();
	int getWidthMap();
	int getHeighthMap();
	int getNumCol();
	int getNumRow();

	void LoadMap();
	void Render();
};
typedef CMap* LPMAP;


/*
* MAPS ==  unordered list store all map 
* ~~~ ANIMATIONS
*/

class CMaps
{
	static CMaps* __instance;
	unordered_map<int, LPMAP> maps;

public:
	CMaps() {};
	void Add(int id, LPMAP map);
	LPMAP Get(int id);
	void Clear();

	static CMaps* GetInstance();
};
typedef CMaps* LPMAPS;
