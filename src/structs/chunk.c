#include "chunk.h"


int CreateChunk(Chunk* chunk, int w, int h, Pos worldPos){

    chunk->w = w;
    chunk->h = h;
    chunk->size = w*h;
    chunk->active = TRUE;
    chunk->chunk_world_pos = worldPos;
    chunk->chunk_pos.x = 0;
    chunk->chunk_pos.y = 0;

    InitDirtyRectList(&chunk->dirty_rect_list);

    
    chunk->data_block = malloc(
        chunk->size*(
        sizeof(STATE_DT) +          // state
        sizeof(TYPE_DT) +           // type
        sizeof(XVEL_DT) +           // xvel
        sizeof(YVEL_DT) +           // yvel
        sizeof(PFLAGS_DT) +         // pflags
        sizeof(COLOR_DT) +          // color
        sizeof(EFFECTT_DT) +        // effect_t
        sizeof(LIFET_DT) +          // life_t
        sizeof(HEAT_DT) +           // heat
        sizeof(DURABILITY_DT) +     // durability
        sizeof(CUSTOM_DT))          // custom
    );


    byte* p = chunk->data_block;            chunk->state = (STATE_DT*)p;
    p += chunk->size*sizeof(STATE_DT);      chunk->type = (TYPE_DT*)p;
    p += chunk->size*sizeof(TYPE_DT);       chunk->xvel = (XVEL_DT*)p;
    p += chunk->size*sizeof(XVEL_DT);       chunk->yvel = (YVEL_DT*)p;
    p += chunk->size*sizeof(YVEL_DT);       chunk->c = (COLOR_DT*)p;
    p += chunk->size*sizeof(COLOR_DT);      chunk->effect_t = (EFFECTT_DT*)p;
    p += chunk->size*sizeof(EFFECTT_DT);    chunk->life_t = (LIFET_DT*)p;
    p += chunk->size*sizeof(LIFET_DT);      chunk->heat = (HEAT_DT*)p;
    p += chunk->size*sizeof(HEAT_DT);       chunk->pflags = (PFLAGS_DT*)p;
    p += chunk->size*sizeof(PFLAGS_DT);     chunk->durability = (DURABILITY_DT*)p;
    p += chunk->size*sizeof(DURABILITY_DT); chunk->custom = (CUSTOM_DT*)p;

    for(int i = 0; i < w*h; i++){
        chunk->type[i] = AIR;
    }
    memset(chunk->state, 0, chunk->size*sizeof(STATE_DT));

    memset(chunk->xvel, 0, chunk->size*sizeof(XVEL_DT));
    memset(chunk->yvel, 0, chunk->size*sizeof(YVEL_DT));
    memset(chunk->c, 0, chunk->size*sizeof(COLOR_DT));
    memset(chunk->effect_t, 0, chunk->size*sizeof(EFFECTT_DT));
    memset(chunk->life_t, 0, chunk->size*sizeof(LIFET_DT));
    memset(chunk->heat, 0, chunk->size*sizeof(HEAT_DT));
    memset(chunk->pflags, 0, chunk->size*sizeof(PFLAGS_DT));
    memset(chunk->durability, 0, chunk->size*sizeof(DURABILITY_DT));
    memset(chunk->custom, 0, chunk->size*sizeof(CUSTOM_DT));

    return 0;
}

void DeleteChunk(Chunk* chunk){
    free(chunk->data_block);
    DeleteList(&chunk->dirty_rect_list);
}

void ClearChunk(Chunk* chunk){
    for(int i = 0; i < chunk->size; i++){
        chunk->type[i] = AIR;
        chunk->state[i] = -1;
    }

    // TODO: 
    // ClearList(chunk->dirty_rect_list);
}

void ActivateChunk(Chunk* chunk){
    chunk->active = TRUE;
}

void SimulateChunk(Chunk* c){
    // printf("%s\n", typeNameList[CHUNK_GET_PARTICLE(c, 100, 32).type]);
    // SimulateRects(&c->pmap, c->dirty_rect_list);
}




void ChunkWallBox(Chunk* chunk){

    for(int j = 0; j < chunk->w; j++){
        chunk->type[j] = WALL;
        ChangeColor(&chunk->c[j], WALL_COLORS);
        chunk->type[(chunk->h-1)*chunk->w + j] = WALL;
        ChangeColor(&chunk->c[(chunk->h-1)*chunk->w + j], WALL_COLORS);
    }
    for(int i = 1; i < chunk->h - 1; i++){
        chunk->type[i*chunk->w] = WALL;
        ChangeColor(&chunk->c[i*chunk->w], WALL_COLORS);
        chunk->type[(i+1)*chunk->w - 1] = WALL;
        ChangeColor(&chunk->c[(i+1)*chunk->w - 1], WALL_COLORS);
    }
}

void ColorChunk(Chunk* chunk, Color color){
    for(int i = 0; i < chunk->size; i++){
        chunk->c[i] = color;
    }
}