#pragma once
// ========================================= MAIN SCREEN ==================================================
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"SAMPLE 05 - SCENCE MANAGER"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300
#define MAX_FRAME_RATE 120

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define TILE_SIZE 16			 // 16x16

// ======================================= PLAY SCENE ===============================================
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TILE_MAP			7

// ========================================= MARIO ==================================================
// =========== BBOX ====================
#define MARIO_BIG_BBOX_WIDTH				14
#define MARIO_BIG_BBOX_HEIGHT				27

#define MARIO_SMALL_BBOX_WIDTH				13
#define MARIO_SMALL_BBOX_HEIGHT				15

#define MARIO_RACCOON_BBOX_WIDTH			15
#define MARIO_RACCOON_BBOX_HEIGHT			28

// =========== TIME ====================
#define MARIO_UNTOUCHABLE_TIME		2000
#define MARIO_FLYING_TIME			3000

// =========== SPEED ====================
// VX 
#define MARIO_WALKING_SPEED			0.08f	
#define MARIO_MAX_SPEED				0.16f
#define MARIO_SPEED_UP				0.0001f // TANG TOC
#define MARIO_SPEED_DOWN			0.0006f // GIAM TOC 

// VY
#define MARIO_JUMP_SPEED_Y			0.25f
#define MARIO_JUMP_DEFLECT_SPEED	0.1f
#define MARIO_GRAVITY				0.0005f // trong luc

#define MARIO_DIE_DEFLECT_SPEED		0.35f  // die

#define TIME_FLY					3000

// 
// =========== STATE MARIO ====================
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_FALL			400
#define MARIO_STATE_DIE				500
#define MARIO_STATE_SIT				600
#define MARIO_STATE_RUN				700
#define MARIO_STATE_FLY				800
#define MARIO_STATE_STOP			900
#define MARIO_STATE_ATTACK			1000
#define MARIO_STATE_STAND_UP		1100
#define MARIO_STATE_JUMP_LOW		1200


// =========== ANI BIG MARIO ====================
#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_BIG_WALKING_RIGHT			2
#define MARIO_ANI_BIG_WALKING_LEFT			3
#define MARIO_ANI_BIG_JUMP_RIGHT			4
#define MARIO_ANI_BIG_JUMP_LEFT				5	
#define MARIO_ANI_BIG_RUN_RIGHT				6	
#define MARIO_ANI_BIG_RUN_LEFT				7
#define MARIO_ANI_BIG_STOP_RIGHT			8
#define MARIO_ANI_BIG_STOP_LEFT				9
#define MARIO_ANI_BIG_SIT_RIGHT				10
#define MARIO_ANI_BIG_SIT_LEFT				11
#define MARIO_ANI_BIG_FLY_RIGHT				12
#define MARIO_ANI_BIG_FLY_LEFT				13
#define MARIO_ANI_BIG_KICK_RIGHT			14
#define MARIO_ANI_BIG_KICK_LEFT				15
#define MARIO_ANI_BIG_THROUGH_PIPE			16
#define MARIO_ANI_BIG_IDLE_HOLDING_RIGHT	17
#define MARIO_ANI_BIG_IDLE_HOLDING_RIGHT	18
#define MARIO_ANI_BIG_WALK_HOLDING_RIGHT	19
#define MARIO_ANI_BIG_WALK_HOLDING_LEFT		20
#define MARIO_ANI_BIG_ON_AIR_HOLDING_RIGHT	21
#define MARIO_ANI_BIG_ON_AIR_HOLDING_LEFT	22
#define MARIO_ANI_BIG_FALLING_RIGHT			23
#define MARIO_ANI_BIG_FALLING_LEFT			24

// =========== ANI SMALL MARIO ====================
#define MARIO_ANI_SMALL_IDLE_RIGHT				25
#define MARIO_ANI_SMALL_IDLE_LEFT				26
#define MARIO_ANI_SMALL_WALKING_RIGHT			27
#define MARIO_ANI_SMALL_WALKING_LEFT			28
#define MARIO_ANI_SMALL_JUMP_RIGHT				29
#define MARIO_ANI_SMALL_JUMP_LEFT				30
#define MARIO_ANI_SMALL_RUN_RIGHT				31
#define MARIO_ANI_SMALL_RUN_LEFT				32
#define MARIO_ANI_SMALL_STOP_RIGHT				33
#define MARIO_ANI_SMALL_STOP_LEFT				34
#define MARIO_ANI_SMALL_SIT_RIGHT				35
#define MARIO_ANI_SMALL_SIT_LEFT				36
#define MARIO_ANI_SMALL_JUMP_HIGH_RIGHT			37
#define MARIO_ANI_SMALL_JUMP_HIGH_LEFT			38
#define MARIO_ANI_SMALL_KICK_RIGHT				39
#define MARIO_ANI_SMALL_KICK_LEFT				40
#define MARIO_ANI_SMALL_THROUGH_PIPE			41
#define MARIO_ANI_SMALL_IDLE_HOLDING_RIGHT		42
#define MARIO_ANI_SMALL_IDLE_HOLDING_LEFT		43
#define MARIO_ANI_SMALL_WALK_HOLDING_RIGHT		44
#define MARIO_ANI_SMALL_WALK_HOLDING_LEFT		45
#define MARIO_ANI_SMALL_ON_AIR_HOLDING_RIGHT	46
#define MARIO_ANI_SMALL_ON_AIR_HOLDING_LEFT		47
#define MARIO_ANI_SMALL_FALLING_RIGHT			48
#define MARIO_ANI_SMALL_FALLING_LEFT			49
#define MARIO_ANI_SMALL_WM_INTRO				50
#define MARIO_ANI_DIE							51

