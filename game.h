#ifndef _GAME_H
#define _GAME_H

#include <stdlib.h>
#include <stdio.h>

typedef long coordinate_size;
typedef struct{
	coordinate_size x;
	coordinate_size y;
} pixel;

pixel **initPixelArr(size_t *len, size_t *size);
pixel **appendPixel(pixel **arr, size_t *len, size_t *size, const coordinate_size x, const coordinate_size y);
// if len didn't changed realloc failed.
// when creating the pixel array give len 0 and size 0

void freePixelArr(pixel **arr, const size_t len);
void printPixelArr(pixel **arr, const size_t len, const size_t size);

#endif