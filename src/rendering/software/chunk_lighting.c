#include "chunk_lighting.h"


static Image chunk_lightmap;
static FormatImage chunk_lightmap_f;

static int reqChunkWidth;
static int reqChunkHeight;
static int reqParticleSize;

void init_lightmap_image(int chunk_w, int chunk_h, int partSize){
    reqChunkWidth = chunk_w;
    reqChunkHeight = chunk_h;
    reqParticleSize = partSize;
    create_image(&chunk_lightmap, chunk_w, chunk_h);
    create_fimage(&chunk_lightmap_f, chunk_w, chunk_h, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

void free_lightmap_image(){
    delete_fimage(&chunk_lightmap_f);
    delete_image(&chunk_lightmap);
}


int start_particle_lighting_sw(int chunk_w, int chunk_h, int partSize){
    init_lightmap_image(chunk_w, chunk_h, partSize);
    return 0;
}

int change_particle_lighting_sw(int chunk_w, int chunk_h, int partSize){
    init_lightmap_image(chunk_w, chunk_h, partSize);
    return 0;
}

void end_particle_lighting_sw(){
    free_lightmap_image();
}

void chunk_to_lightmap(Chunk* chunk){
    if(chunk == NULL){
        return;
    }
    for(int i = 0; i < chunk->size; i++){
        part_type_t type = CHUNK_GETI_TYPE(*chunk, i);
        if( type == LAVA || type == FIRE || type == FIRE_LIQUID || 
            type == FIRE_SMOKE || type == ACID || type == PHANTOM || 
            type == FUNGUS || CHECK_FLAG(CHUNK_GETI_PFLAGS(*chunk, i), BURNING))
            chunk_lightmap.buffer[i] = CHUNK_GETI_COLOR(*chunk, i);
        else
            chunk_lightmap.buffer[i] = (Color){0, 0, 0, 255};
    }
    return;
}

void draw_chunk_lightmap(Image lightmap, Chunk* chunk, int x, int y){
    chunk_to_lightmap(chunk);
    draw_image_on_image(lightmap, chunk_lightmap, x, y);
} 

void draw_cs_lightmap(Image lightmap, ChunkSpace* cs, int x, int y){
    for(int i = 0; i < (int)cs->height_c; i++){
    for(int j = 0; j < (int)cs->width_c; j++){
            draw_chunk_lightmap(
                lightmap, &cs->chunks[i*cs->width_c + j], 
                x + j*reqChunkWidth,//*reqParticleSize, 
                y + i*reqChunkHeight//*reqParticleSize
            );
        }
    }
}

void blur_lightmap(Image dest, int times){
    if(times == 0) return;
    gaussian_blur_3x3(dest, chunk_lightmap);
    for(int i = 1; i < times; i++){
        gaussian_blur_3x3(dest, dest);
    }
}

void blur_lightmap2(Image* dest, Image light_map, int times){
    if(times == 0) return;
    Image dest2;
    dest2.buffer = NULL;
    create_similar(&dest2, *dest);
    bool must_copy = FALSE;

    gaussian_blur_3x3(*dest, light_map);
    for(int i = 1; i < times; i++){
        // gaussian_blur_3x3(dest, dest);
        if(!must_copy){
            gaussian_blur_3x3(dest2, *dest);
            must_copy = TRUE;
        }
        else{
            gaussian_blur_3x3(*dest, dest2);
            must_copy = FALSE;
        }
    }

    if(must_copy){
        delete_image(dest);
        (*dest).buffer = dest2.buffer;
        return;
    }
    delete_image(&dest2);
}

void blur_lightmap_strong(Image* dest, Image light_map, int kernel_size, int times){
    if(times == 0) return;
    Image dest2;
    dest2.buffer = NULL;
    create_similar(&dest2, *dest);

    gaussian_blur_horizontal(*dest, light_map, kernel_size);
    gaussian_blur_vertical(dest2, *dest, kernel_size);
    for(int i = 1; i < times; i++){
        gaussian_blur_horizontal(*dest, dest2, kernel_size);
        gaussian_blur_vertical(dest2, *dest, kernel_size);
    }

    copy_image_content(*dest, dest2);
    delete_image(&dest2);
}