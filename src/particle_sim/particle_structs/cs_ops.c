#include "cs_ops.h"



// TODO: Clever rect creation and deletion, using individual chunks


void CreateParticleCS(ChunkSpace *cs, int x, int y, int type){
    if(CS_GET_CUSTOM(cs, x, y) != NULL){
        free(CS_GET_CUSTOM(cs, x, y));
    }
    CS_GET_STATE(cs, x, y) = P_FRESH;
    CS_GET_TYPE(cs, x, y) = type;
    CS_GET_COLOR(cs, x, y) = typeColorList[type][0];
    CS_GET_LIFE_T(cs, x, y) = 500;
    CS_GET_EFFECT_T(cs, x, y) = 500;
    CS_GET_HEAT(cs, x, y) = CHECK_FLAG(typeFlagsList[type], HEAT_RELEASER) ? 2000 : 0;;
}

void CreateMagicParticleCS(ChunkSpace *cs, int x, int y, int type, part_color_t color, part_lifet_t life_t){
    CS_GET_STATE(cs, x, y) = P_FRESH;
    CS_GET_TYPE(cs, x, y) = type;
    CS_GET_COLOR(cs, x, y) = color;
    CS_GET_LIFE_T(cs, x, y) = life_t - rand()%250;
}

void DeleteParticleCS(ChunkSpace *cs, int x, int y){
    CS_GET_STATE(cs, x, y) = P_IGNORED;
    CS_GET_TYPE(cs, x, y) = AIR;
    // CS_GET_COLOR(cs, x, y).a = 0;
    // CS_GET_COLOR(cs, x, y).rgba = 0xFFAAAAAA;
}

// NOTE: I need to ensure that chunk coordinates will stay in registers,
//       and will not be recalculated.
void SwapParticlesCS(ChunkSpace* cs, int x0, int y0, int x1, int y1){
    Chunk c0 = cs->chunks[
        ((y0 >> cs->height_power) * cs->width_c) + 
        (x0 >> cs->width_power)
    ];

    Chunk c1 = cs->chunks[
        ((y1 >> cs->height_power) * cs->width_c) + 
        (x1 >> cs->width_power)
    ];


    int i0 = (y0&(cs->chunk_height-1)) * c0.w + (x0&(cs->chunk_width-1));
    int i1 = (y1&(cs->chunk_height-1)) * c1.w + (x1&(cs->chunk_width-1));

    A_SWAP(CHUNK_GETI_STATE(c0, i0),    CHUNK_GETI_STATE(c1, i1)); 
    A_SWAP(CHUNK_GETI_TYPE(c0, i0),     CHUNK_GETI_TYPE(c1, i1)); 
    A_SWAP(CHUNK_GETI_XVEL(c0, i0),     CHUNK_GETI_XVEL(c1, i1)); 
    A_SWAP(CHUNK_GETI_YVEL(c0, i0),     CHUNK_GETI_YVEL(c1, i1)); 
    A_SWAP(CHUNK_GETI_COLOR(c0, i0),    CHUNK_GETI_COLOR(c1, i1)); 
    A_SWAP(CHUNK_GETI_EFFECT_T(c0, i0), CHUNK_GETI_EFFECT_T(c1, i1)); 
    A_SWAP(CHUNK_GETI_LIFE_T(c0, i0),   CHUNK_GETI_LIFE_T(c1, i1)); 
    A_SWAP(CHUNK_GETI_HEAT(c0, i0),     CHUNK_GETI_HEAT(c1, i1)); 
    A_SWAP(CHUNK_GETI_PFLAGS(c0, i0),   CHUNK_GETI_PFLAGS(c1, i1)); 
    A_SWAP(CHUNK_GETI_DURAB(c0, i0),    CHUNK_GETI_DURAB(c1, i1)); 
    A_SWAP(CHUNK_GETI_CUSTOM(c0, i0),   CHUNK_GETI_CUSTOM(c1, i1)); 
}

void ReplaceParticleCS(ChunkSpace *cs, int x, int y, int type){
    if(CS_GET_CUSTOM(cs, x, y) != NULL){
        free(CS_GET_CUSTOM(cs, x, y));
    }
    CS_GET_STATE(cs, x, y) = P_FRESH;
    CS_GET_TYPE(cs, x, y) = type;
    CS_GET_COLOR(cs, x, y) = typeColorList[type][0];
    CS_GET_LIFE_T(cs, x, y) = 500;
    CS_GET_EFFECT_T(cs, x, y) = 500;
    CS_GET_HEAT(cs, x, y) = CHECK_FLAG(typeFlagsList[type], HEAT_RELEASER) ? 2000 : 0;
}


//======================================================

