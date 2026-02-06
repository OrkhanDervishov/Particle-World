#include "cs_ops.h"



// TODO: Clever rect creation and deletion, using individual chunks


void CreateParticleCS(ChunkSpace *cs, int x, int y, int type){
    if(CS_GET_CUSTOM(cs, x, y) != NULL){
        free(CS_GET_CUSTOM(cs, x, y));
    }
    CS_GET_STATE(cs, x, y) = P_FRESH;
    CS_GET_TYPE(cs, x, y) = type;
    CS_GET_COLOR(cs, x, y) = typeColorList[WALL][0];
}

void DeleteParticleCS(ChunkSpace *cs, int x, int y){
    CS_GET_STATE(cs, x, y) = P_IGNORED;
    CS_GET_TYPE(cs, x, y) = AIR;
    CS_GET_COLOR(cs, x, y).a = 0;
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
    
}


//======================================================

void CreateParticlesRectCS(ChunkSpace *cs, int startX, int startY, int width, int height, int type){
    Rect r = {.x = startX, .y = startY, .w = width, .h = height};
    // CorrectRect(r, cs->width, cs->height);
    startX = r.x;
    startY = r.y;

    int endX = startX + r.w;
    int endY = startY + r.h;
    
    for(int i = startY; i < endY; i++)
    for(int j = startX; j < endX; j++){
        CS_GET_STATE(cs, j, i) = P_FRESH;
        CS_GET_TYPE(cs, j, i) = type;
        CS_GET_COLOR(cs, j, i) = typeColorList[type][0];
        // TODO: Init particle data.
    }
}

void CreateParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad, int type){
    int starty = cY - rad;
    int startx = cX - rad;
    int endy = cY + rad;
    int endx = cX + rad;
    int srad = rad * rad;

    for(int i = starty; i < endy; i++){
        int di = abs(cY - i);
        for(int j = startx; j < endx; j++){
            int dj = abs(cX - j);
            if(di*di + dj*dj < srad){
                if(j >= 0 && i >= 0 && j < cs->width_p && i < cs->height_p){
                    // if(sim->pMap[i][j].id < 0)
                    CS_GET_STATE(cs, j, i) = P_FRESH;
                    CS_GET_TYPE(cs, j, i) = type;
                    CS_GET_COLOR(cs, j, i) = typeColorList[type][0];
                }
            }
        }
    }

}

void DeleteParticlesRectCS(ChunkSpace *cs, int startX, int startY, int width, int height){
    int endX = startX + width;
    int endY = startY + height;
    
    for(int i = startY; i < endY; i++)
    for(int j = startX; j < endX; j++){
        CS_GET_STATE(cs, j, i) = P_IGNORED;
        CS_GET_TYPE(cs, j, i) = AIR;
    }    
}

void DeleteParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad){
    
}

//======================================================

void WallBoxCS(ChunkSpace *cs){

    for(int j = 0; j < cs->width_p; j++){
        CreateParticleCS(cs, j, 0, WALL);
        CreateParticleCS(cs, j, cs->height_p-1, WALL);
    }

    for(int i = 1; i < cs->height_p - 1; i++){
        CreateParticleCS(cs, 0, i, WALL);
        CreateParticleCS(cs, cs->width_p-1, i, WALL);
    }   
}


//======================================================

// void SimulateChunkSpace(ChunkSpace *cs){
//     for(int i = 1; i < cs->height_c-1; i++)
//     for(int j = 1; j < cs->width_c-1; j++){
//         SimulateChunk(&CS_GET_CHUNK(cs, j, i));
//     }
// }