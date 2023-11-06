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

static bool keepRunning = true;
static bool reachedEnd = false;
static bool shouldRender = true;
static int x_add = 0;
static int y_add = 0;

void *keyListener(void *varg)
{
    int c;
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
            keepRunning = false;
            break;
        case 27:
            getchar();
            switch (getchar())
            {
            case 65:
                y_add++;
                shouldRender = true;
                break;
            case 66:
                y_add--;
                shouldRender = true;
                break;
            case 67:
                x_add--;
                shouldRender = true;
                break;
            case 68:
                x_add++;
                shouldRender = true;
                break;
            }
            break;
        }
    }

    pthread_exit(varg);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main <filename>\n");
        return -1;
    }
    char *filename = argv[1];
    size_t len, size, newlen, newsize;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return 1;
    
    pixelArr = readPixels(pixelArr, filename, &len, &size);
    if(pixelArr == NULL)
        return 1;
    
    // no pixels were read
    if (len == 0)
    {
        freePixelArr(pixelArr, len);
        printf("No values inside the file!\n");
        return -1;
    }

    initscr();
    raw();
    noecho();
    curs_set(0);

    // TODO: fix resize window bugs
    int yMax, xMax;
    pthread_t tid;
    WINDOW *win = stdscr;

    keypad(win, 1);
    pthread_create(&tid, NULL, keyListener, NULL); 

    unsigned long long delta_time = 0;
    unsigned long long last_time = 0, current_time = 0;

    do
    {
        getmaxyx(win, yMax, xMax);
        last_time = current_time;
        current_time = getTimeMs();
        delta_time += current_time - last_time;

        if (shouldRender)
        {
            // render
            wclear(win);
            box(win, 0, 0); // the box is twiching a bit
            mvwprintw(win, 0, 2, "Game-Of-Life-(%ld)", (long) getpid());

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
            pixel **newPixelArr = check(pixelArr, &len, &newlen, &newsize);
            if(newPixelArr == NULL)
            {
                freePixelArr(pixelArr, len);
                return 1;
            }

            if (isEqual((const pixel **)pixelArr, len, (const pixel **)newPixelArr, newlen))
                reachedEnd = true;
            
            freePixelArr(pixelArr, len);
            len = newlen;
            size = newsize;
            pixelArr = newPixelArr;

            shouldRender = true;
            delta_time = 0;
        }
    } while(keepRunning && !reachedEnd);

    wclear(win);
    endwin();

    if (reachedEnd)
    {
        if (len == 0)
            printf("You ruined life! None of the pixels survived!!!\n");
        else
            printf("Congratz! Your pixels are gonna stay forever!!!\n");
    }

    freePixelArr(pixelArr, len);

    return 0;   
}