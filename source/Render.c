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
	int i = 0;

	//Iterate the slices of the screen from left to right.
	for(i = 0; i < columns; i++){

		//These variables are for rendering walls behind walls.
		int z = 0;	//iteration nr
		int highest_building = 0;	//height of building hit
		int lowest_pixel = 192;		//pixel height already drawn to. (bottom to top)
		float ray_x = camera.x;		//where did the last ray start from
		float ray_y = camera.y;		//where to shoot the ray from
		float ray_tilt = 0;
		for(z = 0; z < VISION_HEIGHT_ITERATIONS;z ++){
			//calculate the pan angle of the current beam
			float angle = camera.pan + camera.fov_width*(-0.5 + (i+1)/(float)(columns+1));

			//get the building, distance, and face of the ray
			Building building = {0};
			bool is_x_wall = false;
			Pos hitpoint = {0,0,0};

			float distance = Map_get_raycast_distance(ray_x, ray_y, angle, &is_x_wall, &building, highest_building, ray_tilt, &hitpoint,VISION_RAYCAST_RECURSION - 20 * z);

			if(distance >= RAYCAST_ERROR_DISTANCE){
				break;
			}

			//for windows coloring
			float percent_on_wall = is_x_wall ? hitpoint.y : hitpoint.x;
			percent_on_wall -= (int)percent_on_wall;

			//color from palette
			u16 wall_color = color_from_wall(building.color, !is_x_wall);

			//adjust for fish-eye effect
			float adjusted_distance = cos(camera.tilt)*(distance*cos(camera.fov_width*(-0.5+i/(float)columns)));


			//adjust by distance
			u8 distance_shift = clamp((int)(distance/COLOR_FALLOFF_PER_DISTANCE),0,8);
			wall_color += distance_shift*16;	//shift by palette

			//Stats for the render
			float wall_height = building.height;
			float camera_height = camera.z;
			float vert_fov = camera.fov_height;

			//The top bottom real world distance of the screen, when at the distance of the wall
			float screen_height_at_wall = (adjusted_distance * 2*tan(vert_fov/2)) / cos(camera.tilt);

			//real world height of the bottom of the rendered building, relatively to the camera (+angle)
			float bottom_wall = (adjusted_distance * tan(vert_fov/2 - camera.tilt)) + camera_height;
			//calculate the length of the building in pixels on screen.
			int bottom = 192 * (bottom_wall) / screen_height_at_wall;
			int top = 192 * (bottom_wall - wall_height) / screen_height_at_wall;

			//clamp the bottom to the last wall as to not draw over it.
			bottom = bottom <= lowest_pixel ? bottom : lowest_pixel;

			//Draw if rectangle has area.
			if (top <= bottom){
				//if((int)(percent_on_wall*6)%3 == 1)
				//		FillBuilding(bT, clamp(top,0,191), clamp(bottom,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
				//else
				FillRectangle(bT, clamp(top,0,191), clamp(bottom,0,191), (int)(i*(256/(float)columns)),(int)((i+1)*(256/(float)columns))-1, wall_color);
			}

			highest_building = building.height + 1;
			lowest_pixel = top + 1;
	}
}

	//Move the horizon (bring it to me)
	float horizon_height = (HORIZON_DISTANCE * tan(camera.fov_height/2 - camera.tilt)) + camera.z;
	int pixel_height = (91 - (192 * (horizon_height) / (HORIZON_DISTANCE * 2*tan(camera.fov_height/2))));

	if(IS_SCREEN_FLIPPED)
		REG_BG0VOFS = -92 - pixel_height;
	else
		REG_BG0VOFS = 150 + pixel_height;
}

void Render_2D(enum BUFFER_TYPE bT, Camera camera, int left, int top, int right, int bottom){

	int i = 0;
	for(i = 0; i < MAP_WIDTH; i++){
		int j;
		for(j=0; j < MAP_HEIGHT; j++){
			int color = color_from_wall(getBuilding(i,j).color, false);
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
	DrawAngledLine(bT,x/2,y/2,angle,10,5);
	//FillCircle(bT,x+10*cos(6.28+angle),y+10*sin(6.28+angle),3,5);
}

int t = 8200;

void Dont_Render_Sprites(){
	extern u16* char_sprite_ptr;

	oamSet(
	&oamMain,
	0,
	0,
	0,
	0,
	0,
	SpriteSize_64x64,
	SpriteColorFormat_256Color,
	char_sprite_ptr,
	0,
	0,
	true,
	false, //hflip
	false, //vflip
	false
	);
}

void Render_Sprites(int xo, int yo, float slope){
	extern u16* char_sprite_ptr;

	if(IS_SCREEN_FLIPPED)
	{
		oamRotateScale(&oamMain, 0, (slope>0 ? -8200 : (slope<0 ? 8200 : 16400))-5220*atan(slope), 1<<8, 1<<8);
		oamSet(
			&oamMain,
			0,
			256-2*xo-32,
			192-2*yo-32,
			0,
			0,
			SpriteSize_64x64,
			SpriteColorFormat_256Color,
			char_sprite_ptr,
			0,
			0,
			false,
			false, //hflip
			false, //vflip
			false
			);
	}
	else
	{
		oamRotateScale(&oamMain, 0, (slope>0 ? 8200 : (slope<0 ? -8200 : 0))-5220*atan(slope), 1<<8, 1<<8);
		oamSet(
			&oamMain,
			0,
			2*xo-32,
			2*yo-32,
			0,
			0,
			SpriteSize_64x64,
			SpriteColorFormat_256Color,
			char_sprite_ptr,
			0,
			0,
			false,
			false, //hflip
			false, //vflip
			false
			);
	}
}

void draw_Pause()
{
	displayScore();
	displayMaxScore();
	writeMaxScore();
}

void draw_GameOver()
{
	displayScore();
	displayMaxScore();
	writeMaxScore();
}
