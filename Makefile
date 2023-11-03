run:
	gcc -o main main.c game.c
	./main

leak:
	gcc -o main -ggdb3 main.c game.c
	valgrind ./main