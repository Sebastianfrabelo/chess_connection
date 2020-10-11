all: main

main: header.h main.c host_game.c join_game.c
	gcc -o main main.c host_game.c join_game.c
	./main
	
run: main
	./main