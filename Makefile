run:
	gcc -o main main.c game.c -Wall -Wextra
	./main

leak:
	gcc -o main -ggdb3 main.c game.c
	valgrind ./main