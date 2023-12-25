#include "gameplay.h"

float g = 9.81;				// gravity acceleration

float k = 1;				// spring constant
float dl = 0;				// web length delta
float UpOffset = 0;			// initial Offset when changing web (bounce up)
float DOffset = 0;			// initial Offset when Fallinging on web (bounce down)

float dt = DEFAULT_DT;				// time interval per loop

float a = 0.95;			// rope shortening factor

float cosT = 0;
float sinT = 0;
float cosF = 0;
float sinF = 0;

void Transit(Player* player, Grip* grip)
{
	//TEMP initial velocity when swinging
	//player->vx = 0; //(grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	//player->vy = 0; //(grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	//player->vz = 0; //(- grip->vd * sinF + grip->d*grip->vtheta * cosF);

	//calculate the initial length of the rope to maintain
	grip->d = mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));

	//Bounce constants
	grip->d_rest = grip->d - UpOffset;
	dl = grip->d - grip->d_rest;

	//calculate initial positions
	cosT = (grip->z - player->z)/grip->d;

	if(cosT>0)
		player->state = Swinging;

	grip->theta = acos( cosT );
	sinT = sin(grip->theta);

	cosF = (grip->x-player->x) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	sinF = (grip->y-player->y) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	grip->phi = ( (cosF > 0) ? ( (sinF>0) ? acos(cosF) : -acos(cosF)) : ( (sinF>0) ? acos(cosF) : -acos(cosF)));


	grip->vd = player->vx*sinT*cosF + player->vy*sinT*sinF - player->vz*cosT;				//correct
	grip->vtheta = (player->vx*cosT*cosF + player->vy*cosT*sinF + player->vz*sinT)/grip->d;	//correct
	grip->vphi = (-player->vx*sinF + player->vy*cosF)/grip->d; 								//correct


}

float FallBounce(Grip* grip, Player* player)
{
	cosT = (grip->z - player->z)/grip->d;
	return (-grip->d*cosT + grip->z);
}

void Fall(Player* player, Grip* grip)
{

	if(player->z <= FallBounce(grip, player) && grip->ON)
	{
			Transit(player, grip);
	}
	else
	{
		player->vz = player->vz - g*dt;

		player->x = player->x + player->vx*dt;
		player->y = player->y + player->vy*dt;
		player->z = player->z + player->vz*dt;
	}

}

void Swing(Player* player, Grip* grip)
{
	grip->vd = grip->vd - k * dl * dt;
	grip->d  = grip->d + grip->vd*dt;
	dl =  grip->d - grip->d_rest;

	grip->vphi = grip->vphi - 2*grip->vtheta*grip->vphi*cosT/sinT*dt;
	grip->phi = grip->phi + grip->vphi*dt;

	cosF = cos(grip->phi);
	sinF = sin(grip->phi);

	grip->vtheta = grip->vtheta - g*sinT*dt/grip->d + sqr(grip->vphi)*cosT*sinT*dt;
	grip->theta = grip->theta + grip->vtheta*dt;

	cosT = cos(grip->theta);
	sinT = sin(grip->theta);

	//player->x = -grip->d*sinT*cosF + grip->x; 	//clamp_float(-grip->d*sinT*cosF + grip->x, 0, MAP_WIDTH << WORLD_BLOCK_BITS);
	//player->y = -grip->d*sinT*sinF + grip->y; 	//clamp_float(-grip->d*sinT*sinF + grip->y, 0, MAP_HEIGHT << WORLD_BLOCK_BITS);
	//player->z = -grip->d*cosT + grip->z; 		//clamp_float(-grip->d*cosT + grip->z, MINZMAP, MAXZMAP);

	player->vx = -(grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	player->vy = -(grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	player->vz = (- grip->vd * cosT + grip->d*grip->vtheta * sinT);

	player->x = player->x + player->vx*dt;
	player->y = player->y + player->vy*dt;
	player->z = player->z + player->vz*dt;
}
