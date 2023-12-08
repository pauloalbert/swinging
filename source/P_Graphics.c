#include "P_Graphics.h"

u16* P_Graphics_mainBuffer;
u16* P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;
u8 A16[32] = {
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff
};
bool main_graphics_frame = true;
bool sub_graphics_frame = true;
void P_Graphics_setup_main()
{
#ifdef FB0
	//Graphics (Part 1)
	//Set the mode to FB0
	REG_DISPCNT = MODE_FB0;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_LCD;


	P_Graphics_assignBuffer(MAIN, (u16*)VRAM_A,256,192);

#endif

#ifdef ROTOSCALE
	//BG0 will be a tilemap for the sky and floor, using VRAM B
	//BG2 will be a a rotoscale for the walls, using VRAM A
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	//VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG;

	BGCTRL[2] = BG_BMP_BASE(1) | BG_BMP8_256x256;

	//wall 1
	BG_PALETTE[1] = RGB15(15,15,31);
	BG_PALETTE[2] = RGB15(8,8,21);
	//wall 2
	BG_PALETTE[3] = RGB15(31,15,15);
	BG_PALETTE[4] = RGB15(21,8,8);
	//wall 3
	BG_PALETTE[5] = RGB15(5,21,15);
	BG_PALETTE[6] = RGB15(2,16,10);

	//wall 4
	BG_PALETTE[7] = RGB15(0,15,15);
	BG_PALETTE[8] = RGB15(0,8,8);

	//GOAL
	BG_PALETTE[9] = RGB15(31,31,2);
	BG_PALETTE[10] = RGB15(25,25,1);
	//roof floor (if needed)
	BG_PALETTE[15] = RGB15(23,23,30);
	BG_PALETTE[31] = RGB15(15,11,15);
	//P_Graphics_assignBuffer(MAIN, (u16*)BG_GFX,256,192);
	REG_BG2PA = 256;
	REG_BG2PC = 0;
	REG_BG2PB = 0;
	REG_BG2PD = 256;

	/*Tilemap*/
	BGCTRL[0] = BG_MAP_BASE(1) | BG_32x32 | BG_COLOR_16 | BG_TILE_BASE(0) | BG_PRIORITY_1;
	P_Graphics_assignBuffer(MAIN,BG_GFX,256,192);


	dmaCopy(A16, &BG_TILE_RAM(0)[0], 32);

	int i;
	for(i=0;i<32*32;i++){
		BG_MAP_RAM(1)[i] = 0 | (i>=32*12 ? BIT(12) : 0);
	}
#endif
}

inline u16* get_buffer_pointer(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainBuffer : P_Graphics_subBuffer;}
inline int get_buffer_width(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainW : P_Graphics_subW;}
inline int get_buffer_height(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainH : P_Graphics_subH;}

void P_Graphics_assignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h)
{

    switch(bT)
    {
        case MAIN: P_Graphics_mainBuffer = buffer;
            P_Graphics_mainW = w;
            P_Graphics_mainH = h;
            break;
        case SUB: P_Graphics_subBuffer = buffer;
            P_Graphics_subW = w;
            P_Graphics_subH = h;
            break;
    }
}


void FillScreen(enum BUFFER_TYPE t, u16 color)
{
	int i;
#ifdef FB0
	u16* P_Buffer = get_buffer_pointer(t);
#endif
#ifdef ROTOSCALE
	u16* P_Buffer = get_buffer_pointer(t);
#endif

	int P_BufferH = get_buffer_height(t);
	int P_BufferW = get_buffer_width(t);
		//Fill the whole screen (256*192) with the given color
	for(i = 0; i<P_BufferH*P_BufferW/2; i++){
		//color=color&0b11111 |(color&(0b11111<<5)) | color&(0b11111<<10);
		P_Buffer[i] = color+(color<<8);
	}

}

void FillRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
#ifdef FB0
	u16* P_Buffer = get_buffer_pointer(bT);
#endif
#ifdef ROTOSCALE
	u16* P_Buffer = get_buffer_pointer(bT);
#endif

	int P_BufferH = get_buffer_height(bT);
	int P_BufferW = get_buffer_width(bT);

	top = clamp(top, 0, P_BufferH);
	bottom = clamp(bottom, -1, P_BufferH-1);
	left = clamp(left, 0, P_BufferW);
	right = clamp(right, -1, P_BufferW-1);

	if(right < left || bottom < top)
		return;

	short i, j;

	for(j = top; j <= bottom; j++){
		i = left;
		//if i'm on an odd pixel, draw only the second one of the pair.
		if(i%2 == 1)
			P_Buffer[coords(i++,j,P_BufferW)/2] |= (color<<8);

		for(; i < right; i+=2){
			P_Buffer[coords(i,j,P_BufferW)/2] |= color+(color<<8);
		}
		if(i == right)
			P_Buffer[coords(i,j,P_BufferW)/2] = color;
	}
}

void DrawHorizontalLine(enum BUFFER_TYPE bT, int x, int x2, int y, u16 color){
	if(x2 >= x)
		FillRectangle(bT,y,y,x,x2,color);
	else
		FillRectangle(bT,y,y,x2,x,color);
}
void DrawVerticalLine(enum BUFFER_TYPE bT, int y, int y2, int x, u16 color)
{
	if(y2 >= y)
		FillRectangle(bT,y,y2,x,x,color);
	else
		FillRectangle(bT,y2,y,x,x,color);
}

void DrawRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	DrawVerticalLine(bT,top,bottom,left,color);
	DrawVerticalLine(bT,top,bottom,right,color);
	DrawHorizontalLine(bT,left,right,top,color);
	DrawHorizontalLine(bT,left,right,bottom,color);

}

void swap_buffers(enum BUFFER_TYPE bT){

#ifdef FB0

	switch(bT){
	case MAIN:
		if(main_graphics_frame) P_Graphics_assignBuffer(MAIN,(u16*)VRAM_B,256,192);
		else P_Graphics_assignBuffer(MAIN,(u16*)VRAM_A,256,192);

		//Set the coresponding VRAM bank
		if(main_graphics_frame) {REG_DISPCNT = MODE_FB0;}
		else {REG_DISPCNT = MODE_FB1;}
		main_graphics_frame = !main_graphics_frame;
		break;
	case SUB:
		break;
	}
#endif


#ifdef ROTOSCALE
	switch(bT){
	case MAIN:
		if(main_graphics_frame) P_Graphics_assignBuffer(MAIN,BG_GFX+0x8000,256,192);
		else P_Graphics_assignBuffer(MAIN,BG_GFX+0x2000,256,192);

		if(main_graphics_frame) memset(BG_GFX + 0x8000,0,256*192);
		else memset(BG_GFX+0x2000,0,256*192);

		if(main_graphics_frame) BGCTRL[2] = BG_BMP_BASE(1) | BG_BMP8_256x256;
		else BGCTRL[2] = BG_BMP_BASE(4) | BG_BMP8_256x256;
		main_graphics_frame = !main_graphics_frame;
		break;
	case SUB:
		break;
	}
#endif
}
