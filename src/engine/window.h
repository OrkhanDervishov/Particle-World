#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"
#include "painter.h"
#include "platform.h"
#include "sdl2_layer.h"


// Third party
// #include <SDL2/SDL.h>


#define SCR_WIDTH 1080
#define SCR_HEIGHT 800

#define WIN_TITLE "SDL Playground"
#define MAX_TITLE_LEN 512


typedef struct{
    int w;
    int h;
    char title[MAX_TITLE_LEN];
    bool isrunning;
    FormatImage context;
    SDL_Window* window;
    SDL_Renderer* renderer;
} Window;


// Inits
int CreateWindow(Window** win, int w, int h, const char* title, bool fs);
void DestroyWindow(Window** win);

// Render
void Clear(Window* win);

int SetWindowIcon(Window* window, char* path);
void GetWindowInfo(Window* window);

#endif