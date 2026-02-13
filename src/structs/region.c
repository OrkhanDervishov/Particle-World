#include "region.h"

// This function does not allocate space for Region object itself
int CreateRegion(Region* region, size_t region_w, size_t region_h, size_t chunk_w, size_t chunk_h){
    region->w = region_w;
    region->h = region_h;
    region->chunk_width = chunk_w;
    region->chunk_height = chunk_h;

    region->chunks = (Chunk*)malloc(sizeof(Chunk)*region->w*region->h);
    if(region->chunks == NULL){
        return 1;
    }
    size_t size = region->w*region->h;
    for(size_t i = 0; i < size; i++){
        CreateChunk(&region->chunks[i], region->chunk_width, region->chunk_height);
    }


    return 0;
}


void DeleteRegion(Region* region){
    if(region->chunks == NULL) return;
    free(region->chunks);
}


// int FillRegion(Region* region){
//     if(region->chunks == NULL){
//         return 1;
//     }

//     size_t size = region->w*region->h;
//     for(size_t i = 0; i < size; i++){
//         CreateChunk(&region->chunks[i], region->chunk_width, region->chunk_height);
//     }

//     return 0;
// }

int ClearRegion(Region* region){
    if(region->chunks == NULL){
        return 1;
    }

    size_t size = region->w*region->h;
    for(size_t i = 0; i < size; i++){
        ClearChunk(&region->chunks[i]);
        // ChunkFillWithAir(&region->chunks[i]);
        // ChunkWallBox(&region->chunks[i]);
    }

    return 0;
}


void ActivateRegion(Region* region){
    region->active = region->active == TRUE ? FALSE : TRUE;
}

void ColorRegion(Region* region, Color color){
    for(int i = 0; i < region->h*region->w; i++){
        ColorChunk(&region->chunks[i], color);
    }
}