/*
 * util.c
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */
#include "P_Util.h"

MAC_EXTERN inline float clamp_float(float value, float min, float max){
	return value < min ? min : (value > max ? max : value);
}

MAC_EXTERN inline int clamp(int value, int min, int max){
	return value < min ? min : (value > max ? max : value);
}
MAC_EXTERN inline int coords(int x, int y, int w){
	return w*y + x;
}

MAC_EXTERN inline int sign(int x){
	if(x > 0)
		return 1;
	else if(x < 0)
		return -1;
	return 0;
}

MAC_EXTERN inline int round_float(float b){
	return (int)(b+0.5);
}

MAC_EXTERN inline int mod(int x, int amount){
	return ((x % amount) + amount) % amount;
}
MAC_EXTERN inline float mod_float(float x, int amount){
	return fmod(fmod(x, amount) + amount,amount);
}

int seed = 123456789;
#define RNG_A 1103515245
#define RNG_C 12345
#define RNG_M (2<<15)
int rng(){
	seed = (RNG_A * seed + RNG_C) % RNG_M;
	return seed < 0 ? -seed : seed;
}

float min(float a,float b)
{return ((a<b)*a+(a>=b)*b);

}
float max(float a,float b)
{return ((a>b)*a+(a<=b)*b);
}

MAC_EXTERN inline int convert_ranges(int number, int origin_min, int origin_max, int target_min, int target_max){
	//this function converts a number from one range to another
	return (number - origin_min) * (target_max - target_min) / (origin_max - origin_min) + target_min;
}

void set_seed(int new_seed){
	seed = new_seed;
}


