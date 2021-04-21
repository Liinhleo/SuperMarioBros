#include "Map.h"
#include "Utils.h"

// ANIMATION FRAME == TILE MAP = Sprite (tile)
CTileMap::CTileMap(LPSPRITE sprite) {
	this->sprite = sprite;
}
LPSPRITE CTileMap::GetSprite() {
	return sprite;
}

// ANIMATION 
//void CMap::Add(int spriteId)
//{
//	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
//
//	if (sprite == NULL)
//	{
//		DebugOut(L"[ERROR] Map ID %d cannot be found!\n", spriteId);
//	}
//
//	LPTILEMAP tile = new CTileMap(sprite);
//	tiles.push_back(tile);
//}


void CMap::Draw(float x, float y, int alpha)
{
	//tiles->GetSprite()->Draw(x, y, alpha);
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
//	for (auto x : maps)
//	{
//		LPMAP map = x.second;
//		delete map;
//	}
//
//	maps.clear();
//}

