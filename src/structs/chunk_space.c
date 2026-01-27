#include "chunk_space.h"

int CreateChunkSpace
(
    ChunkSpace* cs, size_t w, size_t h, 
    size_t region_width, size_t region_height, 
    size_t chunk_width, size_t chunk_height
)
{

    // Regions
    cs->width_r = w > 0 ? w : DEFAULT_CS_WIDTH;
    cs->height_r = h > 0 ? h : DEFAULT_CS_HEIGHT;
    cs->size_r = cs->width_r*cs->height_r;
    cs->regions = (Region*)malloc(cs->size_r*sizeof(Region));

    cs->region_width = region_width > 0 ? region_width : DEFAULT_REGION_WIDTH;
    cs->region_height = region_height > 0 ? region_height : DEFAULT_REGION_HEIGHT;
    
    // Chunks
    cs->width_c = cs->width_r*cs->region_width;
    cs->height_c = cs->height_r*cs->region_height;
    cs->size_c = cs->size_r*cs->region_width*cs->region_height;
    cs->chunks = (Chunk*)malloc(cs->size_c*sizeof(Chunk));

    cs->chunk_width = chunk_width > 0 ? chunk_width : DEFAULT_CHUNK_SIZE;
    cs->chunk_height = chunk_height > 0 ? chunk_height : DEFAULT_CHUNK_SIZE;

    
    Pos wp = {0,0};
    for(int i = 0; i < cs->size_r; i++){
        CreateRegion(&cs->regions[i], cs->region_width, cs->region_height, wp);
        FillRegion(&cs->regions[i]);   
    }

}

void DeleteChunkSpace(ChunkSpace* cs){
    for(int i = 0; i < cs->size_r; i++){
        DeleteRegion(&cs->regions[i]);
    }
}

void ArrangeChunks(ChunkSpace *cs){

    for(int i = 0; i < cs->height_c; i++)
    for(int j = 0; j < cs->width_c; j++){
        int rx = j / cs->region_width;
        int ry = i / cs->region_height;

        int rcx = j % cs->region_width;
        int rcy = i % cs->region_height;

        int r_coord = ry*cs->width_r + rx;
        int rc_coord = rcy*cs->region_width + rcx;

        cs->chunks[i*cs->width_c + j] = cs->regions[r_coord].chunks[rc_coord];
    }
}

void SimulateChunkSpace(ChunkSpace* cs){
    
}

Color getRandomColor2(){
    Color c = {
        .r = rand() % 256,
        .g = rand() % 256,
        .b = rand() % 256,
        .a = 255
    };

    return c;
}

void ColorChunkSpace(ChunkSpace* cs){
    for(int i = 0; i < cs->size_r; i++){
        ColorRegion(&cs->regions[i], getRandomColor2());
    }
}