all:
	gcc -o main main.c game.c -lncurses -Wall -Wextra

run:
	gcc -o main main.c game.c -lncurses -Wall -Wextra
	./main

leak:
	gcc -o main -ggdb3 main.c game.c -lncurses
	valgrind ./main