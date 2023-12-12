#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
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

	camera.tilt = 0;
	while(true) {
		t += 0.1;
		camera.tilt -= sin(2*t)/70.;
		camera.pan -= (((int)(t*100))%10 - 5)/200.;
		swap_buffers(MAIN);
		swap_buffers(SUB);
		//FillRectangle(SUB,0,100,0,100,1);
		Render_3D(MAIN,camera,64);
		Render_2D(SUB,camera,0,0,256,192);
		handleInput(&camera, &player);
		swiWaitForVBlank();
	}
}
