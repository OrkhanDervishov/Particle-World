#ifndef MOUSE_H
#define MOUSE_H

#include "painter.h"


typedef struct{
    Image cursor_img_normal;
} Mouse;

void draw_cursor(Image context, Mouse mouse);
int load_cursor_image(Mouse* mouse, const char* path);
void free_cursor_image(Mouse* mouse);

#endif