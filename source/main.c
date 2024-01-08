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
			printf("%f\n",player.z);
		}

		redraw_screen();
		handle_pause(player);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
	}
	oamFreeGfx(&oamMain, char_sprite_ptr);
}

void handle_pause(Player player){
	if(player.live == false){
		bool first_time = swap_palettes(2);
		//this is how we fill sub screen
		FillRectangle(SUB,0,120,0,128,RGB15(10,10,10));
		if(first_time){
			consoleClear();
			printf("\n           =~ YOU DIED ~=\n");
		}
	}
	else if(player.state == Paused ){
		bool first_time = swap_palettes(1);

		if(first_time){
			consoleClear();
			printf("\n       =~ GAME PAUSED ~=\n");
		}
	}
	else{
		swap_palettes(0);
	}
}
