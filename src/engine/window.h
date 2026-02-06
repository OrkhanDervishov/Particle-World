#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"


// Third party
#include <SDL2/SDL.h>


#define SCR_WIDTH 1080
#define SCR_HEIGHT 800

#define WIN_TITLE "SDL Playground"
#define MAX_TITLE_LEN 256


typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isrunning;
    char title[MAX_TITLE_LEN];
    int w;
    int h;
} Window;


// Inits
int CreateWindow(Window** win, int w, int h, const char* title, bool fs);
void DestroyWindow(Window** win);

// Render
void Clear(Window* win);

int SetWindowIcon(Window* window, char* path);
void GetWindowInfo(Window* window);

#endif