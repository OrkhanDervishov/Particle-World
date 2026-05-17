#ifndef SDL_LAYER
#define SDL_LAYER

#include "core.h"
#include "platform.h"

#include <SDL2/SDL.h>
// #include <SDL2/SDL_video.h>
// #include <SDL2/SDL_mouse.h>


typedef SDL_Surface Surface;
// typedef SDL_Texture Texture;

vec2i mouse_get_pos();

int sdl2_init_opengl();

#endif