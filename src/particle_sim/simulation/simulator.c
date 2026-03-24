#include "simulator.h"


void SimulateChunkHeatMapComplete(ChunkSpace* cs, Chunk* chunk, int cs_x, int cs_y){
    int rows = chunk->h;
    int cols = chunk->w;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){ 

            TYPE_DT p_type = CHUNK_GET_TYPE(*chunk, j, i);

            if(p_type == AIR || p_type == WALL) continue;

            HEAT_DT *p_heat = &CS_GET_HEAT(cs, j + cs_x, i + cs_y);

            TYPE_DT neighbor_types[] = {
                CS_GET_TYPE(cs, j + cs_x - 1, i + cs_y),
                CS_GET_TYPE(cs, j + cs_x + 1, i + cs_y),
                CS_GET_TYPE(cs, j + cs_x, i + cs_y - 1),
                CS_GET_TYPE(cs, j + cs_x, i + cs_y + 1)
            };

            HEAT_DT *neighbor_heats[] = {
                &CS_GET_HEAT(cs, j + cs_x - 1, i + cs_y),
                &CS_GET_HEAT(cs, j + cs_x + 1, i + cs_y),
                &CS_GET_HEAT(cs, j + cs_x, i + cs_y - 1),
                &CS_GET_HEAT(cs, j + cs_x, i + cs_y + 1)
            };



            for(int i = 0; i < 4; i++){
                TYPE_DT other_type = neighbor_types[i];
                HEAT_DT* other_heat = neighbor_heats[i];
                if(other_type == AIR || other_type == WALL) continue;
                int h = ((*p_heat) + (*other_heat)) / 2;
                *p_heat = h;
                *other_heat = h;
            }
        }
    }
}

int old_move = 0;
void SimulateChunkComplete(Chunk* chunk, int cs_x, int cs_y){
    int rows = chunk->h;
    int cols = chunk->w;

    int mov = old_move;
    
    // CONSOLE("works10\n");
    for(int i = rows-1; i >= 0; i--){
        mov = mov ? 0 : 1;
        int nj = mov ? cols-1 : 0;
        int add = mov ? -1 : 1;
        for(int j = nj; mov ? j >= 0 : j < cols; j += add){
            int x = j;
            int y = i;

            if(CHUNK_GET_STATE(*chunk, j, i) <= P_USED) continue;
            CHUNK_GET_STATE(*chunk, j, i) = P_USED;

            if(typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)] == NULL) {continue;}
            typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)](x + cs_x, y + cs_y);
        }
    }
    // old_move = old_move ? 0 : 1;

    for(int i = 0; i < chunk->size; i++){
        if(chunk->state[i] == P_USED){
            chunk->state[i] = P_FRESH;
        }
    }
}




Rect SimulateChunkSpaceRect(ChunkSpace* cs, Chunk* chunk, Rect rect, int cs_x, int cs_y){
    int minx = chunk->w + chunk->w/2, miny = chunk->h + chunk->h/2;
    int maxx = -chunk->w/2, maxy = -chunk->h/2;
    int mov = 0;

    int rectx_end = rect.x + rect.w;
    int recty_end = rect.y + rect.h;

    for(int i = rect.y; i < recty_end; i++){
        int y = i + cs_y;

        mov = mov ? 0 : 1;
        int nj = mov ? rectx_end-1 : rect.x;
        int add = mov ? -1 : 1;
        for(int j = nj; mov ? j >= rect.x : j < rectx_end; j += add){
            int x = j + cs_x;

            if(CS_GET_STATE(cs, x, y) <= P_USED) continue;
            CS_GET_STATE(cs, x, y) = P_USED;
            
            if(typeFuncList[CS_GET_TYPE(cs, x, y)] == NULL) continue;
            
            // Particle logic
            bool changed = typeFuncList[CS_GET_TYPE(cs, x, y)](x, y);

            // Rectangle checking
            if(changed == TRUE){
                minx = minx < j ? minx : j;
                miny = miny < i ? miny : i;
                maxx = maxx > j ? maxx : j;
                maxy = maxy > i ? maxy : i;
            }
        }
    }

    
    int add = 2;
    Rect new_rect;
    new_rect.x = minx - add;
    new_rect.y = miny - add;
    new_rect.w = maxx - new_rect.x + add;
    new_rect.h = maxy - new_rect.y + add;


    new_rect = CorrectRectCustom(
        new_rect, 
        -(chunk->w/2), -(chunk->h/2), 
        chunk->w + chunk->w/2, chunk->h + chunk->h/2
    );

    Rect new_rect_correct = (Rect){
        new_rect.x + cs_x,
        new_rect.y + cs_y,
        new_rect.w,
        new_rect.h
    };

    new_rect_correct = CorrectRectFully(new_rect_correct, cs->width_p, cs->height_p);

    new_rect = (Rect){
        new_rect_correct.x - cs_x,
        new_rect_correct.y - cs_y,
        new_rect_correct.w,
        new_rect_correct.h
    };

    return new_rect;
}


