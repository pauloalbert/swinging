/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds paulo
 *
 */
#include "Map.h"
#include "P_Util.h"

MAC_EXTERN inline s8 getBuilding(int x, int y){
	return map[coords(x,y,MAP_WIDTH)];
}

MAC_EXTERN inline s8 getBuildingFromWorld(float px, float py){
	return getBuilding(round_float(px)>>WORLD_BLOCK_BITS,round_float(py)>>WORLD_BLOCK_BITS);
}

MAC_EXTERN inline s8 getBuildingFromFXP(int x, int y){
	return getBuilding(x>>FXP_DECIMAL_BITS, y>>FXP_DECIMAL_BITS);
}

int map[] = {1,1,1,1,1, 1,1,1,1,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 1,1,1,0,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 0,1,0,0,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 0,0,0,0,1,
		1,1,1,1,1, 1,1,1,1,1
};

void Map_Init(){

}

/*
 * This function returns the distance and the type of wall from a position.
 *
 * */

float Map_get_raycast_distance(int px, int py, float angle, bool x_wall, int* wall_type){
	float slope = tan(angle);

	bool facing_down = sin(angle) > 0;
	bool facing_right = cos(angle) > 0;
	inline int mod(int x, int amount){
		return ((x % amount) + amount) % amount;
	}
	inline float mod_float(float x, int amount){
		return fmod(fmod(x, amount) + amount,amount);
	}

	if(x_wall){
		float float_py = py;
		//set a limit
		int i = 0;
		float distance = 0;

		for(i = 0; i < RAYCAST_RECURSION; i++){
			int last_px = px;
			px = (((px+(facing_right ? WORLD_BLOCK_SIZE : -1))>>WORLD_BLOCK_BITS))<<WORLD_BLOCK_BITS;
			float_py += slope*(px - last_px);
			distance += sqrt( (1+slope*slope)*(px-last_px)*(px-last_px) );

			px = mod(px - !facing_right, MAP_WIDTH<<WORLD_BLOCK_BITS) + !facing_right;
			float_py = mod_float(float_py,MAP_HEIGHT<<WORLD_BLOCK_BITS);

			if(px < 0 || (int)float_py < 0 || px>>WORLD_BLOCK_BITS > MAP_WIDTH || (int)float_py>>WORLD_BLOCK_BITS > MAP_HEIGHT) return 1000000;

			int current_wall = map[coords((((px)>>WORLD_BLOCK_BITS) - !facing_right),((int)float_py)>>WORLD_BLOCK_BITS,MAP_WIDTH)];
			if(current_wall != 0){
				if(wall_type) *(wall_type) = current_wall;
				return distance;
			}
		}
		return 1000000;
	}
	else{
		float float_px = px;
		//set a limit
		int i = 0;
		float distance = 0;
		for(i = 0; i < RAYCAST_RECURSION; i++){
			int last_py = py;
			py = ((py+(facing_down ? WORLD_BLOCK_SIZE : -1))>>WORLD_BLOCK_BITS)<<WORLD_BLOCK_BITS;
			float_px += (1/slope)*(py - last_py);
			distance += sqrt( (1+(1/slope)*(1/slope))*(py-last_py)*(py-last_py) );

			py = mod(py - !facing_down, MAP_HEIGHT<<WORLD_BLOCK_BITS) + !facing_down;
			float_px = mod_float(float_px,MAP_WIDTH<<WORLD_BLOCK_BITS);
			if(py < 0 || (int)float_px < 0 || py>>WORLD_BLOCK_BITS > MAP_WIDTH || (int)float_px>>WORLD_BLOCK_BITS > MAP_HEIGHT) return 1000000;

			int current_wall = map[coords(((int)(float_px)>>WORLD_BLOCK_BITS),(py>>WORLD_BLOCK_BITS)-!facing_down,MAP_WIDTH)];
			if(current_wall != 0){
				if(wall_type) *(wall_type) = current_wall;
				return distance;
			}
		}
		return 1000000;

	}

}

float get_grip_position (Camera camera, touchPosition touch, Pos* grip){

	//Calculate the vertical and horizontal angle from the touch position:
	float angle_horizontal = touch.px * camera.fov_width / 256;
	float angle_vertical = touch.py * camera.fov_height / 192;

	//get the grip from raycasting
	Pos get_grip = {-1,-1,-1};

	return 0;
}



