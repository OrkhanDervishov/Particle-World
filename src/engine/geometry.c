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

    // rect.w -= abs(rect.x);
    // rect.h -= abs(rect.y);
    return rect;
}

Rect CorrectRect2(Rect rect, int maxWidth, int maxHeight){

    
    // x, y correction
    if(rect.x < 0){
        rect.x = abs(rect.x);
        rect.w -= abs(rect.x);
    }
    else if(rect.x > maxWidth) rect.x = maxWidth;
    if(rect.y < 0){
        rect.y = abs(rect.y);
        rect.h -= abs(rect.y);
    }
    else if(rect.y > maxHeight) rect.y = maxHeight;
    
    // w, h correction
    if(rect.x + rect.w > maxWidth)
        rect.w = maxWidth - rect.x;
    if(rect.y + rect.h > maxHeight)
        rect.h = maxHeight - rect.y;

    // rect.w -= abs(rect.x);
    // rect.h -= abs(rect.y);
    return rect;
}

Rect get_intersection_rect(Rect a, Rect b){
    Rect rect;

    CONSOLE_RECT(a);
    CONSOLE_RECT(b);
    
    rect.x = 0;
    rect.y = 0;
    if(b.x < a.x) rect.x = a.x - b.x;
    if(b.y < a.y) rect.y = a.y - b.y;
    
    int a_endx = a.x + a.w;
    int a_endy = a.y + a.h;
    int b_endx = b.x + b.w;
    int b_endy = b.y + b.h;
    
    rect.w = b.w;
    rect.h = b.h;
    if(b_endx < a_endx) rect.w = (a_endx - rect.x) == 0 ? 0 : (a_endx - rect.x);
    if(b_endy < a_endy) rect.h = (a_endy - rect.y) == 0 ? 0 : (a_endy - rect.y);
    if(b_endx > a_endx) rect.w = (rect.x - a_endx) == 0 ? 0 : (rect.x - a_endx);
    if(b_endy > a_endy) rect.h = (rect.y - a_endx) == 0 ? 0 : (rect.y - a_endy);
    
    CONSOLE_RECT(rect);
    printf("------------\n");
    return rect;
}

Rect get_intersection_rect2(Rect a, Rect b){
    Rect rect;

    rect.x = a.x < b.x ? a.x : b.x;
    rect.y = a.y < b.y ? a.y : b.y;
    int endx = a.x+a.w < b.x+b.w ? a.x+a.w : b.x+b.w;
    int endy = a.y+a.h < b.y+b.h ? a.y+a.h : b.y+b.h;
    rect.w = endx;
    rect.h = endy;
    rect.x = abs(rect.x);
    rect.y = abs(rect.y);

    return rect;
}

vec2 correct_start_pos(Rect rect, vec2 pos){
    if(pos.x < rect.x) pos.x = 0;
    if(pos.y < rect.y) pos.y = 0;
    if(pos.x >= rect.x + rect.w) pos.x = rect.x + rect.w-1;
    if(pos.y >= rect.y + rect.h) pos.y = rect.y + rect.h-1;
    return pos;
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