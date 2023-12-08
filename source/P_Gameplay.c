#include "P_Gameplay.h"

int timer = 0;
int last_guage = 0;

void Give_Hint(const Player* player,const Goal* goal){
	timer += 1;
	if(timer*timer < last_guage/100)
		return;

	float dx = (goal->x<<MAZE_BLOCK_BITS) - player->x;
	float dy = (goal->y<<MAZE_BLOCK_BITS) - player->y;

	float angle = atan2(dy,dx) - player->angle;
	float distance = (dx*dx + dy*dy);
	last_guage = clamp(distance/80,150,60000);
	timer = 0;
	u8 volume = (u8)clamp(255-(distance/1000),20,255);
	if(cos(angle) < 0.5) volume >> 1;
	if(cos(angle) < 0) volume >> 1;
	Audio_PlaySoundEX( SFX_LASER , volume, (u8)(125 * sin(angle) + 128));

}
