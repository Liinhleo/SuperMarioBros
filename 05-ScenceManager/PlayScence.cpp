#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Map.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/
void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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
void CPlayScene::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());
	
	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap);

	//CMaps::GetInstance()->Add(map_id, map);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		{	
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;

			DebugOut(L"[INFO] Player object created!\n");
			break;
		}
	case OBJECT_TYPE_GOOMBA: 
		{
			obj = new CGoomba(); 

			// General object setup
			//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			//obj->SetPosition(x, y);
			//obj->SetAnimationSet(ani_set);
			//objects.push_back(obj);
		}
		break;

	case OBJECT_TYPE_KOOPAS:
	{
		obj = new CKoopas();

		// General object setup
		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		//obj->SetPosition(x, y);
		//obj->SetAnimationSet(ani_set);
		//objects.push_back(obj);
	}
	break;

	case OBJECT_TYPE_BRICK: 
		{
			int typeBrick = atoi(tokens[4].c_str());
			int typeItem = atoi(tokens[5].c_str());
			int count = atoi(tokens[6].c_str());

			obj = new CBrick(typeBrick, typeItem, count);

	//		// General object setup
	//		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	//		//obj->SetPosition(x, y);
	//		//obj->SetAnimationSet(ani_set);
	//		//listStatic.push_back(obj);
		}
		break;
	case OBJECT_TYPE_GROUND: 
		{
			// Read file
			int w = atoi(tokens[4].c_str());
			int h = atoi(tokens[5].c_str());
			int i = atoi(tokens[6].c_str());

			obj = new CGround(w, h, i);

			// General object setup
			//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			//obj->SetPosition(x, y);
			//obj->SetAnimationSet(ani_set);
			//listStatic.push_back(obj);
		}
		break;

	case OBJECT_TYPE_PORTAL:
		{	
			// Read file
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);

			// General object setup
			//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			//obj->SetPosition(x, y);
			//obj->SetAnimationSet(ani_set);
			//listStatic.push_back(obj);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAPS]") {
			section = SCENE_SECTION_TILE_MAP; continue;	}
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

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	// Get Scene ID -> Get map_id --> update map with camera

	vector<LPGAMEOBJECT> coObjects; //coObject == collision object

	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 


#pragma region CAMERA FOLLOW PLAYER
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	// vi tri cam luon de mario o giua screen
	//cx -= game->GetScreenWidth() / 2;
	//cy -= game->GetScreenHeight() / 2;

	game->cam_y = 250;

	// TH mario o giua screen va screen_width < screen_map_width
	if (player->x > (SCREEN_WIDTH / 2) && player->x + (SCREEN_WIDTH / 2) < map->getWidthMap())
	{
		cx = player->x - (SCREEN_WIDTH / 2);
		game->cam_x = cx;
	}
	//CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
#pragma endregion
}

void CPlayScene::Render()
{
	map->Render();

	// i = 0 : Render Mario (player)
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	// maps.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


/* INPUT KEYBOARD */
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE)
		return;

	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACOON && abs(mario->vx) == MARIO_MAX_SPEED) {
			mario->SetState(MARIO_STATE_FLY);
		}
		else {
			if (mario->isOnGround) {
				mario->SetState(MARIO_STATE_JUMP);
			}
		}
		break;
	case DIK_X:
		if (mario->isOnGround)
			mario->SetState(MARIO_STATE_JUMP_LOW);
		break;
	case DIK_F1:
		mario->Reset();
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACOON) {
			mario->SetState(MARIO_STATE_ATTACK);
		}
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE)
		return;

	// Check event after releasing key 
	switch (KeyCode){
	case DIK_S:
		if (!mario->flyTime->IsTimeUp()) {
			mario->vy = -(MARIO_GRAVITY + 0.002f * 4) * mario->dt;
		}
		else {
			mario->vy = 0;
		}
		break;
	case DIK_DOWN:
		if (mario->GetLevel() == MARIO_LEVEL_BIG
		|| mario->GetLevel() == MARIO_LEVEL_RACOON) {
			mario->SetState(MARIO_STATE_STAND_UP);// avoid Mario falling out after sitting
		}
		else
			return;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE)
		return;

	if (mario->isOnGround) {
		// RUN RIGHT
		if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT)) {
			if (mario->state == MARIO_STATE_SIT)
				return;
			if (mario->vx < 0) {
				mario->SetState(MARIO_STATE_STOP);
			}
			else {
				mario->SetState(MARIO_STATE_RUN);
				mario->SetAccelerate(mario->a += MARIO_SPEED_UP);
			}
		}

		// RUN LEFT
		else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_LEFT)) {
			if (mario->state == MARIO_STATE_SIT)
				return;
			if (mario->vx > 0) {
				//mario->SetAccelerate(mario->a += MARIO_SPEED_DOWN);
				mario->SetState(MARIO_STATE_STOP);
			}
			else {
				mario->SetState(MARIO_STATE_RUN);
				mario->SetAccelerate(mario->a -= MARIO_SPEED_UP);
			}
		}

		// WALK RIGHT
		else if (game->IsKeyDown(DIK_RIGHT)) {
			if (mario->state == MARIO_STATE_SIT)
				return;
			
			// suy nghi them de mario co the truot 1 doan truoc khi di tiep ben trai
			if (mario->vx < 0) {
				mario->SetAccelerate(mario->a = 0);
				mario->SetState(MARIO_STATE_STOP);
			}

			else {
				if (mario->GetAccelerate() != 0) {
					mario->SetAccelerate(mario->a -= MARIO_SPEED_DOWN);
					if (mario->a <= 0) {
						mario->SetAccelerate(mario->a = 0);
					}
				}
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
		}

		// WALK LEFT
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (mario->state == MARIO_STATE_SIT)
				return;
			if (mario->vx > 0) {
				mario->SetState(MARIO_STATE_STOP);
			}
			else {
				if (mario->GetAccelerate() != 0) {
					mario->SetAccelerate(mario->a += MARIO_SPEED_DOWN);
					if (mario->a <= 0) {
						mario->SetAccelerate(mario->a = 0);
					}
				}
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
		}

		// SIT
		else if (game->IsKeyDown(DIK_DOWN)) {
			if (mario->GetLevel() == MARIO_LEVEL_BIG
				|| mario->GetLevel() == MARIO_LEVEL_RACOON) {
				mario->SetState(MARIO_STATE_SIT);
			}
			else
				return;
		}
		else {
			mario->SetAccelerate(mario->a = 0);
			mario->SetState(MARIO_STATE_IDLE);
		}
	}
	else {
		// is not on Ground -> jump / fly
	}

	
}