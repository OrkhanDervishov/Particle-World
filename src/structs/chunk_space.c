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

    if(cs->regions == NULL) return 1;

    cs->region_width = region_width > 0 ? region_width : DEFAULT_REGION_WIDTH;
    cs->region_height = region_height > 0 ? region_height : DEFAULT_REGION_HEIGHT;
    
    // Chunks
    cs->width_c = cs->width_r*cs->region_width;
    cs->height_c = cs->height_r*cs->region_height;
    cs->size_c = cs->size_r*cs->region_width*cs->region_height;
    cs->chunks = (Chunk*)malloc(cs->size_c*sizeof(Chunk));

    if(cs->chunks == NULL) return 2;

    cs->chunk_width = chunk_width > 0 ? chunk_width : DEFAULT_CHUNK_SIZE;
    cs->chunk_height = chunk_height > 0 ? chunk_height : DEFAULT_CHUNK_SIZE;

    // cs->chunk_width_mo = cs->chunk_width - 1;
    // cs->chunk_height_mo = cs->chunk_height - 1;
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

    
    Pos wp = {0,0};
    for(int i = 0; i < cs->size_r; i++){
        CreateRegion(&cs->regions[i], cs->region_width, cs->region_height, wp);
        FillRegion(&cs->regions[i]);   
    }

    return 0;
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


void WallBox(ChunkSpace *cs, int chunkOffset){
    
    for(int i = chunkOffset*cs->chunk_height; 
        i < (cs->height_c-chunkOffset)*cs->chunk_height;
        i++
    ){
        CS_GET_TYPE(cs, chunkOffset*cs->chunk_width, i) = WALL;
        CS_GET_TYPE(cs, (cs->width_c-chunkOffset)*cs->chunk_width, i) = WALL;
    }

    for(int j = chunkOffset*cs->chunk_width; 
        j < (cs->width_c-chunkOffset)*cs->chunk_width;
        j++
    ){
        CS_GET_TYPE(cs, j, chunkOffset*cs->chunk_height) = WALL;
        CS_GET_TYPE(cs, j, (cs->height_c-chunkOffset)*cs->chunk_height) = WALL;
    }




    // for(int  i = chunkOffset; i < cs->height_c-chunkOffset; i++){
    //     for(int k = 0; k < cs->chunk_height; k++){
    //         CHUNK_GET_TYPE(cs->chunks[i*cs->width_c + chunkOffset], 0, k) = WALL;   
    //         CHUNK_GET_TYPE(cs->chunks[i*cs->width_c + cs->width_c - chunkOffset], cs->chunk_width-1, k) = WALL;   
    //     }
    // }

    // for(int  j = chunkOffset; j < cs->width_c-chunkOffset; j++){
    //     for(int k = 0; k < cs->chunk_width; k++){
    //         CHUNK_GET_TYPE(cs->chunks[chunkOffset*cs->width_c + j], k, 0) = WALL;   
    //         CHUNK_GET_TYPE(cs->chunks[(cs->width_c - chunkOffset)*cs->width_c + j], k, cs->chunk_height-1) = WALL;   
    //     }
    // }
}