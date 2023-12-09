/*
 * Render.h
 *
 *  Created on: Dec 9, 2023
 *      Author: nds
 */

#pragma once
#pragma once
#include <stdio.h>
#include <nds.h>
#include "P_Graphics.h"
#include "P_Graphics_Plus.h"
#include "Constants.h"
#include <stdlib.h>
#include <math.h>
#include "P_Util.h"
#include "Map.h"
void Render_3D(enum BUFFER_TYPE bT, Camera player, int columns);

void Render_2D(enum BUFFER_TYPE bT, Camera camera, int left, int top, int right, int bottom);
