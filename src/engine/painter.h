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


void CreateImage(Image* img, size_t w, size_t h);
void DeleteImage(Image* img);

void FillImage(Image* img, Color color);
void PutPixel(Image* img, int x, int y, Color color);
Color GetRandomColor();

Rect CorrectRectImage(Image* img, Rect rect);

// TODO: Add fill option
void DrawCircle(Image* img, int x, int y, int radius, Color color);
void DrawRect(Image* img, Rect rect, Color color);
void DrawFilledCircle(Image* img, int x, int y, int radius, Color color);
void DrawFilledRect(Image* img, Rect rect, Color color);

void DrawTriangle(Image* img, Triangle t, Color color);
void DrawMesh(Image* img, Triangle* triangles, int triangleCount, Color color);
// Bresenham
void DrawLine(Image* img, Color c, int x0, int y0, int x1, int y1);

// Xiaolin Wu
void DrawLineAntiAliased(Image* img, Color c, int x0, int y0, int x1, int y1);

#define POINT_RADIUS 3
void DrawLineFromPoints(Image* img, vec2* points, int count, Color color, int drawPoints);

int SaveImagePPM(Image* img, char* filename);
int SaveImagePNG(Image* img, char* filename);
int LoadPNG(Image* img, char* filename);

#endif