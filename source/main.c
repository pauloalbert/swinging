#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
#include "Game.h"
#include "gameplay.h"

	float pan;
	float tilt;

	float fov_width;
	float fov_height;

Camera camera= {140,110,100,2*3.141592*10/180.,-3.141592*100/180.,3.141592*70/180.,3.141592*52/180.};
Player player = {140,110,50,0,0,0,0};
Grip grip = {0,0,0,0,0,0,0,0,0,0,0};

int main(void)
{
	consoleDemoInit();
	REG_POWERCNT &= ~POWER_SWAP_LCDS;
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	Audio_Init();

	while(1) {
		t += 0.1;
		camera.tilt += sin(2*t)/30.;
		camera.pan -= (((int)(t*10))%10 - 5)/400.;
		redraw_screen();
		game_loop(&camera, &player, &grip);

		//if(state != GameOff)
		//	printf("%lf\n",player.z);

		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
