#include "game.h"
#include "mstime.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(x)
#else
    #include <sys/types.h>
    #include <unistd.h>
    #define sleep(x) usleep((x)*1000)
#endif

#define TPS 20

static int keepRunning = 1;
static bool shouldRender = true;
static int x_add = 0;
static int y_add = 0;

void *keyListener(void *varg)
{
    char c;
    while(keepRunning && (c = getchar()))
    {
        switch (c)
        {
        case 'w':
            y_add++;
            shouldRender = true;
            break;
        case 's':
            y_add--;
            shouldRender = true;
            break;
        case 'a':
            x_add++;
            shouldRender = true;
            break;
        case 'd':
            x_add--;
            shouldRender = true;
            break;
        case 3:
            keepRunning = 0;
        }
    }

    pthread_exit(NULL);
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
    pthread_create(&tid, NULL, keyListener, NULL); 

    unsigned long long delta_time = 0;
    unsigned long long last_time = 0, current_time = 0;

    do
    {
        last_time = current_time;
        current_time = getTimeMs();
        delta_time += current_time - last_time;

        if (shouldRender)
        {
            // render
            wclear(win);
            // box(win, 0, 0); // the box is twiching a bit
            mvwprintw(win, 0, 2, "Game Of Life (%ld)", (long) getpid());

            for (size_t i = 0; i < len; i++)
            {
                // check if the new pixel is inside the screen
                coordinate_size x = pixelArr[i]->x + xMax/2 - x_add;
                coordinate_size y = pixelArr[i]->y + yMax/2 - y_add;
                if (!(x >= xMax-1 || y >= yMax-1 || x <= 0 || y <= 0))
                    mvwprintw(win, y, x, "O");
            }
            wrefresh(win);

            shouldRender = false;
        }

        if (delta_time >= 1000/TPS)
        {
            // tick
            pixel **newPixelArr = check(pixelArr, &len, &size);
            if(newPixelArr == NULL)
            {
                freePixelArr(pixelArr, len);
                return 1;
            }
            pixelArr = newPixelArr;

            shouldRender = true;
            delta_time = 0;
        }

        
    } while(keepRunning);

    wclear(win);
    freePixelArr(pixelArr, len);
    endwin();

    return 0;   
}