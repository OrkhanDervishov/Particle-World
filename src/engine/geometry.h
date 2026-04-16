#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "core.h"

Rect CorrectRect(Rect rect, int maxWidth, int maxHeight);
Rect CorrectRect2(Rect rect, int maxWidth, int maxHeight);
Rect get_intersection_rect(Rect a, Rect b);
Rect get_intersection_rect2(Rect a, Rect b);
vec2 correct_start_pos(Rect rect, vec2 pos);
Rect CorrectRectCustom(Rect rect, int minX, int minY, int maxX, int maxY);
Rect CorrectRectFully(Rect rect, int maxWidth, int maxHeight);
Rect CombineRects(Rect a, Rect b);

#endif