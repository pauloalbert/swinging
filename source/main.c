#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Maze.h"
#include "Controls.h"
#include "P_Audio.h"
Camera camera= {60,140,0};
Player player = {60,140,0,0,0,0,0,10};
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
	while(1) {
		swap_buffers(MAIN);
		handleInput(&camera, &player);
		Render_screen(MAIN,camera,32);
		swiWaitForVBlank();
	}
}
