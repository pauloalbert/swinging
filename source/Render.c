/*
 * Render.c
 *
 *  Created on: Dec 9, 2023
 *      Author: nds
 */
#include "Render.h"
void Render_3D(enum BUFFER_TYPE bT, Camera camera, int columns){
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

void Render_2D(enum BUFFER_TYPE bT, Camera camera, int left, int top, int right, int bottom){

	int map_border = 0;
	int i = 0;
	for(i = 0; i < MAP_WIDTH; i++){
		int j;
		for(j=0; j < MAP_HEIGHT; j++){

			if(map[coords(i,j,MAP_WIDTH)]) FillRectangle(bT,16*j + map_border,16*(j+1)-1 - map_border,16*i + map_border,16*(i+1)-1- map_border,RGB15(10,10,10));
			else FillRectangle(bT,16*j,16*(j+1)-1,16*i,16*(i+1)-1,RGB15(0,0,0));
		}
	}
	DrawCircle(bT,camera.x,camera.y,5,RGB15(31,31,31));
	DrawCircle(bT,camera.x,camera.y,5.5,RGB15(31,31,31));

	DrawAngledLine(bT,camera.x,camera.y,camera.tilt,10,RGB15(31,0,0));
}

