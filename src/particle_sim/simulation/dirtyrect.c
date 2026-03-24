#include "dirtyrect.h"

#define MAX_RECT_COUNT 64

void dr_list_init(Rect** dr_list){
    *dr_list = (Rect*)malloc(sizeof(Rect) * MAX_RECT_COUNT);
}

void dr_list_destroy(Rect** dr_list){
    free(*dr_list);
}

Rect dr_correct(Rect rect, int w, int h){
    if(rect.x < 0) rect.x = 0;
    if(rect.x >= w) rect.x = w - 1;

    if(rect.y < 0) rect.y = 0;
    if(rect.y >= h) rect.y = h - 1;

    if(rect.x + rect.w < 0) rect.w = rect.x;
    if(rect.x + rect.w >= w) rect.w = w - rect.x - 1;

    if(rect.y + rect.h < 0) rect.h = rect.y;
    if(rect.y + rect.h >= h) rect.h = h - rect.y - 1;

    if(rect.w < 0) rect.w = 0;
    if(rect.h < 0) rect.h = 0;

    return rect;
}

void dr_add(Rect* dr_list, int* dr_count, Rect rect, int chunk_width, int chunk_height){
    if(*dr_count >= MAX_RECT_COUNT) return;
    rect = CorrectRect(rect, chunk_width, chunk_height);
    dr_list[(*dr_count)++] = rect;
}
void dr_delete(Rect* dr_list, int* dr_count, int index){
    if(index >= *dr_count) return;
    dr_list[index] = dr_list[*dr_count - 1];
    --(*dr_count);
}

void dr_rects_clear(Rect* dr_list, int* dr_count){
    *dr_count = 0;
}

