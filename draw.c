#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/body.h"

#define START_SIZE 4
//TODO consider having speed be affected by window size
#define SPEED 50000 //global refresh rate (for usleep())
#define TIMEOUT 1

//#define DEBUG 1

void updateScore(int which, int score);
void updateScreen(SNAKE *sp, int dir, bool *collision);

#ifdef DEBUG
void count_up(void);
#endif

int main(){
	int score = 0;
	int score2 = 0;
	int highscore;
	int input;
	int dir;
	bool collision = false;
	int i;

	//TODO consider removing score2 altogether

	//TODO implement code to retreive existing high score
	highscore = 0;

	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	set_escdelay(1);

	//draw current score, and high score
	updateScore(0, score);
	updateScore(1, highscore);
	updateScore(2, score2);

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
		updateScore(1, highscore);
		updateScore(2, score2);
		updateScreen(player, dir, &collision);
		//TODO consider changing SPEED depending on if moving horizontally or vertically
		refresh();
		usleep(SPEED);
	}

	//turn timeout off (wait for input again)
	timeout(-1);

	//TODO add something like "game over"
	getch();

	//DEBUG
#ifdef DEBUG
	//count_up();
	getch();
#endif

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

		case 2: //p2 score
			title = "Score";
			start = getmaxx(stdscr) - 6;
			break;

		default:
			exit(1);
	}

	//draw the title and the score in the right location
	mvprintw(0, start, "%s", title);
	mvprintw(1, start + (which % 2 ? 5 : 3) - (!score ? 0 : (int)log10(score)), "%d", score);

	return;
}

void updateScreen(SNAKE *sp, int dir, bool *collision){
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
	updateSnake(sp, dir, getmaxx(stdscr), getmaxy(stdscr), collision);
	//turn on front
	mvaddch(getSnakeTaily(sp), getSnakeTailx(sp), eyes);
	attroff(A_REVERSE);

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