// =========== ANI RACOON MARIO ====================
#define MARIO_ANI_RACOON_IDLE_RIGHT				52
#define MARIO_ANI_RACOON_IDLE_LEFT				53
#define MARIO_ANI_RACOON_WALKING_RIGHT			54
#define MARIO_ANI_RACOON_WALKING_LEFT			55
#define MARIO_ANI_RACOON_JUMP_RIGHT				56
#define MARIO_ANI_RACOON_JUMP_LEFT				57
#define MARIO_ANI_RACOON_RUN_RIGHT				58
#define MARIO_ANI_RACOON_RUN_LEFT				59
#define MARIO_ANI_RACOON_STOP_RIGHT				60
#define MARIO_ANI_RACOON_STOP_LEFT				61
#define MARIO_ANI_RACOON_SIT_RIGHT				62
#define MARIO_ANI_RACOON_SIT_LEFT				63
#define MARIO_ANI_RACOON_FLY_RIGHT				64
#define MARIO_ANI_RACOON_FLY_LEFT				65
#define MARIO_ANI_RACOON_KICK_RIGHT				66
#define MARIO_ANI_RACOON_KICK_LEFT				67
#define MARIO_ANI_RACOON_ATTACK_RIGHT			68
#define MARIO_ANI_RACOON_ATTACK_LEFT			69
#define MARIO_ANI_RACOON_IDLE_HOLDING_RIGHT		70
#define MARIO_ANI_RACOON_IDLE_HOLDING_LEFT		71
#define MARIO_ANI_RACOON_WALK_HOLDING_RIGHT		72
#define MARIO_ANI_RACOON_WALK_HOLDING_LEFT		73
#define MARIO_ANI_RACOON_ON_AIR_HOLDING_RIGHT	74
#define MARIO_ANI_RACOON_ON_AIR_HOLDING_LEFT	75
#define MARIO_ANI_RACOON_FALLING_RIGHT			76
#define MARIO_ANI_RACOON_FALLING_LEFT			77
#define MARIO_ANI_RACOON_THROUGH_PIPE			78

// =========== ANI FIRE MARIO ====================


// ========= MARIO LEVEL ========
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3
#define MARIO_LEVEL_FIRE	4





// ========================================= BRICK ==================================================
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

// brickType
#define BRICK_NORMAL		0 
#define BRICK_QUESTION		1
#define BRICK_GLASS			2
#define BRICK_MUSIC			3

// isContainItem
#define CONTAIN_NON			0
#define CONTAIN_ITEM		1
#define CONTAIN_COIN		2
#define CONTAIN_P_SWITCH	3

/*  ========== STATE BRICK ========== */
#define BRICK_STATE_ACTIVE		1
#define BRICK_STATE_BROKEN		2  // broken -> gach cung
#define BRICK_STATE_INACTIVE	3  // bien mat hoan toan

/*  ========== ANI BRICK ==========  */
// brick_normal 
#define BRICK_ANI_NORMAL_ACTIVE			0
// brick question mark
#define BRICK_ANI_QUESTION_ACTIVE		1
#define BRICK_ANI_QUESTION_BROKEN		2	// broken -> gach cung
// brick glass
#define BRICK_ANI_GLASS_ACTIVE		3
#define BRICK_ANI_GLASS_BROKEN		4 // broken -> gach cung
#define BRICK_ANI_GLASS_INACTIVE  	5 // effect broken and disapear
// brick_music
#define BRICK_ANI_MUSIC_ACTIVE		6

////BRONZE BRICK PIECE
//#define PIECE_GRAVITY		0.0008f
//#define PIECE_SPEED_X		0.03f
//#define PIECE_SPEED_Y		0.1f



// ========================================= OBJECT TYPE ==================================================

enum ObjectType { 
	MARIO = 0,
	
	//static list object
	GROUND = 01,
	BRICK = 02,	
	PIPE = 03,
	LIFT = 04,

	//dynamic list object
	GOOMBA = 05,
	KOOPA = 06,
	PIRANHA_FLOWER = 07,
	FIRE_FLOWER = 8,
	NINJA = 9,

	//item
	ITEM = 10,
	COIN = 11,
	P_SWITCH =12,

	//weapon
	FIRE_BALL = 13,

	//trans_map
	PORTAL = 14,
};

#define OBJECT_TYPE_MARIO	0

// static objects
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GROUND	2
#define OBJECT_TYPE_PIPE	3
#define OBJECT_TYPE_LIFT	4

//item
#define OBJECT_TYPE_COIN		5

// enemy
#define OBJECT_TYPE_GOOMBA			10
#define OBJECT_TYPE_KOOPAS			11
#define OBJECT_TYPE_PIRANHA_FLOWER	12
#define OBJECT_TYPE_FIRE_FLOWER		13


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024