#pragma once
// ========================================= MAIN SCREEN ==================================================
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"SAMPLE 05 - SCENCE MANAGER"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define MAX_FRAME_RATE 120

#define ID_TEX_BBOX -100		// special texture to draw object bounding box


// ========================================= MARIO ==================================================
#define MARIO_BIG_BBOX_WIDTH				14
#define MARIO_BIG_BBOX_HEIGHT				27

#define MARIO_SMALL_BBOX_WIDTH				13
#define MARIO_SMALL_BBOX_HEIGHT				15

//#define MARIO_RACCOON_BBOX_WIDTH			15
//#define MARIO_RACCOON_BBOX_HEIGHT			28

#define MARIO_UNTOUCHABLE_TIME				2000


#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
//
////mario
//#define TIME_DEFAULT				300
//#define TIME_FLY					3000
//#define MARIO_TIME_ATTACK			400
//#define MARIO_TIME_SHOOT			300
//#define MARIO_TIME_STOP				100
//
//#define MARIO_WALKING_SPEED				0.08f
//#define MARIO_JUMP_SPEED_Y				0.25f
//#define MARIO_JUMP_DEFLECT_SPEED		0.1f
//#define MARIO_GRAVITY					0.0005f
//#define GOOMBA_GRAVITY					0.0005f
//#define KOOPA_GRAVITY					0.0004f
//
//#define MARIO_FALLING_SPEED_Y			0.45f
//#define MARIO_DIE_DEFLECT_SPEED			0.5f
//#define MARIO_RUN_SPEED_THRESH			0.16f
//#define MARIO_ACCELERATION				0.0006f
//#define mario_ACCELERATION				0.0003f
//
//#define MARIO_ANI_IDLE_RIGHT				0
//#define MARIO_ANI_WALK_RIGHT				1
//#define MARIO_ANI_JUMP_RIGHT				2
//#define MARIO_ANI_FALL_RIGHT				3
//#define MARIO_ANI_SIT_RIGHT					4
//#define MARIO_ANI_RUN_RIGHT					5
//#define MARIO_ANI_STOP_RIGHT				6
//#define MARIO_ANI_IDLE_HOLD_RIGHT			7
//#define MARIO_ANI_WALK_HOLD_RIGHT			8
//#define MARIO_ANI_ON_AIR_HOLD_RIGHT			9
//
//#define MARIO_ANI_IDLE_LEFT					10
//#define MARIO_ANI_WALK_LEFT					11
//#define MARIO_ANI_JUMP_LEFT					12
//#define MARIO_ANI_FALL_LEFT					13
//#define MARIO_ANI_SIT_LEFT					14
//#define MARIO_ANI_RUN_LEFT					15
//#define MARIO_ANI_STOP_LEFT					16
//#define MARIO_ANI_IDLE_HOLD_LEFT			17
//#define MARIO_ANI_WALK_HOLD_LEFT			18
//#define MARIO_ANI_ON_AIR_HOLD_LEFT			19
//
//#define mario_ANI_IDLE_RIGHT				20
//#define mario_ANI_WALK_RIGHT				21
//#define mario_ANI_JUMP_RIGHT				22
//#define mario_ANI_RUN_RIGHT					23
//#define mario_ANI_STOP_RIGHT				24
//#define mario_ANI_IDLE_HOLD_RIGHT			25
//#define mario_ANI_WALK_HOLD_RIGHT			26
//#define mario_ANI_ON_AIR_HOLD_RIGHT			27
//
//#define mario_ANI_IDLE_LEFT					28
//#define mario_ANI_WALK_LEFT					29
//#define mario_ANI_JUMP_LEFT					30
//#define mario_ANI_RUN_LEFT					31
//#define mario_ANI_STOP_LEFT					32
//#define mario_ANI_IDLE_HOLD_LEFT			33
//#define mario_ANI_WALK_HOLD_LEFT			34
//#define mario_ANI_ON_AIR_HOLD_LEFT			35
//
//#define MARIO_ANI_DIE						36
//
//#define RACCOON_ANI_IDLE_RIGHT				37
//#define RACCOON_ANI_WALK_RIGHT				38
//#define RACCOON_ANI_RUN_RIGHT				39
//#define RACCOON_ANI_JUMP_RIGHT				40
//#define RACCOON_ANI_STOP_RIGHT				41
//#define RACCOON_ANI_FIGHT_IDLE_RIGHT		42
//#define RACCOON_ANI_SIT_RIGHT				43
//#define RACCOON_ANI_FALL_RIGHT				44
//#define RACCOON_ANI_WAG_TAIL_RIGHT			45
//#define RACCOON_ANI_FLY_RIGHT				46
//#define RACCOON_ANI_IDLE_HOLD_RIGHT			47
//#define RACCOON_ANI_WALK_HOLD_RIGHT			48
//#define RACCOON_ANI_ON_AIR_HOLD_RIGHT		49
//
//#define RACCOON_ANI_IDLE_LEFT				50
//#define RACCOON_ANI_WALK_LEFT				51
//#define RACCOON_ANI_RUN_LEFT				52
//#define RACCOON_ANI_JUMP_LEFT				53
//#define RACCOON_ANI_STOP_LEFT				54
//#define RACCOON_ANI_FIGHT_IDLE_LEFT			55
//#define RACCOON_ANI_SIT_LEFT				56
//#define RACCOON_ANI_FALL_LEFT				57
//#define RACCOON_ANI_WAG_TAIL_LEFT			58
//#define RACCOON_ANI_FLY_LEFT				59
//#define RACCOON_ANI_IDLE_HOLD_LEFT			60
//#define RACCOON_ANI_WALK_HOLD_LEFT			61
//#define RACCOON_ANI_ON_AIR_HOLD_LEFT		62
//
//#define FIRE_ANI_IDLE_RIGHT					63
//#define FIRE_ANI_WALK_RIGHT					64
//#define FIRE_ANI_RUN_RIGHT					65
//#define FIRE_ANI_JUMP_RIGHT					66
//#define FIRE_ANI_STOP_RIGHT					67
//#define FIRE_ANI_FIGHT_IDLE_RIGHT			68
//#define FIRE_ANI_ATTACK_RIGHT				69
//#define FIRE_ANI_SIT_RIGHT					70
//#define FIRE_ANI_FALL_RIGHT					71
//#define FIRE_ANI_IDLE_HOLD_RIGHT			72
//#define FIRE_ANI_WALK_HOLD_RIGHT			73
//#define FIRE_ANI_ON_AIR_HOLD_RIGHT			74
//
//#define FIRE_ANI_IDLE_LEFT					75
//#define FIRE_ANI_WALK_LEFT					76
//#define FIRE_ANI_RUN_LEFT					77
//#define FIRE_ANI_JUMP_LEFT					78
//#define FIRE_ANI_STOP_LEFT					79
//#define FIRE_ANI_FIGHT_IDLE_LEFT			80
//#define FIRE_ANI_ATTACK_LEFT				81
//#define FIRE_ANI_SIT_LEFT					82
//#define FIRE_ANI_FALL_LEFT					83
//#define FIRE_ANI_IDLE_HOLD_LEFT				84
//#define FIRE_ANI_WALK_HOLD_LEFT				85
//#define FIRE_ANI_ON_AIR_HOLD_LEFT			86
//
//#define mario_WORLD_MAP						87
//#define MARIO_WORLD_MAP						88
//#define RACCOON_WORLD_MAP					89
//#define FIRE_WORLD_MAP						90




