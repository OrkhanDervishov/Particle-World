#include "mouse.h"
#include "sdl2_layer.h"

void draw_cursor(Image context, Mouse mouse){
    vec2i mouse_pos = mouse_get_pos();
    pnt_blit_scaled(
        context, 
        mouse.cursor_img_normal, 
        mouse_pos.x - mouse.cursor_img_normal.width/2, 
        mouse_pos.y - mouse.cursor_img_normal.height/2, 
        1, 1
    );
}

int load_cursor_image(Mouse* mouse, const char* path){
    mouse->cursor_img_normal.buffer = NULL;
    return pnt_load_image(&mouse->cursor_img_normal, path);
}

void free_cursor_image(Mouse* mouse){
    pnt_delete_image(&mouse->cursor_img_normal);
}