#ifndef SWING_MAP_H
#define SWING_MAP_H
#include <stdio.h>
#include <nds.h>
#include "Constants.h"
#include <stdlib.h>
#include <math.h>
#include "P_Util.h"
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
	int x;
	int y;
	int old_block;
} Goal;

void Map_Init();

int Map_get_raycast_distance(int px, int py, float angle, int* wall_type);

void Render_map(enum BUFFER_TYPE bT, Camera player);

inline byte getBuilding(int x, int y);

inline byte getBuildingFromWorld(float x, float y);

inline byte getBuildingFromFXP(int px, int py);

#endif
