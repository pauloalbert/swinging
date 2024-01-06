/*
 * Controls.h
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#pragma once

#include <nds.h>
#include "Map.h"
#include "P_Util.h"
#include "Game.h"
#include "Render.h"
#include <math.h>

void initInput();
void handleInput(Camera* camera, Player* player, Grip* grip);
