#include "geometry.h"

// Allows negative width and height
Rect CorrectRect(Rect rect, int maxWidth, int maxHeight){

    // x, y correction
    if(rect.x < 0) rect.x = 0;
    else if(rect.x > maxWidth) rect.x = maxWidth;
    if(rect.y < 0) rect.y = 0;
    else if(rect.y > maxHeight) rect.y = maxHeight;
    
    // w, h correction
    if(rect.x + rect.w > maxWidth)
        rect.w = maxWidth - rect.x;
    if(rect.y + rect.h > maxHeight)
        rect.h = maxHeight - rect.y;

    return rect;
}

// Allows negative width and height
Rect CorrectRectCustom(Rect rect, int minX, int minY, int maxX, int maxY){

    // x, y correction
    if(rect.x < minX) rect.x = minX;
    else if(rect.x > maxX) rect.x = maxX;
    if(rect.y < minY) rect.y = minY;
    else if(rect.y > maxY) rect.y = maxY;
    
    // w, h correction
    if(rect.x + rect.w > maxX)
        rect.w = maxX - rect.x;
    if(rect.y + rect.h > maxY)
        rect.h = maxY - rect.y;

    if(rect.w < 0) rect.w = 0;
    if(rect.h < 0) rect.h = 0;
    
    return rect;
}

// Does not allow negative width and height
Rect CorrectRectFully(Rect rect, int maxWidth, int maxHeight){

    // x, y correction
    if(rect.x < 0) rect.x = 0;
    else if(rect.x > maxWidth) rect.x = maxWidth;
    if(rect.y < 0) rect.y = 0;
    else if(rect.y > maxHeight) rect.y = maxHeight;
    
    // w, h correction
    if(rect.x + rect.w > maxWidth)
        rect.w = maxWidth - rect.x;
    if(rect.y + rect.h > maxHeight)
        rect.h = maxHeight - rect.y;

    if(rect.w < 0) rect.w = 0;
    if(rect.h < 0) rect.h = 0;

    return rect;
}


Rect CombineRects(Rect a, Rect b){
    return (Rect){
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.w > b.w ? a.w : b.w,
        a.h > b.h ? a.h : b.h
    };
}