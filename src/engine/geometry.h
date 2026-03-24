#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "core.h"

Rect CorrectRect(Rect rect, int maxWidth, int maxHeight);
Rect CorrectRectCustom(Rect rect, int minX, int minY, int maxX, int maxY);
Rect CorrectRectFully(Rect rect, int maxWidth, int maxHeight);
Rect CombineRects(Rect a, Rect b);

#endif