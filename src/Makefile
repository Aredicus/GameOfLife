FLAGS = -Wall -Werror -Wextra

all : game

game : game_of_life(1).c
	gcc $(FLAGS) game_of_life(1).c -o ../build/game.

rebuild :
	clean all

clean : 
	rm ../build/*

