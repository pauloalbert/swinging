#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "gameplay.h"

float g = 9.81;				// gravity acceleration

float k = 0.05;				// spring constant
float dl = 0;				// web length delta
float UpOffset = 5;			// initial Offset when changing web (bounce up)
float DOffset = 10;			// initial Offset when Fallinging on web (bounce down)

float dt = 0.1;				// time interval per loop

float a = 0.999;			// rope shortening factor

float cosT = 0;
float sinT = 0;
float cosF = 0;
float sinF = 0;

enum STATE Transit(Player *player, Grip *grip, enum STATE state)
{
	touchPosition touch;
	touchRead(&touch);

	player->vx = (grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	player->vy = (grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	player->vz = (- grip->vd * sinF + grip->d*grip->vtheta * cosF);

// call raycasting and get grip position + distance by inputing touch XXXXXX

	grip->x=80;
	grip->y=160;
	grip->z=100000;
	grip->d=mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));

	if(grip->d<MAXWEB)
	{
		grip->ON = 1;
		grip->d_rest = grip->d - UpOffset;
		dl = grip->d - grip->d_rest;

		if(grip->z<player->z)
			state = Falling;
		else
		{
			state = Swinging;

			cosT = (grip->z - player->z)/grip->d;
			grip->theta = acos( cosT );
			sinT = sin(grip->theta);

			cosF = ( (grip->y<player->y) ? -1 : 1) * (grip->x-player->x) / sqrt(sqr(grip->x-player->x) + sqr(grip->y-player->y) );
			grip->phi = acos( cosF );
			sinF = sin(grip->phi);

			grip->vd = player->vx*sinT*cosF + player->vy*sinT*sinF + player->vz*cosT;
			grip->vtheta = player->vx*cosT*cosF + player->vy*cosT*sinF - player->vz*sinT;
			grip->vphi = - player->vx*sinF + player->vy*cosF;
		}
	}
	else
	{
		grip->ON = 0;
		state = Falling;
	}
	return state;
}

void Swing(Player *player, Grip *grip)
{
	grip->vphi = grip->vphi - 2*grip->vtheta*grip->vphi*cosT/sinT;
	grip->phi = grip->phi + grip->vphi*dt/2;

	cosF = cos(grip->phi);
	sinF = sin(grip->phi);

	grip->vtheta = grip->vtheta - g*sinT*dt/grip->d + sqr(grip->vphi)*cosT*sinT*dt;
	grip->theta = grip->theta + grip->vtheta*dt/2;

	cosT = cos(grip->theta);
	sinT = sin(grip->theta);

	player->x = grip->d*sinT*cosF + grip->x;
	player->y = grip->d*sinT*sinF + grip->y;
	player->z = grip->d*(1-cosT) + grip->z;				// MAYBE NOT 1-cos just cos->->->

	if(grip->d_rest>75)
			{
				grip->d_rest = grip->d_rest*a;
			}

	grip->vd = grip->vd - k * dl * dt;
	grip->d  = grip->d + grip->vd*dt/2;
	dl =  grip->d - grip->d_rest;
}

float FallBounce(Player *player, Grip *grip)
{
	return (grip->z - sqrt(sqr(grip->d+DOffset)-sqr(grip->d*sinT)));
}

enum STATE Fall(Player *player, Grip *grip, enum STATE state)
{
	if(player->z <= FallBounce(player, grip) && grip->ON)
	{
			grip->d = grip->d+DOffset;
			dl = grip->d - grip->d_rest;

			cosT = (grip->z - player->z)/grip->d;
			grip->theta = acos( cosT );
			sinT = sin(grip->theta);

			cosF = ( (grip->y<player->y) ? -1 : 1) * (grip->x-player->x) / sqrt(sqr(grip->x-player->x) + sqr(grip->y-player->y) );
			grip->phi = acos( cosF );
			sinF = sin(grip->phi);

			state = Swinging;
	}
	else
	{
		player->vz = player->vz - g*dt;

		player->x = player->x + player->vx*dt;
		player->y = player->y + player->vy*dt;
		player->z = player->z + player->vz*dt/2;

		if(player->z < 0)
			player->z = 0;
	}
	return state;
}

enum STATE game(Camera *camera, Player *player, Grip *grip, enum STATE state)
{
//---------------------------------------------------------------------------------

	scanKeys();
	u16 keyss = keysHeld();					// handle input ?

	if(keyss == KEY_TOUCH)
	{
	state = Transition;

	// HERE erase previous web XXXXXXXX

	state = Transit(player, grip, state);
	}


	if(state == Falling)
		state = Fall(player, grip, state);

	if(state == Swinging)
		Swing(player, grip);

	camera->x = player->x;
	camera->y = player->y;
	camera->z = player->z;

	return state;
}

