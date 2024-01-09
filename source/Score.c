#include "Score.h"

unsigned int score = 0; Msec = 0, Sec = 0, Min = 0, Hour = 0, max_score;

void score_ISR()
{
	if(score == 0){
		Msec = 0;
		Sec = 0;
		Min = 0;
		Hour = 0;
		score = 1;
	}
	Msec++;
	if(Msec>=1000)
	{
		Msec = 0;
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
					player->live = false;
					player->state = Paused;
					irqDisable(IRQ_TIMER0);
					score = 0;
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
	printf("\n\nSCORE :\n\n%d:%d:%d:%d", Hour, Min, Sec, Msec);
}


/*
 * This function reads the highest score obtained in previous games and
 * stored in a text file called /Record.txt. The read value is assigned
 * to the variable max_score
 */
void readMaxScore()
{
	//Open the file in read mode
	FILE* file = fopen("/Record.txt","r");
	if(file != NULL)
	{
		//Read the value and put it into the variable max_score
		fscanf(file,"%d\n",&max_score);
		// close the file
		fclose(file);
	}
}

/*
 * This function writes the value of the variable max_score
 * into the text file /Record
 */
void writeMaxScore()
{
	//Open the file in write mode to overwrite
	FILE* file = fopen("/Record.txt","w+");
	if(file != NULL)
	{
		//Print the value in the file
		fprintf(file,"%d\n",max_score);
		//Close the file
		fclose(file);
	}
}

/*
 * This function displays the highest score obtained in previous games.
 */
void displayMaxScore()
{

	if(( ( ( Hour*60+Min )*60+Sec )*1000+Msec ) > max_score){
		max_score = ( ( ( Hour*60+Min )*60+Sec )*1000+Msec );
		printf("\n\nNEW RECORD!\n\n%d:%d:%d:%d", Hour, Min, Sec, Msec);
	}
	else {
		int maxH, maxM, maxS, maxMs;

		// total hours
		maxH = (max_score - mod(max_score,(1000*60*60)))/(1000*60*60);

		// total extra mins
		maxM = (mod(max_score,(1000*60*60)) - mod(max_score,(1000*60))) / (1000*60);

		// total extra secs
		maxS = (mod(max_score,(1000*60)) - mod(max_score,1000)) / 1000;

		// total extra msecs
		maxMs = mod(max_score,1000);

		printf("\n\nRECORD :\n\n%d:%d:%d:%d", maxH, maxM, maxS, maxMs);
	}
}
