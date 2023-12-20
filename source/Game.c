#include "Game.h"

void redraw_screen(){
	extern Camera camera;
	swap_buffers(SUB);
	swap_buffers(MAIN);
	//FillRectangle(SUB,0,100,0,100,1);
	Render_3D(MAIN,camera,32);
	Render_2D(SUB,camera,0,0,128,92);
}

//try slinging with a touch position.
//(moved to its own file so that controls doesn't have to deal with many other files)
void try_sling(touchPosition t, Camera* camera, Grip* grip){
	Pos pos = {0,0,0};

	float dist = get_grip_position(*camera,t, &pos);

	if(dist >= 1000000){
		grip->ON = false;
	}
	else{
		grip->x = pos.x;
		grip->y = pos.y;
		grip->z = pos.z;
		grip->ON = true;
	}
	//TODO: call here functions that need pos and dist.
}

void GameLogic(Camera* camera,Player* player,  Grip* grip){
	if(!grip->ON){
		player->vx = 0;
		player->vy = 0;
		player->vz = 0;
	}
	else{
		player->vz = 0;
	}

	player->x += player->vx;
	player->y += player->vy;
	player->z += player->vz;

	camera->x = player->x;
	camera->y = player->y;
	camera->z = player->z + 50;
}
