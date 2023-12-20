/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

void ISR_KEYS(){
	/*extern Camera camera;
	u16 keys = ~REG_KEYINPUT;
*/
	//...
}

void initInput(){
	/*REG_KEYCNT = BIT(14) | KEY_LEFT | KEY_RIGHT;
	irqSet(IRQ_KEYS, &ISR_KEYS);
	irqEnable(IRQ_KEYS);*/
}

void handleInput(Camera* camera, Player* player, Grip* grip){
	extern u16 screen_index;
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();

	if(keys & KEY_LEFT){
		camera->pan -= 0.12;
	}
	if(keys & KEY_RIGHT){
		camera->pan += 0.12;
	}
	if(keys_pressed & KEY_A){
		REG_DISPCNT_SUB ^= DISPLAY_BG0_ACTIVE;
	}

	// read the touch and try slinging
	if(keys_pressed & KEY_TOUCH){
		touchPosition touch;
		touchRead(&touch);


		if(IS_SCREEN_FLIPPED){
			touch.px = 255 - touch.px;
			touch.py = 191 - touch.py;
		}
		try_sling(touch, camera, grip);
	}

}
