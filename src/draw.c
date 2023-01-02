#include "config.h"

#include <errno.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "include/body.h"

#ifdef MKDIR_TAKES_ONE_ARG   /* MinGW32 */
#undef mkdir
#define mkdir(a, b) mkdir(a)
#endif

#define START_SIZE 4
//TODO consider having speed be affected by window size
#define SPEED 50000 //global refresh rate (for usleep())
#define TIMEOUT 1

//#define DEBUG 1

void updateScore(int which, int score);
void updateScreen(SNAKE *sp, int dir, bool *collision, int *fruit_loc, bool *fruit, int *score);
int loadScore(void);
void saveScore(int highscore);

#ifdef DEBUG
void count_up(void);
#endif

int main(){
	int score = 0;
	int highscore;
	int input;
	int dir;
	bool collision;
	int fruit_loc[2]; //index 0 is x, 1 is y
	bool fruit_bool;
	bool new_hs_flag;
	int i;

	//TODO consider removing score2 altogether

	srand(time(NULL));

	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	set_escdelay(1);

	while(input != 'q'){
		clear();
		
		//reset variables
		collision = false;
		fruit_bool = false;
		score = 0;
		new_hs_flag = false;
		highscore = loadScore();


		//draw current score, and high score
		updateScore(0, score);
		updateScore(1, highscore);

		//the snake will be initially drawn without calling to body.h
		//functions in body.h will update the snake and check for collision
		//updateScreen() will actually draw the snake on each update (and update the scoreboard)

		//initialize and draw starting snake
		SNAKE *player = makeSnake(START_SIZE+2, getmaxy(stdscr)/2, START_SIZE);
		//TODO consider various choices for drawing the snake (the below sets reverse video and draws spaces)
		attron(A_REVERSE);
		mvaddch(getmaxy(stdscr)/2, START_SIZE+2, ':');
		for(i = 1; START_SIZE-i > 0; i++){
			mvaddch(getmaxy(stdscr)/2, START_SIZE+2-i, ' ');
		}
		attroff(A_REVERSE);

		//TODO add something like "press any key to start!"
		getch();

		//turn timeout on and set the initial direction to "right"
		timeout(TIMEOUT);
		dir = 1;
		while(!collision){
			input = getch();
			
			//update dir based on received input (check that the input is valid as well)
			switch(input){
				case KEY_UP:
					if(dir % 2) dir = 0;
					break;
				case KEY_RIGHT:
					if(!(dir % 2)) dir = 1;
					break;
				case KEY_DOWN:
					if(dir % 2) dir = 2;
					break;
				case KEY_LEFT:
					if(!(dir % 2)) dir = 3;
			}

			updateScore(0, score);
			if(score > highscore){
				highscore = score;
				new_hs_flag = true;
			}
			updateScore(1, highscore);
			updateScreen(player, dir, &collision, fruit_loc, &fruit_bool, &score);
			//TODO consider changing SPEED depending on if moving horizontally or vertically
			refresh();
			usleep(SPEED);
		}

		deleteSnake(player);
		if(new_hs_flag) saveScore(highscore);

		//turn timeout off (wait for input again)
		timeout(-1);

		//TODO add something like "game over"
		while(input != 'q' && input != ' ' && input != 10) input = getch();

		//DEBUG
#ifdef DEBUG
		//count_up();
		getch();
#endif
	}

	endwin();
	return 0;
}

// 0 for p1 score, 1 for highscore, 2 for p2 score
void updateScore(int which, int score){
	char *title;
	int start;

	switch(which){
		case 0: //p1 score
			title = "Score";
			start = 2;
			break;

		case 1: //high score
			title = "Highscore";
			start = getmaxx(stdscr)/2 - 5;
			break;

		default:
			exit(1);
	}

	//draw the title and the score in the right location
	mvprintw(0, start, "%s", title);
	mvprintw(1, start + (which % 2 ? 5 : 3) - (!score ? 0 : (int)log10(score)), "%d", score);

	return;
}

void updateScreen(SNAKE *sp, int dir, bool *collision, int *fruit_loc, bool *fruit, int *score){
	char eyes;

	//switch case to decide "eyes" char
	switch(dir){
		case 0:
			eyes = '"';
			break;
		case 2:
			eyes = '.';
			break;
		default:
			eyes = ':';
	}

	//turn off "back"
	mvaddch(getSnakeHeady(sp), getSnakeHeadx(sp), ' ');
	//fix previous "front" (no more eyes)
	attron(A_REVERSE);
	mvaddch(getSnakeTaily(sp), getSnakeTailx(sp), ' ');
	//update the snake position
	updateSnake(sp, dir, getmaxx(stdscr), getmaxy(stdscr), collision, fruit_loc, fruit, score);
	//turn on front
	mvaddch(getSnakeTaily(sp), getSnakeTailx(sp), eyes);
	attroff(A_REVERSE);

	//TODO add logic to prevent fruit from spawning inside snake
	//draw a new fruit, if necessary (do not draw it over scoreboard)
	if(!(*fruit)){
		*fruit_loc = rand() % getmaxx(stdscr);
		*(fruit_loc+1) = rand() % (getmaxy(stdscr)-2) + 2;
		attron(A_BOLD);
		mvaddch(*(fruit_loc+1), *fruit_loc, '@');
		attroff(A_BOLD);
		*fruit = true;
	}

	return;
}

int loadScore(){
	FILE *savedata;
	char *path = malloc(sizeof(char) * 256);
	int hs = 0;
	char *home = getenv("HOME");

	sprintf(path, "%s/.local/share/simple-snake/save.bin", home);
	savedata = fopen(path, "rb");

	if(savedata != NULL){
		fread(&hs, sizeof(int), 1, savedata);
		fclose(savedata);
	}

	free(path);
	return hs;
}

void saveScore(int highscore){
	FILE *savedata;
	char *path = malloc(sizeof(char) * 256);
	char *home = getenv("HOME");

	sprintf(path, "%s/.local/share/simple-snake/save.bin", home);
	savedata = fopen(path, "wb");

	//if the file does not exist, create it
	if(savedata == NULL){
		if(home == NULL) return;

		sprintf(path, "%s/.local/", home);
		mkdir(path, 0700);

		sprintf(path, "%s/.local/share/", home);
		mkdir(path, 0755);

		sprintf(path, "%s/.local/share/simple-snake/", home);
		mkdir(path, 0755);

		sprintf(path, "%s/.local/share/simple-snake/save.bin", home);
		savedata = fopen(path, "wb");
		if(savedata == NULL) return;
	}

	fwrite(&highscore, sizeof(int), 1, savedata);
	fclose(savedata);

	free(path);
	return;
}
		

//debug functions start here
#ifdef DEBUG
void count_up(){
	int score = 0;
	int score2 = 0;
	int highscore = 0;

	while(1){
		score++;
		score2++;
		highscore++;
		updateScore(0, score);
		updateScore(1, highscore);
		updateScore(2, score2);
		refresh();
		usleep(50000);
	}

	return;
}
#endif
