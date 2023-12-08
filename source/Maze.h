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
	float angle;
} Camera;

typedef struct{
	float x;
	float y;
	float angle;
	float x_vel;
	float y_vel;
	float torque;
	int score;
	byte health;
} Player;

typedef struct{
	int x;
	int y;
	int old_block;
} Goal;

void Maze_Init();

float Maze_get_raycast_distance(int x, int y, float angle, bool x_wall, int* wall_type);

void Render_screen(enum BUFFER_TYPE bT, Camera player, int columns);

void Render_map(enum BUFFER_TYPE bT, Camera player);

byte getMaze(int x, int y);

byte getMazeFromWorld(float x, float y);

void tryGoal(float x, float y, float angle, Player* player);

u16 shuffleGoal();
