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

u8 char_sprite[] = {
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,1,0,0, 0,1,0,1, 0,0,0,0,

		0,0,0,0, 0,0,1,1, 1,1,0,1, 0,0,0,1,
		0,0,0,0, 0,2,1,1, 1,1,1,0, 0,0,0,0,
		0,0,0,0, 0,2,1,1, 1,1,1,0, 0,0,0,0,
		0,0,0,0, 0,0,1,1, 1,1,1,0, 0,0,0,0,

		0,0,0,0, 0,0,0,1, 1,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,1, 1,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
		0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

u16 palette_buffer[256*3];
u8 current_palette = 0;
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
	//wall 5
	BG_PALETTE[9] = RGB15(31,31,2);
	BG_PALETTE[10] = RGB15(25,25,1);
	//wall 6
	BG_PALETTE[11] = RGB15(0,31,2);
	BG_PALETTE[12] = RGB15(0,25,1);
	//wall 7
	BG_PALETTE[13] = RGB15(31,0,13);
	BG_PALETTE[14] = RGB15(25,0,7);

	int c,p;
	//set other palettes
	for(c = 1; c <= 14; c++){
		u16 base = BG_PALETTE[c];
		//fxp 5
		u16 base_red = ((base)&0b11111)<<5;
		u16 base_green = (base&0b1111100000);
		u16 base_blue = (base&0b111110000000000)>>5;
		for(p=1;p <8; p++){

			u8 tint = COLOR_FALLOFF_GRADIENT_RANGE - p + 1;
			BG_PALETTE[c+16*(p)] = RGB15((tint*(base_red/COLOR_FALLOFF_GRADIENT_RANGE))>>5,(tint*(base_green/COLOR_FALLOFF_GRADIENT_RANGE))>>5,(tint*(base_blue/COLOR_FALLOFF_GRADIENT_RANGE))>>5);
		}
	}
	//roof floor (if needed)
	BG_PALETTE[0xef] = RGB15(23,23,30);
	BG_PALETTE[0xff] = RGB15(15,11,15);


	//copy the palette over to a local memory
	int j;
	for(j = 0; j < 256; j++){
		u16 color = BG_PALETTE[j];
		u8 average_color = (u8)(((color&0x1f) + ((color>>5)&0x1f) + ((color>>10)&0x1f))/3);
		palette_buffer[j] = color;
		palette_buffer[j+256] = RGB15(average_color,average_color,average_color);
		palette_buffer[j+512] = RGB15(average_color,0,0);
	}



	//P_Graphics_assignBuffer(MAIN, (u16*)BG_GFX,256,192);
	if(IS_SCREEN_FLIPPED){
	REG_BG2PA = -256;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = -256;
	REG_BG2X = 256*256;
	REG_BG2Y = 256*192;
	}
	else{
		REG_BG2PA = 256;
		REG_BG2PB = 0;
		REG_BG2PC = 0;
		REG_BG2PD = 256;
		REG_BG2X = 0;
		REG_BG2Y = 0;
	}

	/*Tilemap*/
	BGCTRL[0] = BG_MAP_BASE(1) | BG_32x64 | BG_COLOR_16 | BG_TILE_BASE(0) | BG_PRIORITY_1;
	P_Graphics_assignBuffer(MAIN,BG_GFX+0x2000,256,192);


	dmaCopy(A16, &BG_TILE_RAM(0)[0], 32);

	int i;
	for(i=0;i<32*64;i++){
		BG_MAP_RAM(1)[i] = BIT(15) | BIT(14) | BIT(13) | (i>=32*32 ? BIT(12) : 0);
	}
#endif
}

bool swap_palettes(u8 target_palette){
	if(target_palette == current_palette || target_palette > 2)
		return false;

	current_palette = target_palette;
	dmaCopy(&palette_buffer[256*target_palette],BG_PALETTE,512);

	return true;
}