Rect SimulateRect(Chunk* chunk, Rect rect, int cs_x, int cs_y){
    Rect new_rect;

    int minx = chunk->w, miny = chunk->h;
    int maxx = 0, maxy = 0;
    int mov = 0;
    int rectx_end = rect.x + rect.w;
    int recty_end = rect.y + rect.h;

    for(int i = rect.y; i < recty_end; i++){
        int y = i;

        mov = mov ? 0 : 1;
        int nj = mov ? rectx_end-1 : rect.x;
        int add = mov ? -1 : 1;
        for(int j = nj; mov ? j >= rect.x : j < rectx_end; j += add){
            int x = j;
            if(CHUNK_GET_STATE(*chunk, j, i) > P_USED){
                if(typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)] == NULL) continue;

                // Particle logic
                CHUNK_GET_STATE(*chunk, j, i) = P_USED;
                bool changed = typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)](x+cs_x, y+cs_y);

                // Rectangle checking
                if(changed){
                    minx = minx < x ? minx : x;
                    miny = miny < y ? miny : y;
                    maxx = maxx > x ? maxx : x;
                    maxy = maxy > y ? maxy : y;
                }
            }
        }
    }
    int add = 5;
    new_rect.x = minx - add;
    new_rect.y = miny - add;
    new_rect.w = maxx - new_rect.x + add;
    new_rect.h = maxy - new_rect.y + add;
    new_rect = dr_correct(new_rect, chunk->w, chunk->h);
    return new_rect;
}

void AddRects(ChunkSpace* cs, int j, int i, Rect* rects);

void SimulateRects(ChunkSpace* cs, Chunk* chunk, int cs_x, int cs_y){
    Rect* list = chunk->dirty_rect_list;
    Rect maxRect = {0, 0, 0, 0};

    int cs_x_coor = cs_x*cs->chunk_width;
    int cs_y_coor = cs_y*cs->chunk_height;

    for(int i = 0; i < chunk->dr_count; i++){
        Rect oldRect = list[i];
        Rect newRect = SimulateChunkSpaceRect(cs, chunk, list[i], cs_x_coor, cs_y_coor);
        maxRect = CombineRects(oldRect, newRect);
        Rect rects[9];
        dr_cut(
            rects, newRect, chunk->w, chunk->h,
            -chunk->w/64, -chunk->h/64, 
            chunk->w + chunk->w/64, chunk->h + chunk->h/64
        );

        if(newRect.h <= 1 || newRect.w <= 1){
            dr_delete(list, &chunk->dr_count, i);
        }
        else {
            list[i] = rects[4];
            AddRects(cs, cs_x, cs_y, rects);
        }
    }
    // int chunk_endx = chunk->w + chunk->w/2;
    // int chunk_endy = chunk->h + chunk->h/2;
    // for(int i = -chunk->h/2; i < chunk_endy; i++){
    // for(int j = -chunk->w/2; j < chunk_endx; j++){
    //     if(j + cs_x_coor >= 0 && j + cs_x_coor < cs->width_p && i + cs_y_coor >= 0 && i + cs_y_coor < cs->height_p)
    //         if(CS_GET_STATE(cs, j + cs_x_coor, i + cs_y_coor) == P_USED){
    //                 CS_GET_STATE(cs, j + cs_x_coor, i + cs_y_coor) = P_FRESH;
    //             }
    //         }
    // }
    
    // printf("maxRect: x:%d y:%d w:%d h:%d\n", maxRect.x, maxRect.y, maxRect.w, maxRect.h);
    // int endx = cs_x_coor + maxRect.x + maxRect.w;
    // int endy = cs_y_coor + maxRect.y + maxRect.h;
    // printf("maxRect coors: x:%d y:%d x1:%d y1:%d\n", 
    //     cs_x_coor + maxRect.x, cs_y_coor + maxRect.y, 
    //     endx, endy
    // );
    // for(int i = cs_y_coor + maxRect.y; i < endy; i++)
    // for(int j = cs_x_coor + maxRect.x; j < endx; j++){
    //     if(CS_GET_STATE(cs, j, i) == P_USED){
    //         CS_GET_STATE(cs, j, i) = P_FRESH;
    //     }
    // }
}

