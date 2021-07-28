#include "GreeenLand.h"
#include <iostream>
#include <fstream>
#include "GreenLandObject.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Define.h"

using namespace std;

GreenLand::GreenLand(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	key_handler = new GreenLandKeyHandler(this);
}

#pragma region PARSE FILE

void GreenLand::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void GreenLand::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

/*
	Parse a line in section [MAPS]
*/
void GreenLand::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());
	float hidden_start = strtof(tokens[4].c_str(), NULL);

	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap, hidden_start);
}

void GreenLand::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void GreenLand::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void GreenLand::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		{
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj = CMario::GetInstance();
			player = (CMario*)obj;
			hud = new Hud();

			// General object setup
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);

			DebugOut(L"[INFO] Player object created!\n");
			break;
		}
	case OBJECT_TYPE_PORTAL:
		{	
			int hasPortal = atof(tokens[4].c_str());
			int scene_id = atof(tokens[5].c_str());
			int left = atof(tokens[6].c_str());
			int right = atof(tokens[7].c_str());
			int up = atof(tokens[8].c_str());
			int down = atof(tokens[9].c_str());
			obj = new CPortal(hasPortal, scene_id, left, right, up, down);
			// General object setup
			obj->SetPosition(x, y);

			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

			obj->SetAnimationSet(ani_set);
			listObjects.push_back(obj);

		}
		break;
	case OBJECT_GREEN_LAND:
		{
			int type = atoi(tokens[4].c_str());
			obj = new GreenLandObject(type);

			// General object setup
			obj->SetPosition(x, y);

			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

			obj->SetAnimationSet(ani_set);
			listObjects.push_back(obj);
		}	
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// Khoi tao camera
	//cam = Camera::GetInstance();
}


void GreenLand::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[MAPS]") {
			section = SCENE_SECTION_TILE_MAP; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILE_MAP: _ParseSection_MAPS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

#pragma endregion

#pragma region UPDATE SCENE

void GreenLand::Update(DWORD dt)
{
	//mario
	player->Update(dt, &listObjects);

	for (size_t i = 0; i < listObjects.size(); i++)
	{
		listObjects[i]->Update(dt, &listObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

}

void GreenLand::Render()
{
	map->Render();
	// list objects
	for (int i = 0; i < listObjects.size(); i++)
		listObjects[i]->Render();

	player->Render();
	hud->Render({ CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY() }, player, 0, this->id);
}

void GreenLand::Unload()
{
	for (size_t i = 0; i < listObjects.size(); i++)
		delete listObjects[i];

	listObjects.clear();

	delete player;
	delete hud;

	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
#pragma endregion

#pragma region INPUT KEYBOARD

void GreenLandKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((GreenLand*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_RIGHT:
		if (mario->isIdling && mario->canWalkRight)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		break;
	case DIK_LEFT:
		if (mario->isIdling && mario->canWalkLeft)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		break;
	case DIK_UP:
		if (mario->isIdling && mario->canWalkUp)
			mario->SetState(MARIO_STATE_WALKING_UP);
		break;
	case DIK_DOWN:
		if (mario->isIdling && mario->canWalkDown)
			mario->SetState(MARIO_STATE_WALKING_DOWN);
		break;
	}
}

void GreenLandKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((GreenLand*)scence)->GetPlayer();

	if (game->IsKeyDown(DIK_S))	{
		mario->canSwitchScene = true;
	}
	else {
		mario->canSwitchScene = false;
	}
}

#pragma endregion
