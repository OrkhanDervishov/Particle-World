#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

// Third party
#include <SDL2/SDL.h>
// typedef SDL_Surface Surface;
// typedef SDL_Texture Texture;

#define INF 0x3F3F3F3F
#define GENERIC_FUNC_POINTER void (*)()

typedef enum{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
} Sides;

typedef struct{
    int x, y;
} vec2;
typedef vec2 vec2i;
typedef vec2 Point;
typedef vec2 Pos;

typedef struct{
    int x, y, z;
} vec3;

typedef struct{
    int x, y, z, t;
} vec4;

typedef struct{
    float x, y;
} vec2f;
typedef vec2 Posf;

typedef struct{
    float x, y, z;
} vec3f;

typedef struct{
    float x, y, z, t;
} vec4f;


typedef struct
{
    int x, y;
    int w, h;
} Rect;

typedef struct{
    int x0, y0;
    int x1, y1;
    int x2, y2;
} Triangle;

typedef struct{
    vec2 a;
    vec2 b;
    vec2 c;
} Triangle2;

typedef struct{
    int x, y;
    int radius;
} Circle;

typedef enum{
    WHITE,
    BLACK,
    RED,
    PINK,
    GREEN,
    BLUE,
    LIGHT_BLUE,
    YELLOW,
    LYELLOW,
    ORANGE,
    GRAY,
    GOOD_GRAY,
    OBSIDIAN_GRAY,
    PURPLE,
    BROWN,
    OIL_BROWN,
    SAND_COLORS,
    WATER_COLORS,
    WALL_COLORS,
    ACID_COLORS,
    FIRE_COLORS,
    COAL_COLORS,
} Colors;


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255
#define MYELLOW 255, 255, 0, 255
#define MGOOD_GRAY 58, 58, 58, 255

#define byte char
#define bool uint8_t
#define TRUE 1
#define FALSE 0

#define CONSOLE(m) printf("%s", (m))
#define CONSOLE_RECT(r) printf("x:%d y:%d w:%d h:%d\n", (r).x, (r).y, (r).w, (r).h)

#define COPY_COLOR(dest, src) (dest)->rgba = (src)->rgba;
#define COMP_COLORS(c1, c2) ((c1)->rgba == (c2)->rgba ? 0 : 1)
#define SWAP(a, b, t) t = a, a = b, b = t
#define A_SWAP(a, b) {typeof((a)) (t) = (a); (a) = (b); (b) = (t);}


// Flag operations
#define CHECK_FLAG(p, f) ((p) & (f) ? 1 : 0)
#define SET_FLAG(p, f) ((p) |= (f))
#define CLEAR_FLAG(p, f) ((p) &= (f))
#define TOGGLE_FLAG(p, f) ((p) ^= (f))


// System
void* Malloc2D(int rows, int cols, int elemSize);
void Free2D(void* arr, int rows);
long GetTimeNano();


#endif