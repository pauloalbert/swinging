#pragma once
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"

void redraw_screen();

//called by Controls, does game logic when attempting to sling
void try_sling(touchPosition t,Camera* camera, Grip* grip);

void GameLogic(Camera* camera,Player* player,  Grip* grip);
