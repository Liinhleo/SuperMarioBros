#pragma once
#include "Game.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Map.h"
#include "Camera.h"
#include "Hud.h"

class GreenLand :
    public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 
	LPMAP map;
	LPCAMERA cam = nullptr;
	LPHUD hud;

	
	vector<LPGAMEOBJECT> listObjects; // chua tat ca object except item + effect

	// DOC FILE
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAPS(string line); // Read array 2D of TileMap ~ matrix numCol x numRow

public:
	GreenLand(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

};


class GreenLandKeyHandler : public CScenceKeyHandler
{
public:

	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	GreenLandKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
