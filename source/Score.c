#include "Score.h"

int score = 0; Msec = 0, Sec = 0, Min = 0, Hour = 0;
float max_score = 0;

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
	printf("DISPLAY SCORE  %d\n", score);
	int number;

	// tens of hours
	number = (Hour - mod(Hour, 10))/10;
	displayNumber(number, true, 1);
	// units of hours
	number = mod(Hour, 10);
	displayNumber(number, true, 2);

	// tens of mins
	number = (Min - mod(Min, 10))/10;
	displayNumber(number, true, 3);
	// units of mins
	number = mod(Min, 10);
	displayNumber(number, true, 4);

	// tens of secs
	number = (Sec - mod(Sec, 10))/10;
	displayNumber(number, true, 5);
	// units of secs
	number = mod(Sec, 10);
	displayNumber(number, true, 6);

	// hundreds of msecs
	number = (Msec - mod(Msec, 100))/100;
	displayNumber(number, true, 7);
	// tens of msecs
	number = (mod(Msec, 100) - mod(Msec, 10))/10;
	displayNumber(number, true, 8);
	// units of msecs
	number = mod(Msec, 10);
	displayNumber(number, true, 9);
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
		fscanf(file,"%f\n",&max_score);
		printf("maxscoreRead\n");
		// close the file
		fclose(file);
	}
	else
		printf("NOTREAD\n");
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
		fprintf(file,"%f\n",max_score);
		//Close the file
		fclose(file);
	}
}

/*
 * This function displays the highest score obtained in previous games.
 */
void displayMaxScore()
{
	if(( ((Hour*60+Min)*60+Sec)*1000+Msec) > max_score){
			max_score = ((Hour*60+Min)*60+Sec)*1000+Msec;
		}

	int maxH, maxM, maxS, maxMs, number;

	// total hours
	maxH = (max_score - mod(max_score,(1000*60*60)))/(1000*60*60);

	// tens of hours
	number = (maxH - mod(maxH, 10))/10;
	displayNumber(number, false, 1);

	// units of hours
	number = mod(maxH, 10);
	displayNumber(number, false, 2);

	// total extra mins
	maxM = mod(max_score,(1000*60*60)) - mod(max_score,(1000*60)) / (1000*60);

	// tens of mins
	number = (maxM - mod(maxM, 10))/10;
	displayNumber(number, false, 3);

	// units of mins
	number = mod(maxM, 10);
	displayNumber(number, false, 4);

	// total extra secs
	maxS = (mod(max_score,(1000*60)) - mod(max_score,1000)) / 1000;

	// tens of secs
	number = (maxS - mod(maxS, 10))/10;
	displayNumber(number, false, 5);

	// units of secs
	number = mod(maxS, 10);
	displayNumber(number, false, 6);

	// total extra msecs
	maxMs = mod(max_score,1000);

	// hundreds of msecs
	number = (maxMs - mod(maxMs, 100))/100;
	displayNumber(number, false, 7);

	// tens of msecs
	number = (mod(maxMs, 100) - mod(maxMs, 10))/10;
	displayNumber(number, false, 8);

	// units of msecs
	number = mod(maxMs, 10);
	displayNumber(number, false, 9);

	printf("DISPLAY MAX  %f\n", max_score);
}

// display number in score or max score raw (true or false), at digit position (from 1 to 9)
void displayNumber(int number, bool raw, int digit){
	//BG_MAP_RAM(XXX)[XXX] = numbersMap[(number)*2];
}
