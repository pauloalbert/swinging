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

void handleInput(Camera* camera, Player* player, Grip* grip){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();
	u16 keys_up = keysUp();
	touchPosition touch;
	touchRead(&touch);

	if(player->state != Paused) {

		if(keys & KEY_START){ //alternative to interrupt
			irqDisable(IRQ_TIMER0);
			player->state = Paused;
			mmPause();
			draw_Pause();
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

		if(touch.px || touch.py){


			if(IS_SCREEN_FLIPPED){
				touch.px = 255 - touch.px;
				touch.py = 191 - touch.py;
			}

		try_sling(touch, camera, player, grip);
		}
	}
	else
	{
		// if paused
		if(player->live)
		{
			// resume
			if(keys & KEY_START) { //put touchscreen zone where input is
				Audio_PlayMusic();
				irqEnable(IRQ_TIMER0);
				player->state = Falling;
				swap_palettes(0);
				consoleClear();
				REG_BG2PA_SUB = -128;
				REG_BG2PC_SUB = 0;
				REG_BG2PB_SUB = 0;
				REG_BG2PD_SUB = -120;

				REG_BG2X_SUB = 128*256;
				REG_BG2Y_SUB = 120*192;
			}
			// restart
			if(keys & KEY_SELECT) { //put touchscreen zone where input is
				Struct_Init(camera, player, grip);
				player->state=Falling;
				grip->ON = false;
				score = 0;
				swap_palettes(0);

				player->state = Falling;
				Audio_PlayMusic();
				irqEnable(IRQ_TIMER0);
				consoleClear();
				REG_BG2PA_SUB = -128;
				REG_BG2PC_SUB = 0;
				REG_BG2PB_SUB = 0;
				REG_BG2PD_SUB = -120;

				REG_BG2X_SUB = 128*256;
				REG_BG2Y_SUB = 120*192;
			}
		}
		else // if game over
		{
			// restart
			if(keys & KEY_SELECT) { //put touchscreen zone where input is
				player->x = 100;
				player->y = 140;
				player->z = 60;
				camera->x = 100;
				camera->y = 140;
				camera->z = 60;
				camera->pan = 3.141592*10/180.;
				player->vx = 0;
				player->vy = 0;
				player->vz = 0;
				player->live = true;
				swap_palettes(0);

				player->state = Falling;
				Audio_PlayMusic();
				irqEnable(IRQ_TIMER0);
				consoleClear();
				REG_BG2PA_SUB = -128;
				REG_BG2PC_SUB = 0;
				REG_BG2PB_SUB = 0;
				REG_BG2PD_SUB = -120;

				REG_BG2X_SUB = 128*256;
				REG_BG2Y_SUB = 120*192;
				Map_Init();
			}

		}
	}
}
