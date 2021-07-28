#pragma once
#include "Game.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Map.h"
#include "Camera.h"
#include "Hud.h"
#include "Grid.h"

#define PLAY_TIME			300
#define MINISEC_PER_SEC		1000

/*
* Play Scene have 1 player (MARIO) , objects (static, dynamic) and 1 MAP 
*/
class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 
	LPMAP map;
	LPCAMERA cam = nullptr;
	LPHUD hud;

	LPGRID gridMoving; 
	LPGRID gridStatic;

	vector<LPGAMEOBJECT> listObjects; // chua tat ca object except item + effect
	vector<LPGAMEOBJECT> listItems;	  // list item
	vector<LPGAMEOBJECT> listEffects;	  // list effect

	vector<LPGAMEOBJECT> listStatic;
	vector<LPGAMEOBJECT> listMoving;
	vector<LPGAMEOBJECT> listGrid; // chua cac doi tuong can dua vao grid

	Timer* playTime = new Timer(PLAY_TIME * MINISEC_PER_SEC);
	int remainingTime = 0;

	// DOC FILE
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line); // Read array 2D of TileMap ~ matrix numCol x numRow

public: 
	CPlayScene(int id, LPCWSTR filePath);

	void GetObjectToGrid();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

