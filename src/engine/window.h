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


typedef enum{
    PW_WINDOW_SURFACE = 0, 
    PW_WINDOW_RENDERER = 1,
    PW_WINDOW_OPENGL = 2
} PWWindowRenderingApi;

typedef struct{
    int w;
    int h;
    char title[MAX_TITLE_LEN];
    bool isrunning;

    FormatImage fcontext;
    Image       context;

    PWWindowRenderingApi render_api;
    int id;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* context_texture;
    SDL_GLContext gl_context;
} PWWindow;

typedef PWWindow Window;

// Inits
int pw_window_create(PWWindow** win, int w, int h, const char* title, bool fs, PWWindowRenderingApi api);
void pw_window_destroy(PWWindow** win);

// Render
void pw_window_present(PWWindow* win);
void pw_window_prepare_renderer(Window* win);
void pw_window_present_renderer(Window* win);
void pw_widnow_clear(PWWindow* win);

void pw_window_visible(Window* window, bool visible);
int pw_window_set_icon(PWWindow* win, const char* icon_path);
void pw_window_get_info(PWWindow* window);

void pw_mouse_visible(bool visible);

#endif