#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"

	float pan;
	float tilt;

	float fov_width;
	float fov_height;
Camera camera= {400,400,0,3.141592*0/180.,3.141592*0/180.,3.141592*70/180.,3.141592*52/180.};
Player player = {60,140,0};
extern Goal goal;
int main(void)
{
	consoleDemoInit();
	Map_Init();
	initInput();
	P_Graphics_setup_main();
	Audio_Init();

	u8 i = 0;
	u8 j = 0;
	int building = -1;
	//int distance = Map_get_raycast_distance(300,300,0.1,&building);
	//printf("%x, %d\n", distance, building);

	Render_screen(MAIN,camera,1);
	while(0) {
		swap_buffers(MAIN);
		handleInput(&camera, &player);
		Render_screen(MAIN,camera,32);
		swiWaitForVBlank();
	}
}
