#include "game.h"
#include <stdbool.h>
#include <sys/types.h>

/*
    We thought about implementing this in the fastest way possible but at the end of the day we realized that it 
    didn't really matter that much, and smart people has optimized this algorithm before us, like in this link:
    https://dotat.at/prog/life/life.html 
    Thus we didn't really want to waste our time, and after thinking a bit on how to best implement the data structure we just
    went and implement python-like array and didn't think further. :thumbsup:
    If we will have the time we will revisit out implementations. But for now I think it suffices.
*/


static pixel *createPixel(const coordinate_size x, const coordinate_size y)
{
    pixel *newPixel = (pixel *) malloc(sizeof(pixel));
    if(newPixel == NULL)
        return NULL;

    newPixel->x = x;
    newPixel->y = y;
    return newPixel;
}

static long strnlong(const char* str, int maxLen) {
    long result = 0;
    int i = 0, sign = 1;

    if(str[0] == '-')
    {
        sign = -1;
        i++;
    }

    for (; (i < maxLen || maxLen == -1) && str[i] != '\0'; i++)
        if (str[i] >= '0' && str[i] <= '9')
            result = result * 10 + (str[i] - '0');

    return result * sign;
}

static bool isNeighbor(const pixel *p1, const pixel *p2)
{
    coordinate_size dx = p1->x - p2->x;
    coordinate_size dy = p1->y - p2->y;
    if (dx == 0 && dy == 0)
        return false;
    return labs(dx) <= 1 && labs(dy) <= 1;
}

static bool isInPixelArr(const pixel **arr, const size_t len, const pixel p)
{
    for (size_t i = 0; i < len; i++)
        if (arr[i]->x == p.x && arr[i]->y == p.y)
            return true;
    
    return false;
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

pixel **appendPixel(pixel **arr, size_t *len, size_t *size, const coordinate_size x, const coordinate_size y)
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
    arr[(*len)++] = newPixel;

    return arr;
}

pixel **readPixels(pixel **arr, const char *filename, size_t *len, size_t *size)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return arr;
    
    size_t lineLen;
    ssize_t read;
    char *line;

    while((read = getline(&line, &lineLen, file)) != -1)
    {
        ssize_t i;
        for(i = 0; i<read && line[i] != CSV_SEP; i++) {}
        
        if (i == read) //didn't find sep char
        {
            free(line);
            fclose(file);
            return arr;
        }
        
        pixel np = {strnlong(line, i), strnlong(line+i+1, -1)};

        if(!isInPixelArr((const pixel **)arr, *len, np))
        {
            size_t oldlen = *len;
            arr = appendPixel(arr, len, size, np.x, np.y);
            if(*len == oldlen)
            {
                free(line);
                fclose(file);
                freePixelArr(arr, *len);
                return NULL;
            }
        }

    }

    free(line);
    fclose(file);
    return arr;
}

pixel **check(pixel **arr, size_t *len, size_t *size)
{
    /*
        ze bezt algorizm in ze wezt (uzing balloc)
        Time complexity: O(n^2).  
    */
   
    size_t newlen, newsize;
    pixel **newArr = initPixelArr(&newlen, &newsize);
    if(newArr == NULL)
        return NULL;

    // ADD NEW PIXELS
    for (size_t i = 0; i < *len; i++)
    {
        for (short dx = -1; dx <= 1; dx++)
            for (short dy = -1; dy <= 1; dy++)
            {
                if(dy == 0 && dx == 0)
                    continue;
                const pixel np = {arr[i]->x + dx, arr[i]->y + dy};

                if(!isInPixelArr((const pixel **)arr, *len, np) && !isInPixelArr((const pixel **)newArr, newlen, np))
                {
                    unsigned short neighborCount = 0;
                    for (size_t j = 0; j < *len; j++)
                        if (isNeighbor(&np, arr[j]))
                            neighborCount++;

                    if (neighborCount == 3)
                    {
                        size_t oldlen = newlen;
                        newArr = appendPixel(newArr, &newlen, &newsize, np.x, np.y);
                        if(newlen == oldlen)
                        {
                            freePixelArr(newArr, newlen);
                            return NULL;
                        }
                    }
                }
            }
    }

    // REMOVE ALIVE
    for (size_t i = 0; i < *len; i++)
    {
        unsigned short neighborCount = 0;
        for (size_t j = 0; j < *len; j++)
            if (isNeighbor(arr[i], arr[j]))
                neighborCount++;

        if (neighborCount == 2 || neighborCount == 3)
        {
            size_t oldlen = newlen;
            newArr = appendPixel(newArr, &newlen, &newsize, arr[i]->x, arr[i]->y);
            if(newlen == oldlen)
            {
                freePixelArr(newArr, newlen);
                return NULL;
            }
        }
    }

    freePixelArr(arr, *len);
    *len = newlen;
    *size = newsize;

    return newArr;
}

void freePixelArr(pixel **arr, const size_t len)
{
    for (size_t i = 0; i < len; i++)
        free(arr[i]);
    
    free(arr);
}

void printPixelArr(pixel **arr, const size_t len, const size_t size)
{
    printf("Printing Pixel Array. len: %lu size: %lu\n", len, size);
    for (size_t i = 0; i < len; i++)
        printf("%li, %li\n", arr[i]->x, arr[i]->y);
}
