#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"
#include "painter.h"
#include "platform.h"
#include "sdl2_layer.h"


#define SCR_WIDTH 1536
#define SCR_HEIGHT 1024

#define WIN_TITLE "SDL Playground"
#define MAX_TITLE_LEN 512


typedef struct{
    int w;
    int h;
    char title[MAX_TITLE_LEN];
    bool isrunning;



    FormatImage fcontext;
    Image       context;

    SDL_Window* window;
    SDL_GLContext* gl_context;
    // SDL_Renderer* renderer;
} Window;


// Inits
int CreateWindow(Window** win, int w, int h, const char* title, bool fs);
void DestroyWindow(Window** win);

// Render
void pw_window_present(Window* win);
void Clear(Window* win);

int pw_window_set_icon(Window* win, const char* icon_path);
void GetWindowInfo(Window* window);

#endif