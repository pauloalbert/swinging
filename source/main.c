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

void handle_pause(Player player);

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
	fatInitDefault();
	consoleDemoInit();
	readMaxScore();

	REG_POWERCNT &= ~POWER_SWAP_LCDS;

	Struct_Init(&camera, &player, &grip);
	Map_Init();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	char_sprite_ptr = P_Graphics_setup_sprites(char_sprite_ptr);
	Audio_Init();

	swap_palettes(0);
	redraw_screen();
	redraw_screen(); //draw twice to render
	draw_Pause();

	initScore();

	Audio_PlayMusic();
	mmPause();
	scanKeys();

	while(1){

		handleInput(&camera, &player, &grip);
		if(player.state != Paused)
		{
			gameLogic(&camera, &player, &grip);
		}

		redraw_screen();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
	}
	oamFreeGfx(&oamMain, char_sprite_ptr);
}
