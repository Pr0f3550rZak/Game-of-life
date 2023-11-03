#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t len, size;
    size_t oldlen = 0;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return -1;
    
    oldlen = len;
    pixelArr = appendPixel(pixelArr, &len, &size, 1, 1);
    if (len == oldlen)
    {
        freePixelArr(pixelArr, len);
        return -1;
    }
    
    oldlen = len;
    pixelArr = appendPixel(pixelArr, &len, &size, 1, 1);
    if (len == oldlen)
    {
        freePixelArr(pixelArr, len);
        return -1;
    }
    
    oldlen = len;
    pixelArr = appendPixel(pixelArr, &len, &size, 1, 1);
    if (len == oldlen)
    {
        freePixelArr(pixelArr, len);
        return -1;
    }
    
    oldlen = len;
    pixelArr = appendPixel(pixelArr, &len, &size, 1, 1);
    if (len == oldlen)
    {
        freePixelArr(pixelArr, len);
        return -1;
    }

    printPixelArr(pixelArr, len, size);
    freePixelArr(pixelArr, len);

    return 0;
}