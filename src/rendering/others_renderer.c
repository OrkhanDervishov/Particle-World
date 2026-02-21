#include "others_renderer.h"

void DrawGenerationCircle(Window* window, int x, int y, int rad, Color color){

    x = rad - 1;
    y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (rad << 1);

    while(x >= y){
        int cx = x;
        int cy = y;
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        PutPixel(&(window->screen), x + rad, y + rad, color);
        // DestructionLine(sim, px, py, px + cx, py + cy, power, replaceWith);
        // DestructionLine(sim, px, py, px + cy, py + cx, power, replaceWith);
        // DestructionLine(sim, px, py, px - cy, py + cx, power, replaceWith);
        // DestructionLine(sim, px, py, px - cx, py + cy, power, replaceWith);
        // DestructionLine(sim, px, py, px - cx, py - cy, power, replaceWith);
        // DestructionLine(sim, px, py, px - cy, py - cx, power, replaceWith);
        // DestructionLine(sim, px, py, px + cy, py - cx, power, replaceWith);
        // DestructionLine(sim, px, py, px + cx, py - cy, power, replaceWith);

        if(err <= 0){
            y++;
            err += dy;
            dy += 2;  
        }
        if(err > 0){
            x--;
            dx += 2;
            err += dx - (rad << 1);
        }
    }
}