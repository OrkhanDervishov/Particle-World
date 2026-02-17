#ifndef PAINTER_H
#define PAINTER_H

#include "core.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// #define SWAP(a, b, t) t = a; a = b; b = t;
#define IMG_GET(img, x, y) (img)->buffer[y*(img)->width + x]


#define DEFAULT_IMAGE_WIDTH 1920
#define DEFAULT_IMAGE_HEIGHT 1080

typedef struct{
    int width;
    int height;
    Color* buffer;
} Image;


void createImage(Image* img, size_t w, size_t h);
void deleteImage(Image* img);

void fillImage(Image* img, Color color);
Color getRandomColor();

// TODO: Add fill option
void drawCircle(Image* img, vec2 p, int radius, Color color);
void drawRect(Image* img, Rect rect, Color color);

void drawTriangle(Image* img, Triangle t, Color color);
void drawMesh(Image* img, Triangle* triangles, int triangleCount, Color color);
// Bresenham
void drawLine(Image* img, Color c, int x0, int y0, int x1, int y1);

// Xiaolin Wu
void drawLineAntiAliased(Image* img, Color c, int x0, int y0, int x1, int y1);

#define POINT_RADIUS 3
void drawLineFromPoints(Image* img, vec2* points, int count, Color color, int drawPoints);

int saveImagePPM(Image* img, char* filename);
int saveImagePNG(Image* img, char* filename);
int loadPNG(Image* img, char* filename);

#endif