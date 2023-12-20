#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "gameplay.h"

float g = 9.81;				// gravity acceleration

float k = 0;				// spring constant
float dl = 0;				// web length delta
float UpOffset = 0;			// initial Offset when changing web (bounce up)
float DOffset = 0;			// initial Offset when Fallinging on web (bounce down)

float dt = 0.5;				// time interval per loop

float a = 0.999;			// rope shortening factor

float cosT = 0;
float sinT = 0;
float cosF = 0;
float sinF = 0;

void Transit(Player* player, Grip* grip)
{
	touchPosition touch;
	touchRead(&touch);

	DrawLine(MAIN, 128/2, 191/2, touch.px/2, touch.py/2, ARGB16(1,31,31,31));

	player->vx = 0; //(grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	player->vy = 0; //(grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	player->vz = 0; //(- grip->vd * sinF + grip->d*grip->vtheta * cosF);

	Pos pos = try_sling(touch);

	grip->x = pos.x;
	grip->y = pos.y;
	grip->z = pos.z;

	grip->d = mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));
	printf("%f\n%f\n%f\n",grip->d,grip->x,grip->z);

	if(grip->x || grip->y || grip->z)
	{
		grip->ON = 1;
		grip->d_rest = grip->d - UpOffset;
		dl = grip->d - grip->d_rest;

		//if(grip->z<player->z)
		//	{
		//	printf("Too low");
		//	state = Falling;
		//	}
		//else
		//{
			player->state = Swinging;

			cosT = (grip->z - player->z)/grip->d;
			grip->theta = ( ( grip->y<player->y ) ? -1 : 1) * acos( cosT );
			sinT = sin(grip->theta);

			cosF = (grip->x-player->x) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
			grip->phi = ( ( grip->y<player->y ) ? -1 : 1) * acos( cosF );
			sinF = sin(grip->phi);

			grip->vd = player->vx*sinT*cosF + player->vy*sinT*sinF + player->vz*cosT;
			grip->vtheta = player->vx*cosT*cosF + player->vy*cosT*sinF - player->vz*sinT;
			grip->vphi = - player->vx*sinF + player->vy*cosF;
		//}
	}
	else
	{
		grip->ON = 0;
		player->state = Falling;
	}
}

float FallBounce(Grip* grip)
{
	return (grip->z - sqrt(sqr(grip->d+DOffset)-sqr(grip->d*sinT)));
}

void Fall(Player* player, Grip* grip)
{
	if(player->z <= FallBounce(grip) && grip->ON)
	{
			grip->d = mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));
			grip->d_rest = grip->d+DOffset;
			dl = grip->d - grip->d_rest;

			cosT = (grip->z - player->z)/grip->d;
			grip->theta = ( ( grip->y<player->y ) ? 1 : -1) * acos( cosT );
			sinT = sin(grip->theta);

			cosF = (grip->x-player->x) / sqrt(sqr(grip->x-player->x) + sqr(grip->y-player->y) );
			grip->phi = ( ( grip->x<player->x ) ? 1 : -1) * acos( cosF );
			sinF = sin(grip->phi);

			player->state = Swinging;
	}
	else
	{
		//player->vz = player->vz - g*dt;

		player->x = player->x + player->vx*dt;
		player->y = player->y + player->vy*dt;
		player->z = player->z + player->vz*dt;
	}

}

void Swing(Player* player, Grip* grip)
{
	grip->vphi = grip->vphi - 2*grip->vtheta*grip->vphi*cosT/sinT*dt;
	grip->phi = grip->phi + grip->vphi*dt;

	cosF = cos(grip->phi);
	sinF = sin(grip->phi);

	grip->vtheta = grip->vtheta - g*sinT*dt/grip->d + sqr(grip->vphi)*cosT*sinT*dt;
	grip->theta = grip->theta + grip->vtheta*dt;

	cosT = cos(grip->theta);
	sinT = sin(grip->theta);

	player->x = -grip->d*sinT*cosF + grip->x; 	//clamp_float(-grip->d*sinT*cosF + grip->x, 0, MAP_WIDTH << WORLD_BLOCK_BITS);
	player->y = -grip->d*sinT*sinF + grip->y; 	//clamp_float(-grip->d*sinT*sinF + grip->y, 0, MAP_HEIGHT << WORLD_BLOCK_BITS);
	player->z = -grip->d*cosT + grip->z; 		//clamp_float(-grip->d*cosT + grip->z, MINZMAP, MAXZMAP);

	//DrawLine(MAIN, 128/2, 191/2, (grip->z-player->z), sqrt(sqr(player->x-grip->x)+sqr(player->y-grip->y))*cosF, ARGB16(1,31,31,31));

	player->vx = (grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	player->vy = (grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	player->vz = (- grip->vd * sinF + grip->d*grip->vtheta * cosF);

	if(grip->d_rest>75)
			{
				grip->d_rest = grip->d_rest*a;
			}

	grip->vd = grip->vd - k * dl * dt;
	grip->d  = grip->d + grip->vd*dt;
	dl =  grip->d - grip->d_rest;
}