// ========================================= BRICK ==================================================
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

// ========== NEW ===========
//#define BRICK_ANI_NORMAL		0
//#define BRICK_ANI_BROKEN		1
//#define BRICK_ANI_HIDDEN		2
//#define STATE_BEING_TOSSED		0
//#define STATE_NORMAL			1
//#define STATE_BROKEN			2
//#define BRICK_STATE_HIDDEN		3
//
////BRONZE BRICK PIECE
//#define PIECE_GRAVITY		0.0008f
//#define PIECE_SPEED_X		0.03f
//#define PIECE_SPEED_Y		0.1f

// ========================================= PLAY SCENE ===============================================
//play scene
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
//#define SCENE_SECTION_TILE_MAP	7

// ========================================= OBJECT TYPE ==================================================
#define OBJECT_TYPE_MARIO	0

// static objects
#define OBJECT_TYPE_BRICK	1

// enemy
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

// ========== NEW ===========
//#define OBJECT_TYPE_GROUND			2
//#define OBJECT_TYPE_PIPE			3
//#define OBJECT_TYPE_COIN			4
//
//#define OBJECT_TYPE_LIFT			5
//
////enemy
//#define OBJECT_TYPE_GOOMBA				11
//#define OBJECT_TYPE_KOOPAS				12
//#define OBJECT_TYPE_VENUS_FIRE_TRAP		13
//#define OBJECT_TYPE_PIRANHA_PLANT		14
//#define OBJECT_TYPE_BOOMERANG_BROTHER	15
//
//#define OBJECT_MAP_POINT			51
//#define OBJECT_WORLD_MAP_OBJ		52
//
//
//#define OBJECT_TYPE_LAST_ITEM		100
//#define OBJECT_CURTAIN				101

#define MAX_SCENE_LINE 1024