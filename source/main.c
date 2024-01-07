#include <math.h>
#include <nds.h>
#include <fat.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
#include "Game.h"
#include "Score.h"


#define	RED ARGB16(1,31,0,0)

	float pan;
	float tilt;

	float fov_width;
	float fov_height;
Camera camera= {100,140,60,3.141592*10/180.,-3.141592*10/180.,3.141592*70/180.,3.141592*52/180.};
Player player = {100,140,60, 0,0,0,Paused};
Grip grip = {false,0,0,0,0,0,0,0,0,0,0};

u16* char_sprite_ptr;

int main(void)
{
	//fatInitDefault();   test if works on VM emulator
	consoleDemoInit();
	readMaxScore();

	REG_POWERCNT &= ~POWER_SWAP_LCDS;

	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	P_Graphics_setup_sprites();
	Audio_Init();

	draw_Pause();
	displayMaxScore();
	initScore();
	Audio_PlayMusic();
	mmPause();
	while(1){
		handleInput(&camera, &player, &grip);
		if(player.state != Paused)
		{
		gameLogic(&camera, &player, &grip);
		redraw_screen();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		}
	}
}
