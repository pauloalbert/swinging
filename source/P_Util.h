/*
 * util.h
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */
#pragma once
#include <nds.h>
#include <math.h>

#ifndef PAULO
#define MAC_EXTERN extern
#endif
#ifdef PAULO
#define MAC_EXTERN
#endif

MAC_EXTERN inline float clamp_float(float value, float min, float max);

MAC_EXTERN inline int clamp(int value, int min, int max);

MAC_EXTERN inline int coords(int x, int y, int w);

MAC_EXTERN inline int sign(int x);

MAC_EXTERN inline int round_float(float b);

int rng();

float min(float a, float b);

float max(float a, float b);

float mag(float a, float b, float c);

void set_seed(int new_seed);

MAC_EXTERN inline int mod(int x, int amount);
MAC_EXTERN inline float mod_float(float x, int amount);
//not sure this works for negative numbers
#define floor_bits(i, bits) (((i)>>(bits))<<(bits))
//Fixed percision multiplication for integer floats. see lecture 10

MAC_EXTERN inline int convert_ranges(int number, int origin_min, int origin_max, int target_min, int target_max);
/* FXP */
#define FXP_DECIMAL_BITS 8		//range: -128..128, 1/256 decimals
#define FXP_UNIT (1<<FXP_DECIMAL_BITS)
#define fixed_mult_16on32(i,j,shift) ((int16_t)(((((int32_t)(i)*(int32_t)(j)) >> (shift))))
#define float_to_fxp(f,bits) (((int)((f)*(1<<(bits))))>>(bits))
#define fxp_to_float(fxp,bits) ((fxp)/((float)(1<<(bits))))

#define sqr(x) ((x)*(x))
