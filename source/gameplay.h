#pragma once

#include <math.h>
#include <stdio.h>
#include <nds.h>
#include "Map.h"
#include "P_Util.h"
#include "Controls.h"
#include "Game.h"

#define	RED ARGB16(1,31,0,0)


void Transit(Player* player, Grip* grip);

void Swing(Player* player, Grip* grip);

float FallBounce(Grip* grip, Player* player);

void Fall(Player* player, Grip* grip);

void CrashTest(Player* player, Grip* grip);

