#ifndef _GAME_H
#define _GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CSV_SEP ','

typedef long coordinate_size;
typedef struct{
	coordinate_size x;
	coordinate_size y;
} pixel;

pixel **initPixelArr(size_t *len, size_t *size);
pixel **appendPixel(pixel **arr, size_t *len, size_t *size, const coordinate_size x, const coordinate_size y);
// if len didn't changed realloc failed.
// when creating the pixel array give len 0 and size 0
pixel **readPixels(pixel **arr, const char *filename, size_t *len, size_t *size);
pixel **check(pixel **arr, size_t *len, size_t *newlen, size_t *newsize);

bool isEqual(const pixel **arr1, const size_t len1, const pixel **arr2, const size_t len2);
void freePixelArr(pixel **arr, const size_t len);
void printPixelArr(pixel **arr, const size_t len, const size_t size);

#endif