void P_Graphics_setup_sub(){
	// Sub screen
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	//
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE;
	//
	BGCTRL_SUB[2] = BG_BMP_BASE(1) | BG_BMP8_256x256;

	REG_BG2PA_SUB = -200;
	REG_BG2PC_SUB = 0;
	REG_BG2PB_SUB = 0;
	REG_BG2PD_SUB = -150;

	REG_BG2X_SUB = 200*256;
	REG_BG2Y_SUB = 150*192;

	P_Graphics_assignBuffer(SUB,BG_GFX_SUB+0x0000,256,192);

	int i;
	for(i = 0; i < 15; i++){
		BG_PALETTE_SUB[i] = BG_PALETTE[i];
	}
}

u16* P_Graphics_setup_sprites(u16* char_sprite_ptr){
	//assign the vram
	VRAM_F_CR = VRAM_ENABLE | VRAM_F_MAIN_SPRITE;

	oamInit(&oamMain, SpriteMapping_1D_32, false);

	char_sprite_ptr = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);

	swiCopy(swingPal, SPRITE_PALETTE, swingPalLen/2);
	swiCopy(swingTiles, char_sprite_ptr, swingTilesLen/2);

	return char_sprite_ptr;
}

MAC_EXTERN inline int* get_buffer_pointer(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainBuffer : P_Graphics_subBuffer;}
MAC_EXTERN inline int get_buffer_width(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainW : P_Graphics_subW;}
MAC_EXTERN inline int get_buffer_height(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainH : P_Graphics_subH;}

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

void FillColors(enum BUFFER_TYPE t){
	int p,o;
	for(p = 0; p < 16 ; p++){
		for(o=0; o < 16; o++){
			FillRectangle(t,12*o,12*o+12,16*p,16*p+16,p+16*o);
		}
	}
}

void FillBuilding(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	u16 delta = (bottom - top)/5;
	FillRectangle(bT, top, top+delta,left,right,color);
	FillRectangle(bT, top+delta+1, top+2*delta,left,right,color+32);
	FillRectangle(bT, top+2*delta+1, top+3*delta,left,right,color);
	FillRectangle(bT, top+3*delta+1, top+4*delta,left,right,color+32);
	FillRectangle(bT, top+4*delta, bottom,left,right,color);

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
			P_Buffer[coords(i,j,P_BufferW)/2] = color+(color<<8);
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

#define OFFSET_POINTER(pointer,offset) ((u16*)((u32)(pointer) + (u32)(offset)))
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
	//The sub engine uses two
#define SUB_SECOND_FRAME 5
	switch(bT){
	case MAIN:
		//set the new buffer to be OFFSET 4 or OFFSET 1
		if(main_graphics_frame) P_Graphics_assignBuffer(MAIN,OFFSET_POINTER(BG_GFX, 4 *0x4000),256,192);
		else P_Graphics_assignBuffer(MAIN,OFFSET_POINTER(BG_GFX, 1 *0x4000),256,192);

		//clear the newly set buffer (OFFSET 4 or OFFSET 1)
		if(main_graphics_frame) memset(OFFSET_POINTER(BG_GFX, 4 *0x4000),0,256*192);
		else memset(OFFSET_POINTER(BG_GFX, 1 *0x4000),0,256*192);

		//display the old buffer (OFFSET 1 or OFFSET 4)
		if(main_graphics_frame) BGCTRL[2] = BG_BMP_BASE(1) | BG_BMP8_256x256;
		else BGCTRL[2] = BG_BMP_BASE(4) | BG_BMP8_256x256;
		main_graphics_frame = !main_graphics_frame;
		break;
	case SUB:
		if(sub_graphics_frame) P_Graphics_assignBuffer(SUB,OFFSET_POINTER(BG_GFX_SUB,SUB_SECOND_FRAME*0x4000),256,192);
		else P_Graphics_assignBuffer(SUB,BG_GFX_SUB,256,192);

		if(sub_graphics_frame) memset(OFFSET_POINTER(BG_GFX_SUB,SUB_SECOND_FRAME*0x4000),0,256*100);
		else memset(BG_GFX_SUB,0,256*100);

		if(sub_graphics_frame) BGCTRL_SUB[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
		else BGCTRL_SUB[2] = BG_BMP_BASE(SUB_SECOND_FRAME) | BG_BMP8_256x256;
		sub_graphics_frame = !sub_graphics_frame;
	}
#endif
}
