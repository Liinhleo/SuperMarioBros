#include "Camera.h"
#include "Utils.h"

#define HUD_HEIGHT	30

Camera* Camera::__instance = nullptr;
Camera* Camera::GetInstance() {
	if (!__instance) __instance = new Camera();
	return __instance;
}

Camera::Camera() {
	this->width = SCREEN_WIDTH;
	this->height = SCREEN_HEIGHT - HUD_HEIGHT;
	this->islockUpdate = false;
	this->islockY = true;
	CGame::GetInstance()->SetCamPos(0, 250);
}


void Camera::Update(DWORD dt, D3DXVECTOR2 playerPos, D3DXVECTOR2 start, D3DXVECTOR2 end, bool isFlying, bool isOnGround) {

	if (islockUpdate)
		return;

	// vi tri camera lun nam giua Mario
	position.x = playerPos.x - width / 2 + MARIO_BIG_BBOX_WIDTH;

	/*DebugOut(L"playerPos X ==== %f \n", playerPos.x);
	DebugOut(L"playerPos y ==== %f \n", playerPos.y);

	DebugOut(L"position X ==== %f \n", position.x);
	DebugOut(L"position y ==== %f \n", position.y);*/

	// Check if camera out of screen map
	if (position.x > end.x)
		position.x = end.x;
	if (position.x < start.x)
		position.x = start.x;

	// Check lock_y
	if (playerPos.y < end.y && isFlying)
		islockY = false;
	if (playerPos.y > end.y && isOnGround && !isFlying)
		islockY = true;
	
	// Update pos_cam_y
	if (!islockY) { // pos_mario in center cam  < end.y (map) -> pos_cam = pos_mario...
		if (int(playerPos.y - height / 2 + HUD_HEIGHT) < end.y)
			position.y = int(playerPos.y - height / 2 + HUD_HEIGHT);
	}
	else if (islockY) { // id pos_cam in end-bbox of map-> pos_cam = end_map_y
		position.y = end.y;
	}
	if (position.y < start.y)
		position.y = start.y;
	CGame::GetInstance()->SetCamPos(position.x, position.y);

}
