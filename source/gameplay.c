/*
 * gameplay.c
 *
 *  Created on: Dec 20, 2023
 *      Author: nds
 */
#include "gameplay.h"

Pos RopeTension(Player* player, Pos* grip){
	float dx = grip->x - player->x;
	float dy = grip->y - player->y;
	float dz = grip->z - player->z;

	float length = sqrt(sqr(dx)+sqr(dy)+sqr(dz));
	float force = 0.1 * (length - 128);
	Pos acceleration = {dx*force, dy*force, dz*force};
	return acceleration;
}
