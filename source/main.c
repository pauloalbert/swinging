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
Camera camera= {400,400,0,3.141592*10/180.,3.141592*100/180.,3.141592*70/180.,3.141592*52/180.};
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
	//int distance = Map_get_raycast_distance(300,300,0.1,&building);
	//printf("%x, %d\n", distance, building);
	printf("%f,%f\n",camera.fov_width,camera.fov_height);
	printf("%x %x\n",REG_DISPCNT_SUB,BGCTRL_SUB[0]);
	camera.tilt = 0;
	Render_3D(MAIN,camera,32);

	Render_2D(SUB,camera,0,0,128,92);
	printf("HI %p %p %p\n", bgGetGfxPtr(4), bgGetMapBase(4), bgGetTileBase(4));
	FillCircle(MAIN,100,100,10,3);
	while(1) {
		//t += 0.1;
		//camera.tilt -= sin(2*t)/70.;
		//camera.pan -= (((int)(t*100))%10 - 5)/200.;
		//redraw_screen();
		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
