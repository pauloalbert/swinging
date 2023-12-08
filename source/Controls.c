/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

int startHeldKeys = -1;

void initInput(){

	scanKeys();
	scanKeys();
	startHeldKeys = keysHeld() & 0xff;
	printf("init keys: %d\n", startHeldKeys);
}

void _handleInput(Camera* camera, Player* player, u16 keys, u16 keys_pressed){
	if(keys_pressed & KEY_START){
		int seed = (int)((player->x*100)+(player->y*100000));
		set_seed(seed);
		printf("RESET: new seed - %d\n",seed);
		u16 old = shuffleGoal();

		printf("The goal was in: %d, %d\n",old&0xf, old >> 8);
		printf("Final score: %d\n\n", player->score);
		player->x = 60;
		player->y = 140;
	}

		inline float mod_float(float x, int amount){
			return fmod(fmod(x, amount) + amount,amount);
	}
	player->x = mod_float(player->x, MAZE_WIDTH<<MAZE_BLOCK_BITS);

	player->y = mod_float(player->y, MAZE_HEIGHT<<MAZE_BLOCK_BITS);
	//
	if(keys_pressed & KEY_A)
		tryGoal(player->x,player->y,player->angle, player);
}

void handleInput(Camera* camera, Player* player){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();
	if(startHeldKeys != -1 && keys == startHeldKeys)
		return;
	startHeldKeys = -1;

	if(keys & KEY_L){
			player->torque = clamp_float(player->torque - TURN_GAIN, -TURN_MAX, -TURN_MIN);
			player->angle += player->torque;
	}
		if(keys & KEY_R){

		player->torque = clamp_float(player->torque + TURN_GAIN, TURN_MIN, TURN_MAX);
		player->angle += player->torque;
	}

	float x_vec;
	float y_vec;
	if(keys & KEY_LEFT){
		x_vec += SPEED_STRAFE * sin(player->angle);
		y_vec -= SPEED_STRAFE * cos(player->angle);
	}
	if(keys & KEY_RIGHT){
		x_vec -= SPEED_STRAFE * sin(player->angle);
		y_vec += SPEED_STRAFE * cos(player->angle);
	}
	if(keys & KEY_UP){
		x_vec += SPEED_FORWARD * cos(player->angle);
		y_vec += SPEED_FORWARD * sin(player->angle);
		}
	if(keys & KEY_DOWN){
		x_vec -= SPEED_FORWARD * cos(player->angle);
		y_vec -= SPEED_FORWARD * sin(player->angle);
		}

	x_vec += player->x_vel * (player->x_vel * x_vec < 0 ? 0.5 : 1);
	y_vec += player->y_vel * (player->y_vel * y_vec < 0 ? 0.5 : 1);;
	if(!getMazeFromWorld(player->x+x_vec,player->y))
		player->x += x_vec;
	else
		player->x_vel = 0;
	if(!getMazeFromWorld(player->x,player->y+y_vec))
			player->y += y_vec;
		else
			player->y_vel = 0;

	x_vec -= player->x_vel;
	y_vec -= player->y_vel;

	player->x_vel += x_vec/4;
	player->y_vel += y_vec/4;

	player->x_vel = clamp_float(player->x_vel, -SPEED_MAX, SPEED_MAX);
	player->y_vel = clamp_float(player->y_vel, -SPEED_MAX, SPEED_MAX);
	if(x_vec == 0)
		player->x_vel /= 1.5;

	if(y_vec == 0)
		player->y_vel /= 1.5;

	if(keys & KEY_B)
		MAZE_FOV = (MAZE_FOV + MAZE_FOV_MAX) / 2;
	else
		MAZE_FOV = MAZE_FOV_MIN;

	if(keys & KEY_Y)
			PULLBACK = (2* PULLBACK + PULLBACK_MAX) / 3;
		else
			PULLBACK = (PULLBACK + 2* PULLBACK_MIN) / 3;

	_handleInput(camera, player, keys, keys_pressed);
	//set camera based on player:
	camera->angle = player->angle;
	if(keys & KEY_X)
		camera->angle += 3.141592;
	camera->x = round_float(player->x-PULLBACK*cos(player->angle));
	camera->y = round_float(player->y-PULLBACK*sin(player->angle));


}
