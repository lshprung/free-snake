#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG 1

void updateScore(int which, int score);

#ifdef DEBUG
void count_up(void);
#endif

int main(){
	int score = 0;
	int score2 = 0;
	int highscore;

	//TODO implement code to retreive existing high score
	highscore = 0;

	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	//TODO consider adding color

	//draw current score, and high score
	updateScore(0, score);
	updateScore(1, highscore);
	updateScore(2, score2);

	getch();

	//DEBUG
#ifdef DEBUG
	count_up();
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
