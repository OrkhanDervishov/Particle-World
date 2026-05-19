#include "mouse.h"

void draw_cursor(FormatImage screen, Mouse mouse){
    vec2i mouse_pos = mouse_get_pos();
    // vec2i mouse_pos;
    // int state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    draw_image_on_fimage_scaled(screen, mouse.cursor_img_normal, mouse_pos.x, mouse_pos.y, 1, 1);
}

int load_cursor_image(Mouse* mouse, const char* path){
    mouse->cursor_img_normal.buffer = NULL;
    return load_png(&mouse->cursor_img_normal, path);
}

void free_cursor_image(Mouse* mouse){
    delete_image(&mouse->cursor_img_normal);
}