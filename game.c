#include "game.h"

static pixel *createPixel(const coordinate_size x, const coordinate_size y)
{
    pixel *newPixel = (pixel *) malloc(sizeof(pixel));
    if(newPixel == NULL)
        return NULL;

    newPixel->x = x;
    newPixel->y = y;
    return newPixel;
}

pixel **initPixelArr(size_t *len, size_t *size)
{
    pixel **newArr = (pixel**) malloc(sizeof(pixel *));
    if (newArr == NULL)
        return NULL;
    
    *len = 0;
    *size = sizeof(pixel *);
    return newArr;
}

pixel **appendPixel(pixel *arr[], size_t *len, size_t *size, const coordinate_size x, const coordinate_size y)
{
    if(arr == NULL)
        return NULL;

    pixel *newPixel = createPixel(x, y);
    if(newPixel == NULL)
        return arr;
    size_t newSize = *size;

    while(((*len + 1) * sizeof(pixel*)) > newSize)
        newSize *= 2;
    
    pixel **tempArr = realloc(arr, newSize);
    if(tempArr == NULL)
    {
        free(newPixel);
        return arr;
    }

    arr = tempArr;
    *size = newSize;
    arr[*len] = newPixel;
    *len += 1;
    return arr;
}

void freePixelArr(pixel **arr, const int len)
{
    for (int i = 0; i < len; i++)
        free(arr[i]);
    
    free(arr);
}

void printPixelArr(pixel **arr, const int len)
{
    printf("Printing Pixel Array\n------------\n");
    for (int i = 0; i < len; i++)
        printf("%li, %li\n", arr[i]->x, arr[i]->y);
    printf("------------\nEND\n");
}