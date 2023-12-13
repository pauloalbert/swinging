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
Camera camera= {90,90,0,3.141592*10/180.,3.141592*100/180.,3.141592*70/180.,3.141592*52/180.};
Player player = {60,140,0};
int main(void)
{
	consoleDemoInit();
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	P_Graphics_setup_sub();
	Audio_Init();
	float t = 0;
	camera.tilt = 0;
	while(1) {
		t += 0.1;
		camera.tilt -= sin(2*t)/70.;
		camera.pan -= (((int)(t*10))%10 - 5)/400.;
		redraw_screen();
		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
