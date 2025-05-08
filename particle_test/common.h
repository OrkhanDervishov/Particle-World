#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include <math.h>


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255


typedef struct
{
    int x, y;
} Pos;


typedef struct{
    int r, g, b, a;
} Color;


typedef enum{
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    LYELLOW,
    GRAY,
    PURPLE,
    BROWN
} Colors;

#define SWAP(a, b, t) t = a, a = b, b = t


void** Malloc2D(int rows, int cols, int elemSize);
clock_t FindDelta(clock_t old);

#endif //COMMON_H