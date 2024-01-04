#include "gameplay.h"

float g = 9.81;				// gravity acceleration

float k = 0.1;				// spring constant
float spring_max = 2.0;		// force clamping
float vphi_max = 5;		// v phi max clamping
float vtheta_max = 5;		// v theta max clamping
float dl = 0;				// web length delta
float UpOffset = 30;			// initial Offset when changing web (bounce up)

float dt = DEFAULT_DT;				// time interval per loop

float damping = 0.0005;
float TwoPi = 2*3.141592;

float cosT = 0;
float sinT = 0;
float cosF = 0;
float sinF = 0;

void Transit(Player* player, Grip* grip)
{
	//calculate the initial length of the rope to maintain
	grip->d = mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));

	//Bounce constants

	grip->d_rest = grip->d - UpOffset;

	dl = grip->d - grip->d_rest;

	//calculate initial positions
	cosT = (grip->z - player->z)/grip->d;

	grip->theta = acos( cosT );
	sinT = sin(grip->theta);

	cosF = (grip->x-player->x) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	sinF = (grip->y-player->y) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	grip->phi = ( (cosF > 0) ? ( (sinF>0) ? acos(cosF) : -acos(cosF)) : ( (sinF>0) ? acos(cosF) : -acos(cosF)));


	grip->vd = player->vx*sinT*cosF + player->vy*sinT*sinF - player->vz*cosT; // - UpOffset;				//correct
	grip->vtheta = (player->vx*cosT*cosF + player->vy*cosT*sinF + player->vz*sinT)/grip->d;	//correct
	grip->vphi = (-player->vx*sinF + player->vy*cosF)/grip->d; 								//correct


}

float FallBounce(Grip* grip, Player* player)
{
	cosT = (grip->z - player->z)/mag(grip->x - player->x,grip->y - player->y,grip->z - player->z);
	if(cosT<0)
		return -100;
	else
		return (-grip->d*cosT + grip->z);
}

void Fall(Player* player, Grip* grip)
{

	if(player->z <= FallBounce(grip, player) && grip->ON)
	{
		player->state = Swinging;
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
	grip->vd = grip->vd*(1-damping) - clamp_float(k * dl * dt,-spring_max,spring_max);

	grip->d  = fabs(grip->d + grip->vd*dt);
	dl =  grip->d - grip->d_rest;

	if(fabs(cosT/sinT) < 100)
	grip->vphi = grip->vphi*(1-damping) - 2*grip->vtheta*grip->vphi*cosT/sinT*dt;
	else
	grip->vphi =grip->vphi*(1-damping);

	grip->vphi = clamp_float(grip->vphi,-vphi_max,vphi_max);

	grip->phi = grip->phi + grip->vphi*dt;

	cosF = cos(grip->phi);
	sinF = sin(grip->phi);

	grip->vtheta = grip->vtheta*(1-damping) - g*sinT*dt/grip->d + sqr(grip->vphi)*cosT*sinT*dt;
	grip->vtheta = clamp_float(grip->vtheta,-vtheta_max,vtheta_max);
	grip->theta = grip->theta + grip->vtheta*dt;


	cosT = cos(grip->theta);
	sinT = sin(grip->theta);


	player->vx = -(grip->vd * sinT*cosF + grip->d*grip->vtheta * cosT*cosF - grip->d*grip->vphi*sinT * sinF);
	player->vy = -(grip->vd * sinT*sinF + grip->d*grip->vtheta * cosT*sinF + grip->d*grip->vphi*sinT * cosF);
	player->vz = (- grip->vd * cosT + grip->d*grip->vtheta * sinT);

	player->x = player->x + player->vx*dt;
	player->y = player->y + player->vy*dt;
	player->z = player->z + player->vz*dt;

	if(grip->theta>0 || grip->theta<=0)
		printf("%.2f, %.2f, %.2f, %.2f\n", sqr(grip->vphi),  g*sinT*dt/grip->d, sqr(grip->vphi)*cosT*sinT*dt, grip->vtheta);
	}

void CrashTest(Player* player, Grip* grip)
{

/*	if(player->z <= -100)
	{
		player->state = Paused;
		grip->ON = false;
		mmPause();
		draw_GameOver();
	}

	else

		{
		if(getBuildingFromWorld(player->x,player->y).u16 != 0)
			{
			if(getBuildingFromWorld(player->x,player->y).height > player->z)
			{
				player->state = Paused;
				grip->ON = false;
				mmPause();
				draw_GameOver();
				}
			}
		}
*/
}
