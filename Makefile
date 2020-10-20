all: main

main: header.h xadrez.h main.c host_game.c join_game.c main_game.c xadrez.c
	gcc -o main main.c host_game.c join_game.c main_game.c xadrez.c
	./main
	
run: main
	./main