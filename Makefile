all:
	gcc -o main main.c game.c mstime.c -lncurses -Wall -Wextra

run:
	gcc -o main main.c game.c mstime.c -lncurses -Wall -Wextra
	./main presets/test.csv

leak:
	gcc -o main -ggdb3 main.c game.c mstime.c -lncurses
	valgrind ./main presets/test.csv