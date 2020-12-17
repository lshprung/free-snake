#ifndef BODY_H
#define BODY_H

typedef struct node NODE;

typedef struct snake SNAKE;

SNAKE *makeSnake(int startx, int starty, int len);
void deleteSnake(SNAKE *sp);

void updateSnake(SNAKE *sp, int dir);

int getSnakeTailx(SNAKE *sp);
int getSnakeTaily(SNAKE *sp);
int getSnakeHeadx(SNAKE *sp);
int getSnakeHeady(SNAKE *sp);

#endif
