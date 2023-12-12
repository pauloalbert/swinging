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

		int x_wall_type = 0;
		int y_wall_type = 0;
		float x_wall_distance = Map_get_raycast_distance(camera.x, camera.y, angle, true, &x_wall_type);
		float y_wall_distance = Map_get_raycast_distance(camera.x, camera.y, angle, false, &y_wall_type);

		float distance = x_wall_distance < y_wall_distance ? x_wall_distance : y_wall_distance;

		//int color_falloff = ((int)distance / 30) & 0x1f;
		u16 wall_color = color_from_wall(x_wall_distance < y_wall_distance ? x_wall_type : y_wall_type, x_wall_distance > y_wall_distance);

		float adjusted_distance = (distance*cos(camera.fov_width*(-0.5+i/(float)columns)));

		//should be sourced elsewhere
		float camera_tilt = 30/360;
		float wall_height = 128;
		float camera_height = 60;

		float vert_fov = 3*camera.fov_width/4;
		float screen_height_at_wall = (adjusted_distance * 2*tan(vert_fov/2)) / cos(camera_tilt);

		float bottom_wall = (adjusted_distance * tan(vert_fov/2 - camera_tilt)) - camera_height;
		int top = 192 * (wall_height + bottom_wall) / screen_height_at_wall;
		int bottom = 192 * bottom_wall / screen_height_at_wall;
		FillRectangle(bT, clamp(bottom,0,191), clamp(top,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
	}
}

void Render_2D(enum BUFFER_TYPE bT, Camera camera, int left, int top, int right, int bottom){

	int map_border = 0;
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
		}
	}
	int x = convert_ranges(camera.x, 0, MAP_WIDTH << FXP_DECIMAL_BITS, left, right);
	int y = convert_ranges(camera.y, 0, MAP_HEIGHT << FXP_DECIMAL_BITS, top, bottom);
	DrawCircle(bT,x,y,4,3);
	DrawCircle(bT,x,y,3.5,3);
	DrawAngledLine(bT,x,y,camera.pan,10,5);
}

