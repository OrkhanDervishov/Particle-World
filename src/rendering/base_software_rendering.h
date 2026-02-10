#ifndef BASE_SOFTWARE_RENDERING_H
#define BASE_SOFTWARE_RENDERING_H

#include "../engine/engine_lib.h"

void DrawRectSW(Window* window, Rect r);
void DrawFilledRectSW(Window* window, Rect r);

void DrawCircleSW(Window* window, Circle c);
void DrawFilledCircleSW(Window* window, Circle c);

void DrawTriangleSW(Window* window, Triangle t);
void DrawMeshSW(Window* window, Triangle* t, int count);
// void DrawFilledCircleSW(Window* window, Rect r);


#endif