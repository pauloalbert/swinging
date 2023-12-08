#pragma once
#include <stdio.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Constants.h"
#include <stdlib.h>
#include <math.h>
#include "P_Audio.h"
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

float Map_get_raycast_distance(int x, int y, float angle, bool x_wall, int* wall_type);

void Render_screen(enum BUFFER_TYPE bT, Camera player, int columns);

void Render_map(enum BUFFER_TYPE bT, Camera player);

byte getBuilding(int x, int y);

byte getBuildingFromWorld(float x, float y);
