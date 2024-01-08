#include "gameplay.h"

#define g 9.81				// gravity acceleration

#define vphi_max 2		// v phi max clamping
#define vtheta_max 10		// v theta max clamping

#define UpOffset 80			// initial Offset when changing web (bounce up)

float dt = DEFAULT_DT;				// time interval per loop

#define dampingT 0.0005
#define dampingF 0.0005

float cosT = 0;
float sinT = 0;
float cosF = 0;
float sinF = 0;

extern int score;

void Transit(Player* player, Grip* grip)
{
	//calculate the initial length of the rope
	grip->d = mag((grip->x-player->x),(grip->y-player->y),(grip->z-player->z));

	if(grip->d>=7/4*UpOffset)
		grip->d_rest = grip->d - UpOffset;
	else
		grip->d_rest = grip->d;

	//calculate initial angular position
	cosT = (grip->z - player->z)/grip->d;
	grip->theta = acos( cosT );
	sinT = sin(grip->theta);

	cosF = (grip->x-player->x) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	sinF = (grip->y-player->y) / sqrt( sqr(grip->x-player->x) + sqr(grip->y-player->y) );
	grip->phi = ( (cosF > 0) ? ( (sinF>0) ? acos(cosF) : -acos(cosF)) : ( (sinF>0) ? acos(cosF) : -acos(cosF)));

	// cartesian velocity to spherical
	grip->vd = 0;
	grip->vtheta = (player->vx*cosT*cosF + player->vy*cosT*sinF + player->vz*sinT)/grip->d;
	grip->vphi = (-player->vx*sinF + player->vy*cosF)/grip->d;


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
		// if attached and within swinging range
		player->state = Swinging;
	}
	else
	{
		// fall
		player->vz = player->vz - g*dt;

		player->x = player->x + player->vx*dt;
		player->y = player->y + player->vy*dt;
		player->z = player->z + player->vz*dt;
	}

}


void Swing(Player* player, Grip* grip)
{
	// "pull" web when attaching
	if(grip->d>grip->d_rest && grip->d<=grip->d_rest+UpOffset)
	{
		grip->vd = -UpOffset;	//grip->vd*(1-damping) - clamp_float(k * dl * dt,-spring_max,spring_max);
		grip->d  = fabs(grip->d + grip->vd*dt);
	}
	else
	{
		grip->vd = 0;
		grip->d = fabs(grip->d + grip->vd*dt);
	}


	// avoid divergence
	if(fabs(cosT/sinT) < 70)
	grip->vphi = grip->vphi*(1-dampingF) - 2*grip->vtheta*grip->vphi*cosT/sinT*dt;
	else
	grip->vphi =grip->vphi*(1-dampingF);

	grip->vphi = clamp_float(grip->vphi,-vphi_max,vphi_max);

	grip->phi = fmod(grip->phi + grip->vphi*dt,2*Pi);

	cosF = cos(grip->phi);
	sinF = sin(grip->phi);

	grip->vtheta = grip->vtheta*(1-dampingT) - g*sinT*dt/grip->d + sqr(grip->vphi)*cosT*sinT*dt;
	grip->vtheta = clamp_float(grip->vtheta,-vtheta_max,vtheta_max);
	grip->theta = fmod(grip->theta + grip->vtheta*dt,Pi);


	cosT = cos(grip->theta);
	sinT = sin(grip->theta);


	// spherical velocity to cartesian (excluding radial velocity)
	player->vx = -(grip->d_rest*grip->vtheta * cosT*cosF - grip->d*grip->vphi* sinF);
	player->vy = -(grip->d_rest*grip->vtheta * cosT*sinF + grip->d*grip->vphi* cosF);
	player->vz = grip->d_rest*grip->vtheta * sinT;

	// new position constrained on web length
	player->x = grip->x - grip->d*sinT*cosF;
	player->y = grip->y - grip->d*sinT*sinF;
	player->z = grip->z - grip->d*cosT;
	}

void CrashTest(Player* player, Grip* grip)
{

	if(player->z <= 20)
	{
		player->live = false;
		irqDisable(IRQ_TIMER0);
		score = 0;
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
				player->live = false;
				irqDisable(IRQ_TIMER0);
				score = 0;
				player->state = Paused;
				grip->ON = false;
				mmPause();
				draw_GameOver();
			}
		}
	}

}
