/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

void initInput(){
}

void handleInput(Camera* camera, Player* player){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();

}
