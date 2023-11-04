#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define STATES 1000

int main(void)
{
    size_t len, size;
    
    pixel **pixelArr = initPixelArr(&len, &size);
    if (pixelArr == NULL)
        return 1;
    
    pixelArr = readPixels(pixelArr, "test.csv", &len, &size);
    if(pixelArr == NULL)
        return 1;
    

    printPixelArr(pixelArr, len, size);

    for (size_t i = 0; i < STATES; i++)
    {
        pixel **newPixelArr = check(pixelArr, &len, &size);
        if(newPixelArr == NULL)
        {
            freePixelArr(pixelArr, len);
            return 1;
        }
        pixelArr = newPixelArr;

        printf("\nTHE CURRENT ITERATION IS: %d\n", i+1);
        printPixelArr(pixelArr, len, size);
    }

    freePixelArr(pixelArr, len);

    return 0;
}