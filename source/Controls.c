/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"

bool power = 1;

extern int score, Msec, Sec, Min, Hour;
extern Player* player;

void ISR_KEYS(){
	irqDisable(IRQ_TIMER0);
	player->state = Paused;
	mmPause();
	draw_Pause();
}

void initInput(){
	REG_KEYCNT = BIT(14) | KEY_START;
	irqSet(IRQ_KEYS, &ISR_KEYS);
	irqEnable(IRQ_KEYS);
}

void handleInput(Camera* camera, Player* player, Grip* grip){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();

	if(player->state == Paused && (keys & KEY_LEFT)) {
			mmResume();
			irqEnable(IRQ_TIMER0);
			player->state = Falling; //should go back to last state
	}

	if(player->state != Paused) {

		if(keys & KEY_B){
			//Restart
			player->x = 100;
			player->y = 140;
			player->z = 60;
			camera->x = 100;
			camera->y = 140;
			camera->z = 60;
			player->vx = 0;
			player->vy = 0;
			player->vz = 0;
			player->state = Paused;
			irqDisable(IRQ_TIMER0);
			printf("%d : %d : %d : %d\n",Hour, Min, Sec, Msec);
			score = 0;
			grip->ON = false;
			redraw_screen();
		}

		if(keys & KEY_A){
			camera->pan = fmod(camera->pan - 0.12, 2*3.141592);
		}

		if(keys & KEY_Y){
			camera->pan = fmod(camera->pan + 0.12, 2*3.141592);
		}

		//if(keys_pressed & KEY_A){
		//	REG_DISPCNT_SUB ^= DISPLAY_BG0_ACTIVE;
		//}

		if((keys_pressed & KEY_X) && power){
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

		try_sling(touch, camera, player, grip);
		}
	}
}
