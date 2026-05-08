#include "sdl2_layer.h"

// Input system
vec2i mouse_get_pos(){
    vec2i pos;
    int state = SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}


// OpenGL
int sdl2_init_opengl(){
    
}