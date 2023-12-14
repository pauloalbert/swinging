#include "Game.h"

void redraw_screen(){
	extern Camera camera;
	//swap_buffers(SUB);
	swap_buffers(MAIN);
	//FillRectangle(SUB,0,100,0,100,1);
	Render_3D(MAIN,camera,32);
	//Render_2D(SUB,camera,0,0,128,92);
}

//try slinging with a touch position.
//(moved to its own file so that controls doesn't have to deal with many other files)
void try_sling(touchPosition t){
	extern Camera camera;
	Pos pos = {0,0,0};

	float dist = get_grip_position(camera,t, &pos);
	printf("(%.2f,%.2f,%.2f) - %.2f\n",pos.x,pos.y,pos.z, dist);
}
