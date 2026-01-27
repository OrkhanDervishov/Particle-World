#include "region.h"


// This function does not allocate space for Region object itself
int CreateRegion(Region* region, size_t w, size_t h, Pos worldPos){
    region->w = w;
    region->h = h;
    region->region_world_pos = worldPos;

    region->chunks = (Chunk*)malloc(sizeof(Chunk)*region->w*region->h);
    if(region->chunks == NULL){
        return 1;
    }

    return 0;
}


void DeleteRegion(Region* region){
    free(region->chunks);
}


int FillRegion(Region* region){
    if(region->chunks == NULL){
        return 1;
    }

    Pos pos = {0,0};
    size_t size = region->w*region->h;
    for(size_t i = 0; i < size; i++){
        CreateChunk(&region->chunks[i], DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, pos);
    }

    return 0;
}

int ClearRegion(Region* region){
    if(region->chunks == NULL){
        return 1;
    }

    size_t size = region->w*region->h;
    for(size_t i = 0; i < size; i++){
        ClearChunk(&region->chunks[i]);
        ChunkFillWithAir(&region->chunks[i]);
    }

    return 0;
}


void ActivateRegion(Region* region){
    region->active = region->active == TRUE ? FALSE : TRUE;
}


void SimulateRegion(Region* region){
    
    size_t size = region->w*region->h;
    for(size_t i = 0; i < size; i++){
        SimulateChunk(&region->chunks[i]);
    }
}
