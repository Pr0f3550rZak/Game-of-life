#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#else
#include <sys/types.h>
#include <unistd.h>
#define sleep(x) usleep((x)*1000)
#endif

//in miliseconds
// lower then 10 isn't reccommended, because we were too lazy to implement frambebuffers :)
#define TICK_RATE 100

static int keepRunning = 1;
static int x_add = 0;
static int y_add = 0;

void *keyListener(void *vargp)
{
    WINDOW *win = (WINDOW *)vargp;
    int c;
    while(keepRunning && (c = wgetch(win)))
    {
        if (c == KEY_UP)
            y_add++;
        if (c == KEY_DOWN)
            y_add--;
        if (c == KEY_LEFT)
            x_add++;
        if (c == KEY_RIGHT)
            x_add--;
        if (c == 3)
            keepRunning = 0;
    }
}

int main(void)
{
    size_t len, size;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return 1;
    
    pixelArr = readPixels(pixelArr, "test.csv", &len, &size);
    if(pixelArr == NULL)
        return 1;
    
    initscr();
    raw();
    noecho();
    curs_set(0);

    // TODO: fix resize window bugs
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *win = newwin(yMax, xMax, 0, 0);
    keypad(win, 1);
    pthread_t tid;
    pthread_create(&tid, NULL, keyListener, (void *)win); 

    do
    {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Game Of Life (%ld)", (long) getpid());

        for (size_t i = 0; i < len; i++)
        {
            // check if the new pixel is inside the screen
            coordinate_size x = pixelArr[i]->x + xMax/2 - x_add, y = pixelArr[i]->y + yMax/2 - y_add;
            if (!(x >= xMax-1 || y >= yMax-1 || x <= 0 || y <= 0))
                mvwprintw(win, y, x, "O");
        }
        wrefresh(win);

        pixel **newPixelArr = check(pixelArr, &len, &size);
        if(newPixelArr == NULL)
        {
            freePixelArr(pixelArr, len);
            return 1;
        }
        pixelArr = newPixelArr;

        sleep(TICK_RATE);
    } while(keepRunning);

    wclear(win);
    freePixelArr(pixelArr, len);
    endwin();

    return 0;   
}