void RefreshChunk(Chunk* chunk){
    for(int i = 0; i < chunk->size; i++){
        if(chunk->state[i] == P_USED){
            chunk->state[i] = P_FRESH;
        }
    }
}

void RefreshChunkSpace(ChunkSpace* cs){
    for(int i = cs->simStartY; i < cs->simEndY; i++)
    for(int j = cs->simStartX; j < cs->simEndX; j++){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i);
        RefreshChunk(chunk);
    }
}

void AddRects(ChunkSpace* cs, int j, int i, Rect* rects){
    if(rects[1].w != 0 && rects[1].h != 0){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i-1);
        dr_add(chunk->dirty_rect_list, &chunk->dr_count, rects[1], chunk->w, chunk->h);
    }
    if(rects[3].w != 0 && rects[3].h != 0){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j-1, i);
        dr_add(chunk->dirty_rect_list, &chunk->dr_count, rects[3], chunk->w, chunk->h);
    }
    if(rects[5].w != 0 && rects[5].h != 0){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j+1, i);
        dr_add(chunk->dirty_rect_list, &chunk->dr_count, rects[5], chunk->w, chunk->h);
    }
    if(rects[7].w != 0 && rects[7].h != 0){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i+1);
        dr_add(chunk->dirty_rect_list, &chunk->dr_count, rects[7], chunk->w, chunk->h);
    }
}

void SimulateChunkSpace(ChunkSpace *cs){

    // for(int i = cs->simStartY; i < cs->simEndY; i++)
    // for(int j = cs->simStartX; j < cs->simEndX; j++){
    //     Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i);
    //     SimulateRects(cs, chunk, j, i);
    //     // SimulateChunkComplete(&CS_GET_ARRAY_CHUNK(cs, j, i), j*cs->chunk_width, i*cs->chunk_height);
    //     // SimulateChunkHeatMapComplete(cs, &CS_GET_ARRAY_CHUNK(cs, j, i), j*cs->chunk_width, i*cs->chunk_height);
    // }
    for(int i = cs->simEndY-1; i >= cs->simStartY; i--)
    for(int j = cs->simEndX-1; j >= cs->simStartX; j--){
        Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i);
        SimulateRects(cs, chunk, j, i);
    }
}

void SimulateChunkSpaceDR(ChunkSpace *cs){

    // index:   0 1 2 3
    // offsetX: 0 1 0 1
    // offsetY: 0 0 1 1
    for(int k = 0; k < 4; k++){
        int offsetX = k % 2;
        int offsetY = k > 1 ? 1 : 0;

        for(int i = cs->simStartY + offsetY; i < cs->simEndY; i += 2)
        for(int j = cs->simStartX + offsetX; j < cs->simEndX; j += 2){
            Chunk* chunk = &CS_GET_ARRAY_CHUNK(cs, j, i);
            SimulateRects(cs, chunk, j, i);
        }
    }
}
