/*
 * maze.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds paulo
 *
 */
#include "Maze.h"
int maze[] = {1,1,1,1,1, 1,1,1,1,1,
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

			int current_wall = maze[coords((((px)>>WORLD_BLOCK_BITS) - !facing_right),((int)float_py)>>WORLD_BLOCK_BITS,MAP_WIDTH)];
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

			int current_wall = maze[coords(((int)(float_px)>>WORLD_BLOCK_BITS),(py>>WORLD_BLOCK_BITS)-!facing_down,MAP_WIDTH)];
			if(current_wall != 0){
				if(wall_type) *(wall_type) = current_wall;
				return distance;
			}
		}
		return 1000000;

	}

}

void Render_screen(enum BUFFER_TYPE bT, Camera camera, int columns){
	//FillRectangle(MAIN, 0,85,0,255, RGB15(20,25,31));
	//FillRectangle(MAIN, 86,191,0,255, RGB15(20,31,20));
	int i = 0;
	for(i = 0; i < columns; i++){
		float angle = camera.tilt + camera.fov_width*(-0.5 + (i+1)/(float)(columns+1));

		int x_wall_type = 0;
		int y_wall_type = 0;
		float x_wall_distance = Map_get_raycast_distance(camera.x, camera.y, angle, true, &x_wall_type);
		float y_wall_distance = Map_get_raycast_distance(camera.x, camera.y, angle, false, &y_wall_type);

		float distance = x_wall_distance < y_wall_distance ? x_wall_distance : y_wall_distance;

		//int color_falloff = ((int)distance / 30) & 0x1f;
		u16 wall_color = color_from_wall(x_wall_distance < y_wall_distance ? x_wall_type : y_wall_type, x_wall_distance > y_wall_distance);

		float adjusted_distance = distance*cos(camera.fov_width*(-0.5+i/(float)columns));

		//should be sourced elsewhere
		float camera_tilt = 30/360;
		float wall_height = 128;
		float camera_height = 60;

		float vert_fov = 3*camera.fov_width/4;
		float screen_height_at_wall = (adjusted_distance * 2*tan(vert_fov/2)) / cos(camera_tilt);

		float bottom_wall = (adjusted_distance * tan(vert_fov/2 - camera_tilt)) - camera_height;
		int top = 192 * (wall_height + bottom_wall) / screen_height_at_wall;
		int bottom = 192 * bottom_wall / screen_height_at_wall;
		if(i == 0){
			//printf("%.2f,%.2f, %.2f, %.2f\n", screen_height_at_wall,adjusted_distance, wall_height/screen_height_at_wall, bottom_wall/screen_height_at_wall);
			printf("%.2f,%.2f, %d, %d\n", screen_height_at_wall,adjusted_distance, top, bottom);

		}
		FillRectangle(bT, clamp(bottom,0,191), clamp(top,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);

		//float half_length_wall = 150/(1+(distance*cos(MAZE_FOV*(-0.5+i/(float)columns)))/WORLD_BLOCK_SIZE);
		//FillRectangle(bT, clamp(96-(int)half_length_wall,0,192), clamp(96+(int)half_length_wall,0,192), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);

	}
}

int map_border = 0;
void Render_map(enum BUFFER_TYPE bT, Camera player){


	int i = 0;
	for(i = 0; i < MAP_WIDTH; i++){
		int j;
		for(j=0; j < MAP_HEIGHT; j++){

			if(maze[coords(i,j,MAP_WIDTH)]) FillRectangle(bT,16*j + map_border,16*(j+1)-1 - map_border,16*i + map_border,16*(i+1)-1- map_border,RGB15(10,10,10));
			else FillRectangle(bT,16*j,16*(j+1)-1,16*i,16*(i+1)-1,RGB15(0,0,0));
		}
	}
	DrawCircle(bT,player.x,player.y,5,RGB15(31,31,31));
	DrawCircle(bT,player.x,player.y,5.5,RGB15(31,31,31));

	DrawAngledLine(bT,player.x,player.y,player.tilt,10,RGB15(31,0,0));
}

byte getBuilding(int x, int y){
	return map[coords(x,y,MAP_WIDTH)];
}

byte getBuildingFromWorld(float px, float py){
	return getBuilding(round_float(px)>>WORLD_BLOCK_BITS,round_float(py)>>WORLD_BLOCK_BITS);
}


