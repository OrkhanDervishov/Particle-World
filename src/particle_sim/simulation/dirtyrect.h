#ifndef DIRTYRECT_H
#define DIRTYRECT_H

#include "engine_lib.h"

void dr_list_init(Rect** dr_list);
void dr_list_destroy(Rect** dr_list);

Rect dr_correct(Rect rect, int w, int h);
void dr_add(Rect* dr_list, int* dr_count, Rect rect, int chunk_width, int chunk_height);
void dr_delete(Rect* dr_list, int* dr_count, int index);
void dr_rects_clear(Rect* dr_list, int* dr_count);
int dr_cut(
    Rect* out, Rect in, int chunk_w, int chunk_h,
    int minX, int minY, int maxX, int maxY
);

#endif