#include "Game.h"

void redraw_screen(){
	extern Camera camera;
	swap_buffers(SUB);
	swap_buffers(MAIN);
	//FillRectangle(SUB,0,100,0,100,1);

	Render_Sprites(camera);
	Render_3D(MAIN,camera,32);
	Render_2D(SUB,camera,0,0,128,92);
}

// slows down for 2 second
void slowdown_ISR(bool slow)
{
	extern float dt;
	if(slow)
	{
		dt = dt * 3;
		irqDisable(IRQ_TIMER0);
		NormalTempo();
	}
	else
	{
		slow = 1;
		dt = dt/3;
		SlowTempo();
	}
}

//try slinging with a touch position.
//(moved to its own file so that controls doesn't have to deal with many other files)
void try_sling(touchPosition t,Camera* camera, Grip* grip){
	Pos pos = {0,0,0};

	float dist = get_grip_position(*camera,t, &pos);

	//If no grip found
	if(dist >= RAYCAST_ERROR_DISTANCE){
		grip->ON = false;
		return;
	}

	//TODO: call here functions that need pos and dist.
	//printf("(%.2f,%.2f,%.2f) - %.2f\n",pos.x,pos.y,pos.z, dist);


	grip->ON = true;

	extern Player player;
	do_sling(&player, grip, pos);
}

void do_sling(Player* player, Grip* grip, Pos pos){

	grip->x = pos.x;
	grip->y = pos.y;
	grip->z = pos.z;

	player->state = Falling;
	Transit(player, grip);
}

void gameLogic(Camera* camera, Player* player, Grip* grip){
	if(player->state == Falling){
		Fall(player, grip);
	}

	else if(player->state == Swinging){
		Swing(player, grip);
	}

	camera->x = player->x;
	camera->y = player->y;
	camera->z = player->z + 50;
}
