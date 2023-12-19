#pragma once

#include <stdio.h>
#include <nds.h>
#include "Map.h"
#include "P_Util.h"
#include "Controls.h"
#include "Game.h"

#define	WHITE ARGB16(1,31,31,31)
#define	RED ARGB16(1,31,0,0)
#define	BLUE ARGB16(1,0,0,31)

#define MAXWEB 99999999999999
#define MINZMAP 0
#define MAXZMAP 10000

void Transit(Player* player, Grip* grip);

void Swing(Player* player, Grip* grip);

float FallBounce(Grip* grip);

void Fall(Player* player, Grip* grip);

