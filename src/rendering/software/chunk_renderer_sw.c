#include "chunk_renderer_sw.h"

// System

Image chunk_image;
FormatImage chunk_fimage;

int reqChunkWidth;
int reqChunkHeight;
int reqParticleSize;

void InitChunkImage(int chunk_w, int chunk_h, int partSize){
    reqChunkWidth = chunk_w;
    reqChunkHeight = chunk_h;
    reqParticleSize = partSize;
    create_image(&chunk_image, chunk_w, chunk_h);
    create_fimage(&chunk_fimage, chunk_w, chunk_h, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

void FreeChunkImage(){
    delete_fimage(&chunk_fimage);
    delete_image(&chunk_image);
}


int StartChunkRendererSW(int chunk_w, int chunk_h, int partSize){
    InitChunkImage(chunk_w, chunk_h, partSize);
    return 0;
}

int ChangeChunkSettingsSW(int chunk_w, int chunk_h, int partSize){
    InitChunkImage(chunk_w, chunk_h, partSize);
    return 0;
}

void EndChunkRendererSW(){
    FreeChunkImage();
}

//###########################################

Surface* ChunkToSurface(Chunk* chunk){
    if(chunk == NULL){
        return NULL;
    }
    
    Surface* surface = SDL_CreateRGBSurface(
        0, chunk->w, chunk->h,
        32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    );
    
    // TODO: rewite 2d arrays as 1d
    uint32_t *pixels = (uint32_t *)surface->pixels;
    for(size_t i = 0; i < chunk->size; i++){
        pixels[i] = chunk->c[i].rgba;
    }
    
    return surface;
}

// Turns chunk data into image data and stores it in storeSurface
int ChunkToImage(Chunk* chunk){
    if(chunk == NULL){
        return 1;
    }
    // if(chunk->w != reqChunkWidth || chunk->h != reqChunkHeight){
    //     printf("Chunk sizes differ from required ones\n");
    //     return 2;
    // }
    
    for(size_t i = 0; i < chunk->size; i++){
        chunk_image.buffer[i] = chunk->c[i];
    }

    return 0;
}



Surface* ImageToSurfaceRGBA(Image* img){
    if(img == NULL){
        return NULL;
    }
    
    Surface* surface = SDL_CreateRGBSurface(
        0, img->width, img->height,
        32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    );
    

    uint32_t *pixels = (uint32_t *)surface->pixels;
    for(int i = 0; i < img->height; i++)
    for(int j = 0; j < img->width; j++){
        pixels[i*img->width + j] = img->buffer[i*img->width + j].rgba;
    }
    
    return surface;
}



//###########################################


void ShowChunkDirtyRectsSW(Window* window, Chunk* chunk, int x, int y, Color color){
    Rect* list = chunk->dirty_rect_list;
    
    for(int i = 0; i < chunk->dr_count; i++){
        Rect rect = list[i];
        rect.x = rect.x*DEFAULT_PARTICLE_SIZE + x;
        rect.y = rect.y*DEFAULT_PARTICLE_SIZE + y;
        rect.w *= DEFAULT_PARTICLE_SIZE;
        rect.h *= DEFAULT_PARTICLE_SIZE;
        draw_rect_f(window->context, rect, color, 1);
    }
}

void ShowChunkSpaceDirtyRectsSW(Window* window, ChunkSpace* cs, int x, int y){
    Color color = {.rgba = 0xFFFF00FF};

    for(int i = 0; i < cs->height_c; i++){
        for(int j = 0; j < cs->width_c; j++){
            ShowChunkDirtyRectsSW(
                window, &cs->chunks[i*cs->width_c + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize,
                color
            );
        }
    }
}

void ShowChunkSW(Window* window, Chunk* chunk, int x, int y, Color color){
    Rect rect = {.x = x, .y = y, .w = chunk->w*DEFAULT_PARTICLE_SIZE, .h = chunk->h*DEFAULT_PARTICLE_SIZE};
    draw_rect_f(window->context, rect, color, 1);
    // draw_filled_rect_f(window->context, rect, color);
}

void ShowChunkSpaceSW(Window* window, ChunkSpace* cs, int x, int y){
    
    Color green = {.rgba = 0xFF00FF00};
    Color yellow = {.rgba = 0xFF00FFFF};
    bool row_toggle = FALSE;
    bool col_toggle = FALSE;
    for(int i = 0; i < cs->height_c; i++){
        row_toggle = row_toggle ? FALSE : TRUE;
        col_toggle = row_toggle;
        for(int j = 0; j < cs->width_c; j++){
            col_toggle = col_toggle ? FALSE : TRUE;
            Color color = col_toggle ? green : yellow;
            ShowChunkSW(
                window, &cs->chunks[i*cs->width_c + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize,
                color
            );
        }
    }
}

// void ShowChunkAllSW(Window* window, Chunk* chunk, int x, int y, Color color){

// }

void ShowChunkSpaceAllSW(Window* window, ChunkSpace* cs, int x, int y){
    ShowChunkSpaceDirtyRectsSW(window, cs, x, y);
    ShowChunkSpaceSW(window, cs, x, y);
}

//###########################################


void DrawChunkSW(Window* window, Chunk* chunk, int x, int y){
    ChunkToImage(chunk);
    
    SDL_Rect srcRect = {0, 0, chunk->w, chunk->h};
    SDL_Rect dstRect = {x, y, chunk->w*reqParticleSize, chunk->h*reqParticleSize};
    draw_image_on_fimage_scaled(window->context, chunk_image, x, y, reqParticleSize, reqParticleSize);
}

void DrawRegionSW(Window* window, Region* region, int x, int y){
    for(int i = 0; i < region->h; i++){
    for(int j = 0; j < region->w; j++){
            DrawChunkSW(
                window, &region->chunks[i*region->w + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize
            );
        }
    }
}

void DrawChunkSpaceSW(Window* window, ChunkSpace* cs, int x, int y){
    for(int i = 0; i < cs->height_c; i++){
    for(int j = 0; j < cs->width_c; j++){
            DrawChunkSW(
                window, &cs->chunks[i*cs->width_c + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize
            );
        }
    }
}


//#############################################

// Image software rendering

void DrawImageInWindowSW(Window* window, Image* img, int x, int y, int scale){
    Surface* imageSurface = ImageToSurfaceRGBA(img);
    Surface* winSurface = SDL_GetWindowSurface(window->window);

    SDL_Rect srcRect = {0, 0, img->width, img->height};
    SDL_Rect dstRect = {x, y, img->width*scale, img->height*scale};
    // if(scale != 1)
        // SDL_BlitScaled(storeChunkSurface, &srcRect, winSurface, &dstRect);
    // else
        // SDL_BlitSurface(storeChunkSurface, &srcRect, winSurface, &dstRect);

    SDL_FreeSurface(imageSurface);
}
