#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#else
#include <unistd.h>
#define sleep(x) usleep((x)*1000)
#endif

//in miliseconds
// lower then 10 isn't reccommended, because we were too lazy to implement frambebuffers :)
#define TICK_RATE 10  

static int keepRunning = 1;
static WINDOW *intWin;

void handler(int sig){
    char c = 'y';

    wprintw(intWin, "OUCH, did you hit Ctrl+C? (%d)\nDo you really want to end life? [y/n]", sig);
    wrefresh(intWin);
    // c = wgetch(intWin);
    if (c == 'y' || c == 'Y')
        keepRunning = 0;
    wclear(intWin);
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
    
    signal(SIGINT, handler);
    initscr();
    noecho();
    curs_set(0);

    // TODO: fix resize window bugs
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *win = newwin(yMax, xMax, 0, 0);
    intWin = newwin(yMax, xMax, 0, 0);

    do
    {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Game Of Life");

        for (size_t i = 0; i < len; i++)
        {
            // check if the new pixel is inside the screen
            coordinate_size x = pixelArr[i]->x + xMax/2, y = pixelArr[i]->y + yMax/2;
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

    freePixelArr(pixelArr, len);
    endwin();

    return 0;
    
}