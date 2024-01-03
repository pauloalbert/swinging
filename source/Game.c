#include "Game.h"

int count;
extern bool power;

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
void slowdown_ISR()
{
	count = count + 1;
	extern float dt;

	if(count<=1)
	{
		dt = DEFAULT_DT/3;					// slow down motion
		SlowTempo();				// slow down music
	}
	else
	{
		if(count==TIME_POWER*2)
		{
			dt = DEFAULT_DT;
			NormalTempo();
		}
		else
		{
			if(count>=(TIME_POWER+TIME_POWER_BACK)*2)
			{
				irqDisable(IRQ_TIMER0);
				power = 1;
				Audio_PlaySoundEX(SFX_BOOP, 255, 127);
			}
		}
	}
}

void slowdown()
{
	power = 0;
	TIMER_DATA(0) = TIMER_FREQ_256(2);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_256 | TIMER_IRQ_REQ;

	irqSet(IRQ_TIMER0, &slowdown_ISR);

	count = 0;
	irqEnable(IRQ_TIMER0);
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

	Audio_PlaySoundEX(SFX_WEBSHOOT, 255, 127);
	Transit(player, grip);
}

void DrawWeb(enum BUFFER_TYPE bT, Camera* camera, Player* player, Grip* grip) {
	int x = 0;
	int y = 0;

	DrawLine(MAIN, 128, 190, x, y, ARGB16(1,0,0,0));
}

void gameLogic(Camera* camera, Player* player, Grip* grip){
	if(grip->ON)
		DrawWeb(MAIN, camera, player, grip);

	if(player->state == Falling){
		Fall(player, grip);
	}

	else if(player->state == Swinging && grip->ON){
		Swing(player, grip);
	}

	camera->x = player->x;
	camera->y = player->y;
	camera->z = player->z + 50;

	CrashTest(player, grip);
}
