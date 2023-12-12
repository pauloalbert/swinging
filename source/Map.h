#ifndef SWING_MAP_H
#define SWING_MAP_H
#include <stdio.h>
#include <nds.h>
#include "Constants.h"
#include <stdlib.h>
#include <math.h>
#include "P_Util.h"
#include <stdbool.h>
//         north (-y)
//	(0,0) (1,0) (2,0)
//  (0,1) (1,1) (2,1)   east(+x)
//  (0,2) (1,2) (2,2)
//         south (+y)
//player lives in 256 by 256 grid, but each block is 16*16

typedef struct{
	int x;
	int y;
	int z;

	float pan;
	float tilt;

	float fov_width;
	float fov_height;
} Camera;

typedef struct{
	float x;
	float y;
	float angle;
	//TODO: store for the player
} Player;

typedef struct{
	float x;
	float y;
	float z;
} Pos;

void Map_Init();

float Map_get_raycast_distance(int px, int py, float angle, bool xwall, int* wall_type);

void Render_map(enum BUFFER_TYPE bT, Camera player);

MAC_EXTERN inline s8 getBuilding(int x, int y);

MAC_EXTERN inline s8 getBuildingFromWorld(float x, float y);

MAC_EXTERN inline s8 getBuildingFromFXP(int px, int py);

#endif
