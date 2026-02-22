#include "renderer_sw.h"

void ClearWindow(Window *window, Color color){
    SDL_FillRect(
        SDL_GetWindowSurface(window->window), NULL, 
        SDL_MapRGB(
            SDL_GetWindowSurface(window->window)->format, 
            color.r, color.g, color.b
        )
    );
}