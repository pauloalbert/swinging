#include "Score.h"

int score = 0, Sec, Min, Hour, max_score = 0;

void score_ISR()
{
score++;
if(score>=1000)
{
	score = 0;
	Sec = Sec + 1;
	if(Sec>=60)
	{
		Sec = 0;
		Min = Min + 1;
		if(Min>=60)
		{
			Min = 0;
			Hour = Hour + 1;
			if(Hour >=100)
			{
				extern Player* player;
				extern Grip* grip;
				player->state = Paused;
				grip->ON = false;
				mmPause();
				draw_GameOver();
			}
		}
	}
}
}

void displayScore()
{
	int j,i, number;
	//i, digit to display
	for(i = 5; i>0; i--)
	{
		//calculate the power of i-1 in j
		j = i-1; number = 1;
		while(j--)number = number*10;
		//Obtain the number to render
		number = score / number;
		number = number % 10;
		//Render the number
		//BG_MAP_RAM_SUB(24)[1+(23-i)*32] = numbersMap[(10 - number)*2] | TILE_PALETTE(9);
		//BG_MAP_RAM_SUB(24)[1+(23-i)*32+1] = numbersMap[(10 - number)*2+1] | TILE_PALETTE(9);
	}
}


/*
 * This function reads the highest score obtained in previous games and
 * stored in a text file called /Tetrisrecord.txt. The read value is assigned
 * to the variable max_score
 */
void readMaxScore()
{
	//Open the file in read mode
	FILE* file = fopen("/Tetrisrecord.txt","r");
	if(file != NULL)
	{
		//Read the value and put it into the variable max_score
		fscanf(file,"%i\n",&max_score);
		printf("maxscoreRead\n");
		// close the file
		fclose(file);
	}
	else
			printf("NOTREAD\n");
}

/*
 * This function writes the value of the variable max_score
 * into the text file /tetrisrecord
 */
void writeMaxScore()
{
	//Open the file in write mode to overwrite
	FILE* file = fopen("/Tetrisrecord.txt","w+");
	if(file != NULL)
	{
		//Print the value in the file
		fprintf(file,"%i\n",max_score);
		//Close the file
		fclose(file);
	}
}

/*
 * This function displays the highest score obtained in previous games
 * on the left part of bottom the screen (looking sideways). It uses the
 * palette 8 (with red color preloaded)
 */
void displayMaxScore()
{
	/*int i, j, number;
	for(i = 5; i>0; i--)
	{
		j = i-1; number = 1;
		while(j--)number = number*10;
		number = max_score / number;
		number = number % 10;
		BG_MAP_RAM_SUB(24)[1+(6-i)*32] = numbersMap[(10 - number)*2] | TILE_PALETTE(8);
		BG_MAP_RAM_SUB(24)[1+(6-i)*32+1] = numbersMap[(10 - number)*2+1] | TILE_PALETTE(8);
	}*/
	printf("DISPLAY\n");
}
