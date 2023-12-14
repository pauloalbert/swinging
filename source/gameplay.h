#pragma once

#include <stdio.h>
#include <nds.h>
#include "Map.h"
#include "P_Util.h"
#include "Controls.h"

#define	WHITE ARGB16(1,31,31,31)
#define	RED ARGB16(1,31,0,0)
#define	BLUE ARGB16(1,0,0,31)

#define MAXWEB 99999999999

enum STATE {
	GameOff,
	Transition,
	Swinging,
	Falling
};

enum STATE Transit(Player *player, Grip *grip, enum STATE state);

void Swing(Player *player, Grip *grip);

float FallBounce(Player *player, Grip *grip);

enum STATE Fall(Player *player, Grip *grip, enum STATE state);

enum STATE game(Camera *camera, Player *player, Grip *grip, enum STATE state);