void CreateParticlesRectCS(ChunkSpace *cs, int startX, int startY, int width, int height, int type){
    Rect r = {.x = startX, .y = startY, .w = width, .h = height};
    r = CorrectRect(r, cs->width_p-1, cs->height_p-1);
    startX = r.x;
    startY = r.y;

    int endX = startX + r.w;
    int endY = startY + r.h;

    int cX = startX + r.w/2;
    int cY = startY + r.h/2;
    Chunk* chunk = &cs->chunks[CS_GET_CHUNK_INDEX(cs, cX, cY)];
    dr_add(
        chunk->dirty_rect_list, &chunk->dr_count, 
        r, DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE
    );

    for(int i = startY; i < endY; i++)
    for(int j = startX; j < endX; j++){
        CS_GET_STATE(cs, j, i) = P_FRESH;
        CS_GET_TYPE(cs, j, i) = type;
        CS_GET_COLOR(cs, j, i) = typeColorList[type][0];
        CS_GET_LIFE_T(cs, j, i) = 500;
        CS_GET_EFFECT_T(cs, j, i) = 500;
        CS_GET_HEAT(cs, j, i) = CHECK_FLAG(typeFlagsList[type], HEAT_RELEASER) ? 2000 : 0;
        // TODO: Init particle data.
    }
}

void CreateParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad, int type){
    int starty = cY - rad;
    int startx = cX - rad;
    int endy = cY + rad;
    int endx = cX + rad;
    int srad = rad * rad;

    Chunk* chunk = &cs->chunks[CS_GET_CHUNK_INDEX(cs, cX, cY)];
    Rect rect = {
        .x = cX%DEFAULT_CHUNK_SIZE-rad,
        .y = cY%DEFAULT_CHUNK_SIZE-rad,
        .w = 2*rad,
        .h = 2*rad
    };
    dr_add(
        chunk->dirty_rect_list, &chunk->dr_count, 
        rect, DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE
    );
    
    for(int i = starty; i < endy; i++){
        int di = abs(cY - i);
        for(int j = startx; j < endx; j++){
            int dj = abs(cX - j);
            if(di*di + dj*dj < srad){
                if(j >= 0 && i >= 0 && j < (int)cs->width_p && i < (int)cs->height_p){
                    // if(sim->pMap[i][j].id < 0)
                    CS_GET_STATE(cs, j, i) = P_FRESH;
                    CS_GET_TYPE(cs, j, i) = type;
                    CS_GET_COLOR(cs, j, i) = typeColorList[type][0];
                    CS_GET_LIFE_T(cs, j, i) = 500 + rand()%100;
                    CS_GET_EFFECT_T(cs, j, i) = 50 + rand()%50;
                    CS_GET_HEAT(cs, j, i) = CHECK_FLAG(typeFlagsList[type], HEAT_RELEASER) ? 2000 : 0;
                }
            }
        }
    }
}

void DeleteParticlesRectCS(ChunkSpace *cs, int startX, int startY, int width, int height){
    Rect r = {.x = startX, .y = startY, .w = width, .h = height};
    r = CorrectRect(r, cs->width_p, cs->height_p);
    startX = r.x;
    startY = r.y;

    int endX = startX + r.w;
    int endY = startY + r.h;
    
    for(int i = startY; i < endY; i++)
    for(int j = startX; j < endX; j++){
        CS_GET_STATE(cs, j, i) = P_IGNORED;
        CS_GET_TYPE(cs, j, i) = AIR;
        // CS_GET_COLOR(cs, j, i).a = 0;
    }    
}

void DeleteParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad){
    int starty = cY - rad;
    int startx = cX - rad;
    int endy = cY + rad;
    int endx = cX + rad;
    int srad = rad * rad;

    Chunk* chunk = &cs->chunks[CS_GET_CHUNK_INDEX(cs, cX, cY)];
    Rect rect = {
        .x = cX%DEFAULT_CHUNK_SIZE-rad,
        .y = cY%DEFAULT_CHUNK_SIZE-rad,
        .w = 2*rad,
        .h = 2*rad
    };
    dr_add(
        chunk->dirty_rect_list, &chunk->dr_count, 
        rect, DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE
    );

    for(int i = starty; i < endy; i++){
        int di = abs(cY - i);
        for(int j = startx; j < endx; j++){
            int dj = abs(cX - j);
            if(di*di + dj*dj < srad){
                if(j >= 0 && i >= 0 && j < (int)cs->width_p && i < (int)cs->height_p){
                    CS_GET_STATE(cs, j, i) = P_IGNORED;
                    CS_GET_TYPE(cs, j, i) = AIR;
                    // CS_GET_COLOR(cs, j, i).a = 0;
                }
            }
        }
    }
}

//======================================================

void WallBoxCS(ChunkSpace *cs){

    for(size_t j = 0; j < cs->width_p; j++){
        CreateParticleCS(cs, j, 0, WALL);
        CreateParticleCS(cs, j, cs->height_p-1, WALL);
    }

    for(size_t i = 1; i < cs->height_p - 1; i++){
        CreateParticleCS(cs, 0, i, WALL);
        CreateParticleCS(cs, cs->width_p-1, i, WALL);
    }   
}

