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
Camera camera;
Player player;
Grip grip;

u16* char_sprite_ptr;

int main(void)
{
	//fatInitDefault();   //test if works on VM emulator
	consoleDemoInit();
	readMaxScore();

	REG_POWERCNT &= ~POWER_SWAP_LCDS;

	Struct_Init(&camera, &player, &grip);
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	P_Graphics_setup_sprites();
	Audio_Init();

	redraw_screen();
	redraw_screen(); //draw twice to render
	draw_Pause();
	displayMaxScore();

	initScore();

	Audio_PlayMusic();
	mmPause();
	scanKeys();

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
