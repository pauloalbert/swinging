/*
 * Render.c
 *
 *  Created on: Dec 9, 2023
 *      Author: nds
 */
#include "Render.h"

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



void Render_3D(enum BUFFER_TYPE bT, Camera camera, int columns){
	//FillRectangle(MAIN, 0,85,0,255, RGB15(20,25,31));
	//FillRectangle(MAIN, 86,191,0,255, RGB15(20,31,20));
	int i = 0;
	for(i = 0; i < columns; i++){
		float angle = camera.pan + camera.fov_width*(-0.5 + (i+1)/(float)(columns+1));

		int wall_type = 0;
		bool is_x_wall = false;
		float distance = Map_get_raycast_distance(camera.x, camera.y, angle, &is_x_wall, &wall_type, 0, 0);

		u16 wall_color = color_from_wall(wall_type, !is_x_wall);

		float adjusted_distance = cos(camera.tilt)*(distance*cos(camera.fov_width*(-0.5+i/(float)columns)));

		//should be sourced elsewhere
		float wall_height = 128;
		float camera_height = 60;

		float vert_fov = 3*camera.fov_width/4;
		float screen_height_at_wall = (adjusted_distance * 2*tan(vert_fov/2)) / cos(camera.tilt);

		float bottom_wall = (adjusted_distance * tan(vert_fov/2 - camera.tilt)) - camera_height;
		int top = 192 * (wall_height + bottom_wall) / screen_height_at_wall;
		int bottom = 192 * bottom_wall / screen_height_at_wall;
		FillRectangle(bT, clamp(bottom,0,191), clamp(top,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
	}
}

void Render_2D(enum BUFFER_TYPE bT, Camera camera, int left, int top, int right, int bottom){

	int i = 0;
	for(i = 0; i < MAP_WIDTH; i++){
		int j;
		for(j=0; j < MAP_HEIGHT; j++){
			int color = color_from_wall(getBuilding(i,j), false);
			int x = convert_ranges(i,0,MAP_WIDTH,left,right);
			int y = convert_ranges(j,0,MAP_HEIGHT,top,bottom);
			int x1 = convert_ranges(i+1,0,MAP_WIDTH,left,right) - 1;
			int y1 = convert_ranges(j+1,0,MAP_HEIGHT,top,bottom) - 1;
			if(color < 300) FillRectangle(bT,y,y1,x,x1,color);
			else DrawRectangle(bT,y,y1,x,x1,4);
		}
	}
	int x = convert_ranges(camera.x, 0, MAP_WIDTH << WORLD_BLOCK_BITS, left, right);
	int y = convert_ranges(camera.y, 0, MAP_HEIGHT << WORLD_BLOCK_BITS, top, bottom);
	FillCircle(bT,x,y,4,3);
	float angle = (camera.pan);
	DrawAngledLine(bT,x,y,angle,50,5);
	//FillCircle(bT,x+10*cos(6.28+angle),y+10*sin(6.28+angle),3,5);
}

