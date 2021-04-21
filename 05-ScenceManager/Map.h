#pragma once
#include "Sprites.h"

// ~ A tile map == An Animation Frame = Sprite (sprite_id, left, top, right, bottom, tex_id)

class CTileMap {

	LPSPRITE sprite;

public:
	CTileMap() { this->sprite = sprite; };
	CTileMap(LPSPRITE sprite);
	LPSPRITE GetSprite();
};
typedef CTileMap* LPTILEMAP;


/*
* A map == array 2D store tilemap to create a map (~ ) 
* ~~~ ANIMATION  
* Ex: ani == WALKING_RIGHT == 3 animationFrames --> array 1D 
*/

class CMap
{
	int mapID;
	int widthMap;
	int heightMap;
	vector<vector<LPTILEMAP>> tiles; // arr 2D of tile

public:
	CMap() { this->mapID = mapID; }
	int GetWidthMap();
	void Add(int spriteId);
	void Draw(float x, float y, int alpha = 255);
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
	void Add(int id, LPMAP map);
	LPMAP Get(int id);
	void Clear();

	static CMaps* GetInstance();
};
