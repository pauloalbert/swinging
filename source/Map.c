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
		1,0,0,0,0, 0,0,0,0,2,
		1,0,0,0,0, 0,0,0,0,3,
		1,0,0,0,0, 0,0,0,0,5,
		1,0,0,0,0, 0,0,0,0,1,
		1,0,0,0,0, 0,0,0,0,5,
		1,0,0,0,0, 0,0,0,0,2,
		1,0,0,0,0, 0,0,0,0,3,
		1,1,1,1,1, 1,1,1,1,1
};

void Map_Init(){

}

/*
 * This function returns the distance and the type of wall from a position.
 * If <wall_type> is supplied, the type of wall will be returned aswell.
 * If <is_x_wall> is supplied, the wall face will be returned.
 * */

float Map_get_raycast_distance(int px, int py, float angle, bool* is_x_wall, int* wall_type){
	float slope = tan(angle);
	bool facing_down = sin(angle) > 0;
	bool facing_right = cos(angle) > 0;

	float x_distance = 0;
	float y_distance = 0;

	int i;
	float float_py = py;
	float float_px = px;

	int x_wall_type = 0;
	int y_wall_type = 0;

	for(i = 0; i < RAYCAST_RECURSION && (y_distance < 1000000 || x_distance < 1000000); i++){
		//Advance the shorter ray of the two
		if(x_distance <= y_distance){
			//If a wall on this axis was discovered, return it.
			//(The theory here is, this will only be reached if the y ray is at least longer).
			if(x_wall_type){
				if(wall_type) *(wall_type) = x_wall_type;
				if(is_x_wall) *(is_x_wall) = true;
				return x_distance;
			}

			//Advance the px by 1 block, and py by the slope.
			int last_px = px;
			px = (((px+(facing_right ? WORLD_BLOCK_SIZE : -1))>>WORLD_BLOCK_BITS))<<WORLD_BLOCK_BITS;
			float_py += slope*(px - last_px);
			x_distance += sqrt( (1+slope*slope)*(px-last_px)*(px-last_px) );

			//allow loop-around for the rays
			px = mod(px - !facing_right, MAP_WIDTH<<WORLD_BLOCK_BITS) + !facing_right;
			float_py = mod_float(float_py,MAP_HEIGHT<<WORLD_BLOCK_BITS);

			//This should never happen but is here just incase i want to remove the top line.
			if(px < 0 || (int)float_py < 0 || px>>WORLD_BLOCK_BITS > MAP_WIDTH || (int)float_py>>WORLD_BLOCK_BITS > MAP_HEIGHT) x_distance = 1000000;

			//Get the building the ray is at.
			int current_wall = getBuilding((px>>WORLD_BLOCK_BITS) - !facing_right, ((int)float_py) >> WORLD_BLOCK_BITS);

			//If hit a wall, save it.
			if(current_wall != 0){
				x_wall_type = current_wall;
			}
		}
		else{

			//If a wall on this axis was discovered, return it.
			if(y_wall_type){
				if(wall_type) *(wall_type) = y_wall_type;
				if(is_x_wall) *(is_x_wall) = false;
				return y_distance;
			}

			//Update positions
			int last_py = py;
			py = ((py+(facing_down ? WORLD_BLOCK_SIZE : -1))>>WORLD_BLOCK_BITS)<<WORLD_BLOCK_BITS;
			float_px += (1/slope)*(py - last_py);
			y_distance += sqrt( (1+(1/slope)*(1/slope))*(py-last_py)*(py-last_py) );

			//Loop around
			py = mod(py - !facing_down, MAP_HEIGHT<<WORLD_BLOCK_BITS) + !facing_down;
			float_px = mod_float(float_px,MAP_WIDTH<<WORLD_BLOCK_BITS);

			//Out of bounds sanity check
			if(py < 0 || (int)float_px < 0 || py>>WORLD_BLOCK_BITS > MAP_WIDTH || (int)float_px>>WORLD_BLOCK_BITS > MAP_HEIGHT) y_distance = 1000000;

			//gyat building
			int current_wall = getBuilding(((int)float_px) >> WORLD_BLOCK_BITS,(py - !facing_down)>>WORLD_BLOCK_BITS);

			//save it.
			if(current_wall != 0){
				y_wall_type = current_wall;
			}
		}
	}

	//If both rays have missed, return very large distance.
	return 1000000;
}

float get_grip_position (Camera camera, touchPosition touch, Pos* grip){

	//Calculate the vertical and horizontal angle from the touch position:
	float angle_horizontal = touch.px * camera.fov_width / 256;
	float angle_vertical = touch.py * camera.fov_height / 192;

	//get the grip from raycasting
	Pos get_grip = {-1,-1,-1};

	return 0;
}



