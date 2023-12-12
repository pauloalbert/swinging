/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds paulo
 *
 */
#include "Map.h"
#include "P_Util.h"

extern inline s8 getBuilding(int x, int y){
	return map[coords(x,y,MAP_WIDTH)];
}

extern inline s8 getBuildingFromWorld(float px, float py){
	return getBuilding(round_float(px)>>WORLD_BLOCK_BITS,round_float(py)>>WORLD_BLOCK_BITS);
}

extern inline s8 getBuildingFromFXP(int x, int y){
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

Goal goal = {0, 0, 1};

void Map_Init(){

}

int Map_get_raycast_distance(int px, int py, float angle, int* wall_type){
	float slope = tan(angle);
	int slope_fxp = float_to_fxp(slope,FXP_DECIMAL_BITS);
	int slope_inverse_fxp = float_to_fxp(1/slope, FXP_DECIMAL_BITS);
	bool facing_down = sin(angle) > 0;
	bool facing_right = cos(angle) > 0;

	//distances of x and y
	int x_distance = 0, y_distance = 0;
	//second set of coordinates for the other ray
	int px2 = px;
	int py2 = py;
	/*This is the new MARCHING SQUARES raycast implemenation
	The idea is to shoot two rays, one that stops at X walls, one at Y walls
	if a wall is hit,
	*/

	printf("slope: %.2f, 1/s: %.2f\n",slope,(1/slope));
	int i = 0;
	for(i = 0; i < RAYCAST_RECURSION; i++){
		printf("start %d\n",i);
		printf("%.2f,%.2f\n",fxp_to_float(x_distance,8),fxp_to_float(y_distance,8));
		//Advance the shorter of the two rays.
		if(x_distance <= y_distance){
			if(px < 0 || px > (MAP_WIDTH<<FXP_DECIMAL_BITS) || py < 0 || py > (MAP_HEIGHT<<FXP_DECIMAL_BITS))
				return MAX_INT;
			//if we advanced x in the past and we hit a wall, this must be the shortest path
			int building = getBuildingFromFXP(px,py);
			if(building != 0){
				if(wall_type)
					(*wall_type) = building;
				return x_distance;
			}

			//advance the ray
			int last_px = px;
			px += FXP_UNIT * (facing_right * 2 - 1);	//subtract/add from x 256 based on facing

			//simplify the math depending on if we moved a full square already
			if(true){
				px = floor_bits(px, FXP_DECIMAL_BITS);		//round px to an int
				py += (int)((1/slope)*(px-last_px));		//add to py the movement
				x_distance += float_to_fxp(sqrt( (1+slope*slope)*(px-last_px)*(px-last_px)),FXP_DECIMAL_BITS);	//progress px
			}
			else{
				py += slope_fxp;
				x_distance += 1;
			}

			printf("x %x,%x,%x,%x,%x,%x\n",x_distance, y_distance, px,py,px2,py2);

		}
		else{
			if(px2 < 0 || px2 > (MAP_WIDTH<<FXP_DECIMAL_BITS) || py2 < 0 || py2 > (MAP_HEIGHT<<FXP_DECIMAL_BITS))
				return MAX_INT;
			//the y ray is currently shorter!
			//if we advanced x in the past and we hit a wall, this must be the shortest path
			int building = getBuildingFromFXP(px2,py2);
			if(building != 0){
				if(wall_type)
					(*wall_type) = building;
				return -y_distance;	//return NEGATIVE DISTANCE
			}

			//advance the ray
			int last_py = py2;
			py2 += FXP_UNIT * (facing_down * 2 - 1);	//subtract/add from x 256 based on facing

			//simplify the math depending on if we moved a full square already
			if(true){
				py2 = floor_bits(py, FXP_DECIMAL_BITS);		//round px to an int
				px2 += (int)((1/slope)*(py2-last_py));		//add to py the movement
				y_distance += float_to_fxp(sqrt( (1+(1/slope)*(1/slope))*(py2-last_py)*(py2-last_py)),FXP_DECIMAL_BITS);	//progress px
			}
			else{
				py += slope_inverse_fxp;
				x_distance += 1;
			}

			printf("y %x,%x,%x,%x,%x,%x\n",x_distance, y_distance, px,py,px2,py2);
		}
	}
	return MAX_INT;
}



