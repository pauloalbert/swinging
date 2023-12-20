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
	if(!grip->ON || player->z > grip->z){
		//player->vx = 0;
		//player->vy = 0;
		player->vz += -5;
	}
	else{
		float tension = 0.25;
		float dx = grip->x - player->x;
		float dy = grip->y - player->y;
		float dz = grip->z - player->z;

		float length = sqrt(sqr(dx) + sqr(dy) + sqr(dz));
		float pull = tension*(length - 64);
		if(pull < 0)
			pull = 0;
		pull = 1;

		printf("%.2f %.2f %.2f \n",length, pull, dz);

		player->vx += pull * dx / length;
		player->vy += pull * dy / length;
		player->vz += pull * dz / length;
	}

	player->vx *= 0.95;
	player->vy *= 0.95;
	player->vz *= 0.95;
	player->x += player->vx;
	player->y += player->vy;
	player->z += player->vz;
	if(player->z < 0){
		player->z = 0;
		player->vz = 0;
	}

	camera->x = player->x;
	camera->y = player->y;
	camera->z = player->z + 50;
}
