#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/body.h"

#define SPEED 50000 //global refresh rate (for usleep())

//#define DEBUG 1

void updateScore(int which, int score);
void updateScreen(SNAKE *sp, int dir);

#ifdef DEBUG
void count_up(void);
#endif

int main(){
	int score = 0;
	int score2 = 0;
	int highscore;

	//TODO consider removing score2 altogether

	//TODO implement code to retreive existing high score
	highscore = 0;

	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0);

	//draw current score, and high score
	updateScore(0, score);
	updateScore(1, highscore);
	updateScore(2, score2);

	//the snake will be initially drawn without calling to body.h
	//functions in body.h will update the snake and check for collision
	//updateScreen() will actually draw the snake on each update (and update the scoreboard)

	//initialize and draw starting snake
	SNAKE *player = makeSnake(6, getmaxy(stdscr)/2, 4);
	//TODO consider various choices for drawing the snake (the below sets reverse video and draws spaces)
	attron(A_REVERSE);
	mvaddch(getmaxy(stdscr)/2, 6, ':');
	mvaddch(getmaxy(stdscr)/2, 5, ' ');
	mvaddch(getmaxy(stdscr)/2, 4, ' ');
	mvaddch(getmaxy(stdscr)/2, 3, ' ');
	attroff(A_REVERSE);

	//TODO add something like "press any key to start!"
	getch();

	//TODO replace '1' with conditional to check if there was a collision
	while(1){
		//TODO elaborate on the updated score each loop with conditionals and such
		updateScore(0, score);
		updateScore(1, highscore);
		updateScore(2, score2);
		updateScreen(player, 0);
		refresh();
		//TODO consider changing SPEED depending on if moving horizontally or vertically
		usleep(SPEED);
	}

	//DEBUG
#ifdef DEBUG
	//count_up();
	snakeTest();
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

void updateScreen(SNAKE *sp, int dir){
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
	updateSnake(sp, dir);
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
