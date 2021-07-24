#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Ground.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "PiranhaFlower.h"
#include "Camera.h"

/*
* Play Scene have 1 player (MARIO) , objects (static, dynamic) and 1 MAP 
*/
class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 
	LPMAP map;
	LPCAMERA cam = nullptr;

	vector<LPGAMEOBJECT> listObjects; // chua tat ca object except item + effect
	vector<LPGAMEOBJECT> listItems;	  // list item
	vector<LPGAMEOBJECT> effects;	  // list effect


	vector<LPGAMEOBJECT> listStatic; // include: Ground, brick, pipe, portal

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line); // Read array 2D of TileMap ~ matrix numCol x numRow

public: 
	CPlayScene(int id, LPCWSTR filePath);

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

