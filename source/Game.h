#pragma once
#include <math.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Map.h"
#include "Controls.h"
#include "P_Audio.h"
#include "Render.h"
#include "gameplay.h"
#include "Score.h"

void initScore();

void redraw_screen();

void slowdown_ISR();
void countdown_ISR();

void slowdown();

void try_sling(touchPosition t,Camera* camera, Player* player, Grip* grip);

void do_sling(Player* player, Grip* grip, Pos pos);

void DrawWeb(enum BUFFER_TYPE bT, Camera* camera, Player* player, Grip* grip);

void gameLogic(Camera* camera, Player* player, Grip* grip);
