#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "include/body.h"

typedef struct node{
	int x;
	int y;
	struct node *next;
} NODE;

typedef struct snake{
	NODE *head; //"head" will be the physical tail of the snake
	NODE *tail; //"tail" will be the physical head of the snake
	int length;
} SNAKE;

//private prototypes
NODE *makeNode(int x, int y);
void deleteNode(NODE *np);
void increaseSnake(SNAKE *sp);

//public functions
SNAKE *makeSnake(int startx, int starty, int len){
	SNAKE *new = malloc(sizeof(SNAKE));
	NODE *np;

	assert(new != NULL);

	new->length = len;

	new->head = makeNode(startx-len+1, starty);
	np = new->head;
	len--;

	while(len > 0){
		np->next = makeNode(startx-len+1, starty);
		np = np->next;
		len--;
	}

	new->tail = np;

	return new;
}

void deleteSnake(SNAKE *sp){
	NODE *hold;

	if(sp != NULL){
		while(sp->head != NULL){
			hold = sp->head;
			sp->head = sp->head->next;
			deleteNode(hold);
		}
	}

	free(sp);
	return;
}

void updateSnake(SNAKE *sp, int dir, int maxx, int maxy, bool *collision, int *fruit_loc, bool *fruit, int *score){
	NODE *np;

	if(sp == NULL) return;

	//move the "back" to the "front" (note that sp->tail will not be set until after np->x and np->y are set)
	np = sp->head;
	sp->head = sp->head->next;
	sp->tail->next = np;
	np->next = NULL;

	//dir decides direction the snake is moving
	switch(dir){
		case(0): //snake is moving up
			np->x = sp->tail->x;
			np->y = sp->tail->y-1;
			break;

		case(1): //snake is moving right
			np->x = sp->tail->x+1;
			np->y = sp->tail->y;
			break;

		case(2): //snake is moving down
			np->x = sp->tail->x;
			np->y = sp->tail->y+1;
			break;

		case(3): //snake is moving left
			np->x = sp->tail->x-1;
			np->y = sp->tail->y;
	}

	//update tail pointer
	sp->tail = np;

	//check if eating fruit
	if(*fruit && sp->tail->x == *fruit_loc && sp->tail->y == *(fruit_loc + 1)){
		*fruit = false;
		(*score)++;

		//need to increase snake size
		increaseSnake(sp);
	}

	//check collision
	*collision = checkSnake(sp, maxx, maxy);

	return;
}

//true = collision, false = no collision
bool checkSnake(SNAKE *sp, int maxx, int maxy){
	NODE *np;

	if(sp == NULL) return true;

	//check if oob
	if(sp->tail->x < 0 || sp->tail->x >= maxx || sp->tail->y < 0 || sp->tail->y >= maxy) return true;

	//check if collision with body (check each cell)
	np = sp->head;
	while(np != sp->tail && np != NULL){
		if(sp->tail->x == np->x && sp->tail->y == np->y) return true;
		np = np->next;
	}

	return false;
}

int getSnakeTailx(SNAKE *sp){

	if(sp != NULL && sp->tail != NULL) return sp->tail->x;
	return -1;

}
int getSnakeTaily(SNAKE *sp){

	if(sp != NULL && sp->tail != NULL) return sp->tail->y;
	return -1;

}

int getSnakeHeadx(SNAKE *sp){

	if(sp != NULL && sp->head != NULL) return sp->head->x;
	return -1;

}

int getSnakeHeady(SNAKE *sp){

	if(sp != NULL && sp->head != NULL) return sp->head->y;
	return -1;

}
			
//private functions
NODE *makeNode(int x, int y){
	NODE *new = malloc(sizeof(NODE));
	assert(new != NULL);

	new->x = x;
	new->y = y;
	new->next = NULL;

	return new;
}

void deleteNode(NODE *np){

	if(np != NULL) free(np);

	return;
}

void increaseSnake(SNAKE *sp){
	int xdiff = sp->head->next->x - sp->head->x;
	int ydiff = sp->head->next->y - sp->head->y;
	NODE *new = makeNode(sp->head->x - xdiff, sp->head->y - ydiff);

	new->next = sp->head;
	sp->head = new;
	sp->length++;

	return;
}
