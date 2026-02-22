#include "chunk_space.h"

int CreateChunkSpace
(
    ChunkSpace* cs, size_t w, size_t h, 
    size_t region_width, size_t region_height, 
    size_t chunk_width, size_t chunk_height
)
{

    //------------------------------
    // Regions
    cs->width_r = w > 0 ? w : DEFAULT_CS_WIDTH;
    cs->height_r = h > 0 ? h : DEFAULT_CS_HEIGHT;
    cs->size_r = cs->width_r*cs->height_r;
    cs->regions = (Region*)malloc(cs->size_r*sizeof(Region));
    if(cs->regions == NULL) return 1;
    //------------------------------
    cs->region_width = region_width > 0 ? region_width : DEFAULT_REGION_WIDTH;
    cs->region_height = region_height > 0 ? region_height : DEFAULT_REGION_HEIGHT;
    //------------------------------

    
    // Chunks
    //------------------------------
    cs->width_c = cs->width_r*cs->region_width;
    cs->height_c = cs->height_r*cs->region_height;
    cs->size_c = cs->size_r*cs->region_width*cs->region_height;
    cs->chunks = (Chunk*)malloc(cs->size_c*sizeof(Chunk));
    if(cs->chunks == NULL) return 2;
    //------------------------------
    cs->chunk_width = chunk_width > 0 ? chunk_width : DEFAULT_CHUNK_SIZE;
    cs->chunk_height = chunk_height > 0 ? chunk_height : DEFAULT_CHUNK_SIZE;
    //------------------------------

    // Complete size
    //------------------------------
    cs->width_p = cs->chunk_width * cs->width_c;
    cs->height_p = cs->chunk_height * cs->height_c;
    cs->size_p = cs->width_p * cs->height_p;
    //------------------------------

    //------------------------------
    // cs->chunk_width_mo = cs->chunk_width - 1;
    // cs->chunk_height_mo = cs->chunk_height - 1;
    //------------------------------
    
    
    //------------------------------
    cs->width_power = 0;
    cs->height_power = 0;

    size_t cwidth = cs->chunk_width;
    while(cwidth != 1){
        cwidth /= 2;
        cs->width_power++;
    }

    size_t cheight = cs->chunk_height;
    while(cheight != 1){
        cheight /= 2;
        cs->height_power++;
    }
    //------------------------------

    cs->simStartX = 1;
    cs->simEndX = cs->width_c - 1;
    cs->simStartY = 1;
    cs->simEndY = cs->height_c - 1;
    
    for(size_t i = 0; i < cs->size_r; i++){
        CreateRegion(&cs->regions[i], cs->region_width, cs->region_height, cs->chunk_width, cs->chunk_height); 
    }

    return 0;
}

void DeleteChunkSpace(ChunkSpace* cs){
    for(size_t i = 0; i < cs->size_r; i++){
        DeleteRegion(&cs->regions[i]);
    }
}

void ArrangeChunks(ChunkSpace *cs){

    for(size_t i = 0; i < cs->height_c; i++)
    for(size_t j = 0; j < cs->width_c; j++){
        int rx = j / cs->region_width;
        int ry = i / cs->region_height;

        int rcx = j % cs->region_width;
        int rcy = i % cs->region_height;

        int r_coord = ry*cs->width_r + rx;
        int rc_coord = rcy*cs->region_width + rcx;

        cs->chunks[i*cs->width_c + j] = cs->regions[r_coord].chunks[rc_coord];
    }
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
    for(size_t i = 0; i < cs->size_r; i++){
        ColorRegion(&cs->regions[i], getRandomColor2());
    }
}


void SetSimEndpointsChunkSpace(ChunkSpace* cs, int startX, int endX, int startY, int endY){
    cs->simStartX = startX;
    cs->simEndX = endX;
    cs->simStartY = startY;
    cs->simEndY = endY;
}