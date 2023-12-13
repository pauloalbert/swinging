/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

void ISR_KEYS(){
	extern u16 screen_index;
	extern Camera camera;
	u16 keys = ~REG_KEYINPUT;

	redraw_screen();
}

void initInput(){
	/*REG_KEYCNT = BIT(14) | KEY_LEFT | KEY_RIGHT;
	irqSet(IRQ_KEYS, &ISR_KEYS);
	irqEnable(IRQ_KEYS);*/
}

void handleInput(Camera* camera, Player* player){
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

}
