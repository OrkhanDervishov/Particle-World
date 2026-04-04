#include "sdl2_layer.h"

vec2i mouse_get_pos(){
    vec2i pos;
    int state = SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}