int dr_cut(
    Rect* out, Rect in, int chunk_w, int chunk_h,
    int minX, int minY, int maxX, int maxY
){
    if(out == NULL) return 0;

    Rect rect = {0, 0, 0, 0};
    for(int i = 0; i < 9; i++){
        out[i] = rect;
    }
    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //         printf("[%d %d %d %d] | ", out[i*3 + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    int central_startx = in.x;
    int central_starty = in.y;
    int central_endx = in.x + in.w;
    int central_endy = in.y + in.h;

    int generated_rects_count = 0;

    int chunk_endx = chunk_w;
    int chunk_endy = chunk_h;

    int startX = in.x;
    int startY = in.y;
    int endX = in.x + in.w;
    int endY = in.y + in.h;

    // // Center
    // out[4] = CorrectRectCustom(in, 0, 0, chunk_endx, chunk_endy);
    // generated_rects_count++;

    int ru_gen = FALSE;
    int rd_gen = FALSE;
    int lu_gen = FALSE;
    int ld_gen = FALSE;

    // // Left Up corner 
    // if(startX <= minX && startY <= minY){
    //     Rect rect = CorrectRectCustom(in, minX, minY, 0, 0);
    //     out[0] = (Rect){
    //         .x = chunk_w + rect.x,
    //         .y = chunk_h + rect.y,
    //         .w = abs(rect.x),
    //         .h = abs(rect.y)
    //     };
    //     generated_rects_count++;
    //     lu_gen = TRUE;
    // }
    // // Left Down corner
    // if(startX <= minX && endY >= maxY){
    //     Rect rect = CorrectRectCustom(in, minX, maxY, 0, chunk_h);
    //     out[2] = (Rect){
    //         .x = chunk_w + rect.x,
    //         .y = chunk_h + rect.y,
    //         .w = abs(rect.x),
    //         .h = abs(rect.y)
    //     };
    //     generated_rects_count++;
    //     ld_gen = TRUE;
    // }
    // // Right Up corner
    // if(endX >= maxX && startY <= minY){
    //     Rect rect = CorrectRectCustom(in, maxX, minY, chunk_w, 0);
    //     out[6] = (Rect){
    //         .x = chunk_w + rect.x,
    //         .y = chunk_h + rect.y,
    //         .w = abs(rect.x),
    //         .h = abs(rect.y)
    //     };
    //     generated_rects_count++;
    //     ru_gen = TRUE;
    // }
    // // Right Down corner
    // if(endX >= maxX && endY >= maxY){
    //     Rect rect = CorrectRectCustom(in, maxX, maxY, chunk_w, chunk_h);
    //     out[8] = (Rect){
    //         .x = chunk_w + rect.x,
    //         .y = chunk_h + rect.y,
    //         .w = abs(rect.x),
    //         .h = abs(rect.y)
    //     };
    //     generated_rects_count++;
    //     rd_gen = TRUE;
    // }

    // Left side
    if(startX <= minX){
        central_startx = 0;
        int sy = startY;
        int ey = endY;
        if(lu_gen) sy = 0 < startY ? startY : 0;
        if(ld_gen) ey = chunk_endy > endY ? endY : chunk_endy;

        // Rect rect = (Rect){startX, sy, -startX, ey - sy};
        Rect lrect = CorrectRectCustom(in, minX, sy, 0, ey);
        out[3] = (Rect){
            .x = chunk_w + lrect.x,
            .y = lrect.y,
            .w = abs(lrect.w),
            .h = abs(lrect.h)
        };
        generated_rects_count++;
    }
    // Right side
    if(endX >= maxX){
        central_endx = chunk_endx;
        int sy = startY;
        int ey = endY;
        if(ru_gen) sy = 0;
        if(rd_gen) ey = chunk_endy;

        // Rect rect = (Rect){chunk_endx, sy, maxX - chunk_endx, ey - sy};
        Rect rrect = CorrectRectCustom(in, chunk_endx, sy, maxX, ey);
        // printf("x:%d y:%d w:%d h:%d\n", rect.x, rect.y, rect.w, rect.h);
        out[5] = (Rect){
            .x = rrect.x - chunk_w,
            .y = rrect.y,
            .w = abs(rrect.w),
            .h = abs(rrect.h)
        };
        generated_rects_count++;
    }
    // Up side
    if(startY <= minY){
        central_starty = 0;
        int sx = startX;
        int ex = endX;
        if(lu_gen) sx = 0;
        if(ru_gen) ex = chunk_endx;

        // Rect rect = (Rect){startX, sy, -startX, ey - sy};
        Rect urect = CorrectRectCustom(in, sx, minY, ex, 0);
        // printf("x:%d y:%d w:%d h:%d\n", rect.x, rect.y, rect.w, rect.h);
        out[1] = (Rect){
            .x = urect.x,
            .y = urect.y + chunk_h,
            .w = abs(urect.w),
            .h = abs(urect.h)
        };
        generated_rects_count++;
    }
    // Down side
    if(endY >= maxY){
        central_endy = chunk_endy;
        int sx = startX;
        int ex = endX;
        if(ld_gen) sx = 0;
        if(rd_gen) ex = chunk_endx;

        // Rect rect = (Rect){startX, sy, -startX, ey - sy};
        Rect drect = CorrectRectCustom(in, sx, chunk_endy, ex, maxY);
        // printf("x:%d y:%d w:%d h:%d\n", rect.x, rect.y, rect.w, rect.h);
        out[7] = (Rect){
            .x = drect.x,
            .y = drect.y - chunk_h,
            .w = abs(drect.w),
            .h = abs(drect.h)
        };
        generated_rects_count++;
    }
    
    // Center
    Rect central = (Rect){
        central_startx,
        central_starty,
        central_endx - central_startx,
        central_endy - central_starty
    };
    // out[4] = CorrectRectCustom(central, minX, minY, maxX, maxY);
    out[4] = central;
    generated_rects_count++;
    


    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //         printf("[%d %d %d %d] | ", 
    //             out[i*3 + j].x,
    //             out[i*3 + j].y,
    //             out[i*3 + j].w,
    //             out[i*3 + j].h
    //         );
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    return generated_rects_count;
}