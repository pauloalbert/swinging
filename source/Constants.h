/*
 * Constants.h
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#pragma once
/* VISUALS */

#define RAYCAST_RECURSION 20
#define MAX_INT (0xffff)

/* MAZE */
#define WORLD_BLOCK_BITS 5
#define WORLD_BLOCK_SIZE 32//(2<<MAZE_BLOCK_BITS);
#define MAP_HEIGHT 25
#define MAP_WIDTH 25

//#define MAP_LOOPS

/* BUILDING */
#define BUILDINGS_MAX_HEIGHT 128
#define BUILDING_COLOR_BITS 12
#define BUILDING_HEIGHT_BITS 0
#define BUILDING_HEIGHT_UNITS 1

#define BUILDING(color, height) (((color)<<BUILDING_COLOR_BITS) | (height)<<BUILDING_HEIGHT_BITS)

/* GRAPHICS */
#define HORIZON_DISTANCE WORLD_BLOCK_SIZE*30

#define IS_SCREEN_FLIPPED true
/* MOVEMENT */

#define RAYCAST_ERROR_DISTANCE 1000000.
#define DEFAULT_DT 0.2

/* GAME */

#define TIME_POWER 3			//counted in 500ms
#define TIME_POWER_BACK 5

/* MATH */
#define Pi 3.141592

