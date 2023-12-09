/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds paulo
 *
 */
#include "Map.h"
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

u16 color_from_wall(int wall_type, bool is_x_wall){
#ifdef FB0
	switch(wall_type){
	case 1:
		if (is_x_wall) return RGB15(15,0,15);
		return RGB15(25,10,25);
	case 2:
		if (is_x_wall) return RGB15(15,5,5);
		return RGB15(25,0,0);
	default:
		return RGB15(0,0,0);
	}
#endif
#ifdef ROTOSCALE
	return wall_type*2 - 1 + is_x_wall;
#endif
}

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
	int i = 0;
	for(i = 0; i < RAYCAST_RECURSION; i++){
		printf("start %d\n",i);
		printf("%d,%d,%d,%d,%d,%d\n",x_distance, y_distance, px,py,px2,py2);
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

void Render_screen(enum BUFFER_TYPE bT, Camera camera, int columns){
	//FillRectangle(MAIN, 0,85,0,255, RGB15(20,25,31));
	//FillRectangle(MAIN, 86,191,0,255, RGB15(20,31,20));
	int i = 0;
	for(i = 0; i < columns; i++){
		float angle = camera.tilt + camera.fov_width*(-0.5 + (i+1)/(float)(columns+1));

		int wall_type = 0;
		int distance = Map_get_raycast_distance(camera.x, camera.y, angle, &wall_type);
		printf("result %x\n", distance);
		u16 wall_color = color_from_wall(wall_type, distance > 0);
		distance = abs(distance);

		int adjusted_distance = (int)(distance*cos(camera.fov_width*(-0.5+i/(float)columns)));

		//should be sourced elsewhere
		float camera_tilt = 30/360;
		float wall_height = 1280;
		float camera_height = 60;

		float vert_fov = 3*camera.fov_width/4;
		float screen_height_at_wall = (adjusted_distance * 2*tan(vert_fov/2)) / cos(camera_tilt);

		float bottom_wall = (adjusted_distance * tan(vert_fov/2 - camera_tilt)) - camera_height;
		int top = 192 * (wall_height + bottom_wall) / screen_height_at_wall;
		int bottom = 192 * bottom_wall / screen_height_at_wall;

		FillRectangle(bT, clamp(bottom,0,191), clamp(top,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
	}
}

int map_border = 0;
void Render_map(enum BUFFER_TYPE bT, Camera player){


	int i = 0;
	for(i = 0; i < MAP_WIDTH; i++){
		int j;
		for(j=0; j < MAP_HEIGHT; j++){

			if(map[coords(i,j,MAP_WIDTH)]) FillRectangle(bT,16*j + map_border,16*(j+1)-1 - map_border,16*i + map_border,16*(i+1)-1- map_border,RGB15(10,10,10));
			else FillRectangle(bT,16*j,16*(j+1)-1,16*i,16*(i+1)-1,RGB15(0,0,0));
		}
	}
	DrawCircle(bT,player.x,player.y,5,RGB15(31,31,31));
	DrawCircle(bT,player.x,player.y,5.5,RGB15(31,31,31));

	DrawAngledLine(bT,player.x,player.y,player.tilt,10,RGB15(31,0,0));
}

inline byte getBuilding(int x, int y){
	return map[coords(x,y,MAP_WIDTH)];
}

inline byte getBuildingFromWorld(float px, float py){
	return getBuilding(round_float(px)>>WORLD_BLOCK_BITS,round_float(py)>>WORLD_BLOCK_BITS);
}

inline byte getBuildingFromFXP(int x, int y){
	return getBuilding(x>>FXP_DECIMAL_BITS, y>>FXP_DECIMAL_BITS);
}


