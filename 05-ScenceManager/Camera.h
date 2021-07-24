#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Utils.h"
#include "Game.h"
#include "define.h"

class Camera
{
private:
	static Camera* __instance;
	D3DXVECTOR2 position;

	int width;
	int height;

public:
	static Camera* GetInstance();

	bool islockUpdate = false; // lock when mario die
	bool islockY; // lock when mario isOnGround and jump to get higher ->cam not move

	Camera();
	~Camera() {}

	// D3DXVECTOR2 start, D3DXVECTOR2 end : (2 map: main / hidden map) mario o map nao -> start vi tri map
	void Update(DWORD dt, D3DXVECTOR2 playerPos, D3DXVECTOR2 start, D3DXVECTOR2 end, bool isFlying, bool isOnGround);

	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	bool IsLockUpdate() { return this->islockUpdate; }
	void SetLockUpdate() { this->islockUpdate = true; }
	void SetUnLockUpdate() { this->islockUpdate = false; }

};
typedef Camera* LPCAMERA;

