#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
#include "Game.h"
	float pan;
	float tilt;

	float fov_width;
	float fov_height;
Camera camera= {90,90,60,3.141592*10/180.,-3.141592*10/180.,3.141592*70/180.,3.141592*52/180.};
Player player = {60,140,0};
Grip grip = {false,1,1,1,0,0,0,0,0,0,0};
int main(void)
{
	consoleDemoInit();
	REG_POWERCNT &= ~POWER_SWAP_LCDS;
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	Audio_Init();
	float t = 0;
	//camera.tilt = 0;
	while(1){
		redraw_screen();
		handleInput(&camera, &player, &grip);
		swiWaitForVBlank();
	}
}
