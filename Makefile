CC = gcc
NAME = free_snake
LIBS = -lncurses -lm

$(NAME): draw.o
	$(CC) -o $(NAME) draw.o $(LIBS)

draw.o: draw.c

.PHONY: clean
clean:
	rem *.o $(NAME)
