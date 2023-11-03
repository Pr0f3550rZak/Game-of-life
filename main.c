#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    size_t len, size;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return -1;
    
    pixelArr = appendPixel(pixelArr, &len, &size, 1, 1);
    if (len == 0)
        return 1;
    
    pixelArr = appendPixel(pixelArr, &len, &size, 2, 2);
    if (len == 1)
        return 2;

    pixelArr = appendPixel(pixelArr, &len, &size, 3, 3);
    if (len == 2)
        return 3;
    
    printPixelArr(pixelArr, len);
    freePixelArr(pixelArr, len);

    return 0;
}