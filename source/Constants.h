/*
 * Constants.h
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#pragma once
/* VISUALS */

#define GRIP_RAYCAST_RECURSION 20
#define VISION_RAYCAST_RECURSION 60
#define VISION_SECOND_RAYCAST_RECURSION 30
#define VISION_HEIGHT_ITERATIONS 3
#define MAX_INT (0xffff)

/* MAZE */
#define WORLD_BLOCK_BITS 5
#define WORLD_BLOCK_SIZE 32//(2<<MAZE_BLOCK_BITS);
#define MAP_HEIGHT 25
#define MAP_WIDTH 25

//#define MAP_LOOPS

/* BUILDING */
#define BUILDINGS_MAX_HEIGHT 300
#define BUILDING_COLOR_BITS 12
#define BUILDING_HEIGHT_BITS 0
#define BUILDING_HEIGHT_UNITS 1

#define BUILDING(color, height) (((color)<<BUILDING_COLOR_BITS) | (height)<<BUILDING_HEIGHT_BITS)

/* GRAPHICS */
#define HORIZON_DISTANCE WORLD_BLOCK_SIZE*100
/* COLOR DROPOFF */
//Amount of 8+ how much to do a gradient on colors (0..8..x) where x is black
#define COLOR_FALLOFF_GRADIENT_RANGE 13
#define COLOR_FALLOFF_PER_DISTANCE 64

#define IS_SCREEN_FLIPPED true
/* MOVEMENT */

#define RAYCAST_ERROR_DISTANCE 1000000.
#define DEFAULT_DT 0.3

/* GAME */

#define TIME_POWER 3			//counted in 500ms
#define TIME_POWER_BACK 5

