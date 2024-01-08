#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
#include "Game.h"

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
	consoleDemoInit();
	REG_POWERCNT &= ~POWER_SWAP_LCDS;
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	char_sprite_ptr = P_Graphics_setup_sprites(char_sprite_ptr);
	Audio_Init();

	draw_Pause();
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
	oamFreeGfx(&oamMain, char_sprite_ptr);
}
