/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"

bool power = 1;
float dpan = 0;

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
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();

	if(player->state == Paused && (keys & KEY_START)) {
			mmResume();
			player->state = Swinging;
	}

	if(player->state != Paused) {
	if(keys & KEY_LEFT){
		camera->pan -= 0.12;
	}

	if(keys & KEY_RIGHT){
		camera->pan += 0.12;
	}

	if(keys_pressed & KEY_A){
		REG_DISPCNT_SUB ^= DISPLAY_BG0_ACTIVE;
	}

	if((keys_pressed & KEY_B) && power){
		slowdown();
		}


	// read the touch and try slinging
	touchPosition touch;
	touchRead(&touch);

	if(touch.px || touch.py){


		if(IS_SCREEN_FLIPPED){
			touch.px = 255 - touch.px;
			touch.py = 191 - touch.py;
		}

		try_sling(touch, camera, grip);
	}
	}

	//camera->pan = (player->vx ? atan(player->vy/player->vx) : 0) + dpan;

}
