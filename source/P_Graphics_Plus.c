/*
 * P_Graphics_Plus.c
 *
 *  Created on: Oct 20, 2023
 *      Author: nds
 */
#include "P_Graphics_Plus.h"

/*
# define M_3PI_2   3*M_PI_2
void DrawAngledLine(enum BUFFER_TYPE bT, int x, int y, double angle,
		double distance, u16 color) {
	if (bT == SUB)
		return;

	//These are the current coordinates in the screen as doubles.
	double rx = x + 0.5;
	double ry = y + 0.5;

	//The amount each value will change by moving one unit forwards.
	double dx = cos(angle);
	double dy = sin(angle);
	double dtan = tan(angle);
	bool ray_facing_down = angle > M_PI;
	bool ray_facing_right = angle < M_PI_2 || angle > M_3PI_2;
	bool doing_x = abs(dx) < abs(dy);
	while(distance > 0){
		double lrx = rx, lry = ry;
		if(doing_x){
			rx = (int)(rx + ray_facing_right*2-1);
			ry -= dtan*(rx-lrx);
			DrawVerticalLine(bT, (int)lry, (int)ry, (int)lrx, color);
			distance -= abs(ry-lry);

		}
		else{
			ry = (int)(ry + ray_facing_down*2-1);
			rx -= (ry-lry)/dtan;
			DrawHorizontalLine(bT, (int)lrx, (int)rx, (int)lry, color);
			distance -= abs(rx-lrx);
		}

		doing_x = !doing_x;
		//printf("%d #%.2f, %.2f - d: %.2f\n",doing_x, rx-lrx,ry-lry,distance);
	}
}
*/
void DrawPixel(u16* buffer, int pixel, u16 color){
	if(pixel % 2 == 0){
		buffer[pixel/2] &= 0xff00;
		buffer[pixel/2] |= color;
	}
	else{
		buffer[pixel/2] &= 0xff;
		buffer[pixel/2] |= color<<8;
	}
}

void DrawAngledLine(enum BUFFER_TYPE bT, int x, int y, float angle,
		float distance, u16 color) {
	DrawLine(bT,x,y,x+cos(angle)*distance,y+sin(angle)*distance,color);
}

/*
 *
 */
void DrawLine(enum BUFFER_TYPE bT, int x1, int y1, int x2, int y2,
		u16 color) 
{

	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferH = get_buffer_height(bT);
	int P_BufferW = get_buffer_width(bT);

		float slope;
		float A;
		float B;
		float P;
		float t;
		int level;

		if(abs(y1-y2)>abs(x1-x2))
		{
			slope = (float)(y1-y2) / (float)(x1-x2);

			//clip into frame
			if(x1<0 || x1>P_BufferW)
			{
			int x = clamp(x1, 0, P_BufferW);
			y1 = y1 + slope*(x-x1);
			x1 = x;
			}
			if(y1<0 || y1>P_BufferH)
			{
			int x = clamp(y1, 0, P_BufferH);
			x1 = x1 + (x-y1)/slope;
			y1 = x;
			}

			if(x2<0 || x2>P_BufferW)
			{
			int x = clamp(x2, 0, P_BufferW);
			y2 = y2 + slope*(x-x2);
			x2 = x;
			}
			if(y2<0 || y2>P_BufferH)
			{
			int x = clamp(y2, 0, P_BufferH);
			x2 = x2 + (x-y2)/slope;
			y2 = x;
			}

			A = 2*abs(x1-x2);
			B = A - 2*abs(y1-y2);
			P = A - abs(y1-y2);

			level = (y1<y2) ? x1 : x2 ;

			t = (x1<x2) ? ((level==x1) ? 1 : -1) : ((level==x1) ? -1 : 1) ;

				for(int j = min(y1,y2); j<=max(y1,y2); j++)
				{
					if(P<0)
					{
						P = P + A;
					}
					else
					{
						level = level + t;
						P = P + B;
					}
					P_Buffer[coords(level,j,P_BufferW)] = color;
				}
		}
		else
		{
			slope = (float)(x1-x2) / (float)(y1-y2);

			//clip into frame
			if(x1<0 || x1>P_BufferW)
			{
			int x = clamp(x1, 0, P_BufferW);
			y1 = y1 + slope*(x-x1);
			x1 = x;
			}
			if(y1<0 || y1>P_BufferH)
			{
			int x = clamp(y1, 0, P_BufferH);
			x1 = x1 + (x-y1)/slope;
			y1 = x;
			}

			if(x2<0 || x2>P_BufferW)
			{
			int x = clamp(x2, 0, P_BufferW);
			y2 = y2 + slope*(x-x2);
			x2 = x;
			}
			if(y2<0 || y2>P_BufferH)
			{
			int x = clamp(y2, 0, P_BufferH);
			x2 = x2 + (x-y2)/slope;
			y2 = x;
			}
			A = 2*abs(y1-y2);
			B = A - 2*abs(x1-x2);
			P = A - abs(x1-x2);

					level = (x1<x2) ? y1 : y2 ;

					t = (y1<y2) ? ((level==y1) ? 1 : -1) : ((level==y1) ? -1 : 1) ;

						for(int j = min(x1,x2); j<=max(x1,x2); j++)
						{
							if(P<0)
							{
								P = P + A;
							}
							else
							{
								level = level + t;
								P = P + B;
							}
							P_Buffer[coords(j,level,P_BufferW)] = color;
						}
		}
}


void DrawCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color){
	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferW = get_buffer_width(bT);
	float theta;
	for(theta = 0; theta < 2* M_PI; theta += M_PI_2 / (1.1*radius)){
		DrawPixel(P_Buffer,coords(x+cos(theta)*radius,y+sin(theta)*radius,P_BufferW),color);
	}
}

void FillCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color){
	/*int r = 1;
	for(r = 1; r < radius; r++){
		DrawCircle(bT, x, y, r, false, color);
	}*
	float theta;
	for(theta = 0; theta < 2* M_PI; theta += M_PI_2 / (1.1*radius)){
		DrawAngledLine(bT,x,y,theta,radius,color);
	}*/
	int* P_Buffer = get_buffer_pointer(bT);
		int P_BufferW = get_buffer_width(bT);
	int i,j;
	for(i = x - radius; i < x + radius; i++){
		for(j = y-radius; j < y + radius; j++){
			if((i-x)*(i-x) + (j-y)*(j-y) <= (radius-1) * radius){
				DrawPixel(P_Buffer,coords(i,j,P_BufferW),color);
			}
		}
	}

}