void ClearFullCS(ChunkSpace* cs){

    for(size_t i = 0; i < cs->size_c; i++){
        ClearChunkFull(&cs->chunks[i]);
    }
}

void ClearPartCS(ChunkSpace* cs){

    for(size_t i = 0; i < cs->size_c; i++){
        ClearChunkFull(&cs->chunks[i]);
    }
}

//======================================================

void Explosion(ChunkSpace *cs, int px, int py, int rad, int power, int replaceWith){
    int x = rad - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (rad << 1);

    while(x >= y){
        int cx = x;
        int cy = y;
        DestructionLine(cs, px, py, px + cx, py + cy, power, replaceWith);
        DestructionLine(cs, px, py, px + cy, py + cx, power, replaceWith);
        DestructionLine(cs, px, py, px - cy, py + cx, power, replaceWith);
        DestructionLine(cs, px, py, px - cx, py + cy, power, replaceWith);
        DestructionLine(cs, px, py, px - cx, py - cy, power, replaceWith);
        DestructionLine(cs, px, py, px - cy, py - cx, power, replaceWith);
        DestructionLine(cs, px, py, px + cy, py - cx, power, replaceWith);
        DestructionLine(cs, px, py, px + cx, py - cy, power, replaceWith);

        if(err <= 0){
            y++;
            err += dy;
            dy += 2;  
        }
        if(err > 0){
            x--;
            dx += 2;
            err += dx - (rad << 1);
        }
    }
}


void DestructionLine(ChunkSpace *cs, int x0, int y0, int x1, int y1, int power, int replaceWith){
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    
    while(1){
        if(x0 >= 0 && y0 >= 0 && x0 < cs->width_p && y0 < cs->height_p){
            if(GetTypeCS(cs, x0, y0) == WALL) break;
            CreateParticleCS(cs, x0, y0, replaceWith);
            SetHeatCS(cs, x0, y0, power * 10);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * error;
            if(e2 >= dy){
                if(x0 == x1) break;
                error += dy;
                x0 += sx;
            }
            if(e2 <= dx){
                if(y0 == y1) break;
                error += dx;
                y0 += sy;
            }
        }
        else{
            break;
        }
    }
    // int dx = abs(px1 - px0);
    // int sx = px0 < px1 ? 1 : -1;
    // int dy = -abs(py1 - py0);
    // int sy = py0 < py1 ? 1 : -1;
    // int error = dx + dy;

    // Color c;
    // ChangeColor(&c, FIRE_COLORS);

    // while (1) {
    //     CreateReplaceParticle(sim, px0, py0, &c, FIRE);

    //     if (px0 == px1 && py0 == py1) break;

    //     int e2 = 2 * error;

    //     if (e2 >= dy) {
    //         error += dy;
    //         px0 += sx;
    //     }

    //     if (e2 <= dx) {
    //         error += dx;
    //         py0 += sy;
    //     }
    // }
}


void CreationLineCS(ChunkSpace* cs, int x0, int y0, int x1, int y1, int width, int type){

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    while(1){
        if(x0 >= 0 && y0 >= 0 && x0 < cs->width_p && y0 < cs->height_p){
            // CreateParticleCS(cs, x0, y0, type);
            CreateParticlesCircleCS(cs, x0, y0, width, type);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * error;
            if(e2 >= dy){
                if(x0 == x1) break;
                error += dy;
                x0 += sx;
            }
            if(e2 <= dx){
                if(y0 == y1) break;
                error += dx;
                y0 += sy;
            }
        }
        else{
            break;
        }
    }
}

void DeletionLineCS(ChunkSpace* cs, int x0, int y0, int x1, int y1, int width){

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    while(1){
        if(x0 >= 0 && y0 >= 0 && x0 < cs->width_p && y0 < cs->height_p){
            DeleteParticlesCircleCS(cs, x0, y0, width);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * error;
            if(e2 >= dy){
                if(x0 == x1) break;
                error += dy;
                x0 += sx;
            }
            if(e2 <= dx){
                if(y0 == y1) break;
                error += dx;
                y0 += sy;
            }
        }
        else{
            break;
        }
    }
}


void PutParticleImageCS(ChunkSpace* cs, int x, int y, int scale, ParticleImage pimg){
    
    Rect rect = {.x = x, .y = y, .w = pimg.width, .h = pimg.height};
    rect = CorrectRect(rect, cs->width_p, cs->height_p);
    rect.x = 0;
    rect.y = 0;
    int oldx = x, oldy = y;
    int subx = 0, suby = 0;
    if(x < 0){
        subx = abs(x);
        x = 0;
    }
    if(y < 0){
        suby = abs(y);
        y = 0;
    }

    for(int i = rect.y + suby; i < rect.h; i++)
    for(int j = rect.x + subx; j < rect.w; j++){
        if(pimg.types[i*pimg.width + j] == EMPTY) continue;
        CreateMagicParticleCS(cs, j+x-subx, i+y-suby, pimg.types[i*pimg.width + j], pimg.colors[i*pimg.width + j], 500);
    }
}