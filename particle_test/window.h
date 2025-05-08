#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

//Third party
#include <SDL2/SDL.h>

typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int isrunning;
    char title[MAX_TITLE_LEN];
    int w;
    int h;    
} Window;

int InitWindow(Window** win, int w, int h, const char* title);
void DestroyWindow(Window** win);


#endif //WINDOW_H