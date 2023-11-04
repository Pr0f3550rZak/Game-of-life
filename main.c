#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define STATES 5

int main(void)
{
    size_t len, size;
    // size_t oldlen = 0;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return -1;
    
    pixelArr = readPixels(pixelArr, "test.csv", &len, &size);

    printPixelArr(pixelArr, len, size);

    for (size_t i = 0; i < STATES; i++)
    {
        pixelArr = check(pixelArr, &len, &size);
        printPixelArr(pixelArr, len, size);
    }

    freePixelArr(pixelArr, len);

    return 0;
}