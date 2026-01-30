#include "cs_ops.h"


// TODO: Clever rect creation and deletion, using individual chunks


void CreateParticleCS(ChunkSpace *cs, int x, int y, int type){
    if(CS_GET_CUSTOM(cs, x, y) != NULL){
        free(CS_GET_CUSTOM(cs, x, y));
    }
    CS_GET_STATE(cs, x, y) = P_FRESH;
    CS_GET_TYPE(cs, x, y) = type;
}

void DeleteParticleCS(ChunkSpace *cs, int x, int y){
    CS_GET_STATE(cs, x, y) = P_IGNORED;
    CS_GET_TYPE(cs, x, y) = AIR;
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


    // printf("x0:%d y0:%d\n", x0, y0);
    // printf("x1:%d y1:%d\n", x1, y1);

    // printf("cx0:%d cy0:%d\n", x0&(cs->chunk_width-1), y0&(cs->chunk_height-1));
    // printf("cx1:%d cy1:%d\n", x1&(cs->chunk_width-1), y1&(cs->chunk_height-1));

    // printf("c0.w:%d\n", c0.w);
    // printf("c1.w:%d\n", c1.w);

    int i0 = (y0&(cs->chunk_height-1)) * c0.w + (x0&(cs->chunk_width-1));
    int i1 = (y1&(cs->chunk_height-1)) * c1.w + (x1&(cs->chunk_width-1));
    // printf("i0:%d i1:%d\n", i0, i1);

    // printf("works1000\n");

    int temp;
    // printf("swap: %d %d\n", CHUNK_GETI_TYPE(c0, i0), CHUNK_GETI_TYPE(c1, i1));
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
    int endX = startX + width;
    int endY = startY + height;
    
    for(int i = startY; i < endY; i++)
    for(int j = startX; j < endX; j++){
        CS_GET_STATE(cs, j, i) = P_FRESH;
        CS_GET_TYPE(cs, j, i) = type;
        ChangeColor(&CS_GET_COLOR(cs, j, i), typeColorList[type]);
        // TODO: Init particle data.
    }
}

void CreateParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad, int type){
    
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

// void SimulateChunkSpace(ChunkSpace *cs){
//     for(int i = 1; i < cs->height_c-1; i++)
//     for(int j = 1; j < cs->width_c-1; j++){
//         SimulateChunk(&CS_GET_CHUNK(cs, j, i));
//     }
// }