#include "chunk_renderer.h"


// System

Surface* storeChunkSurface;
int reqChunkWidth;
int reqChunkHeight;
int reqParticleSize;

void InitStoreSurface(int chunk_w, int chunk_h, int partSize){
    if(storeChunkSurface != NULL){
        SDL_FreeSurface(storeChunkSurface);
    }
    reqChunkWidth = chunk_w;
    reqChunkHeight = chunk_h;
    reqParticleSize = partSize;
    storeChunkSurface = SDL_CreateRGBSurface(
        0, chunk_w, chunk_h,
        32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    );
}

void FreeStoreSurface(){
    if(storeChunkSurface == NULL) return;
    SDL_FreeSurface(storeChunkSurface);
}


int StartRenderer(int chunk_w, int chunk_h, int partSize){
    InitStoreSurface(chunk_w, chunk_h, partSize);
    return 0;
}

int ChangeChunkSettings(int chunk_w, int chunk_h, int partSize){
    InitStoreSurface(chunk_w, chunk_h, partSize);
    return 0;
}

void EndRenderer(){
    FreeStoreSurface();
}



//#############################################

// Free the surface after use
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
int ChunkToStoreSurface(Chunk* chunk){
    if(chunk == NULL){
        return 1;
    }
    // if(chunk->w != reqChunkWidth || chunk->h != reqChunkHeight){
    //     printf("Chunk sizes differ from required ones\n");
    //     return 2;
    // }
    
    uint32_t *pixels = (uint32_t *)storeChunkSurface->pixels;
    for(size_t i = 0; i < chunk->size; i++){
        pixels[i] = chunk->c[i].rgba;
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


//#############################################

// Rendering with SDL_Renderer
    
void ChunkRender(Window* window, Chunk* chunk, int x, int y){
    Surface* chunkImage = ChunkToSurface(chunk);
    Texture* chunkTexture = SDL_CreateTextureFromSurface(window->renderer, chunkImage);
        
    
    SDL_Rect srcRect = {0, 0, chunk->w, chunk->h};
    SDL_Rect dstRect = {x, y, chunk->w*reqParticleSize, chunk->h*reqParticleSize};
    SDL_RenderCopy(window->renderer, chunkTexture, &srcRect, &dstRect);
    
    clock_t start = GetTimeNano();
    SDL_DestroyTexture(chunkTexture);
    clock_t end = GetTimeNano();
    printf("%d\n", (end-start)/1000);
    SDL_FreeSurface(chunkImage);
}



void RegionRender(Window* window, Region* region, int x, int y){
    
    for(int i = 0; i < region->h; i++)
    for(int j = 0; j < region->w; j++){
        ChunkRender(
            window, &region->chunks[i*region->w + j], 
            x + j*reqChunkWidth, y + i*reqChunkHeight
        );
    }
}

//#############################################

// Rendering with SDL_Surface

void ChunkDraw(Window* window, Chunk* chunk, int x, int y){
    ChunkToStoreSurface(chunk);
    Surface* winSurface = SDL_GetWindowSurface(window->window);
    
    
    // clock_t start = GetTimeNano();
    SDL_Rect srcRect = {0, 0, chunk->w, chunk->h};
    SDL_Rect dstRect = {x, y, chunk->w*reqParticleSize, chunk->h*reqParticleSize};
    SDL_BlitScaled(storeChunkSurface, &srcRect, winSurface, &dstRect);
    // clock_t end = GetTimeNano();
    // printf("%d\n", (end-start)/1000);

    // SDL_FreeSurface(chunkImage);
}

void RegionDraw(Window* window, Region* region, int x, int y){
    // CONSOLE("########################\n");
    for(int i = 0; i < region->h; i++){
    for(int j = 0; j < region->w; j++){
            ChunkDraw(
                window, &region->chunks[i*region->w + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize
            );
        }
        // CONSOLE("-----------------\n");
    }
}

void DrawChunkSpace(Window* window, ChunkSpace* cs, int x, int y){
    for(int i = 0; i < cs->height_c; i++){
    for(int j = 0; j < cs->width_c; j++){
            ChunkDraw(
                window, &cs->chunks[i*cs->width_c + j], 
                x + j*reqChunkWidth*reqParticleSize, 
                y + i*reqChunkHeight*reqParticleSize
            );
            // printf("works10000");
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
    if(scale != 1)
        SDL_BlitScaled(storeChunkSurface, &srcRect, winSurface, &dstRect);
    else
        SDL_BlitSurface(storeChunkSurface, &srcRect, winSurface, &dstRect);

    SDL_FreeSurface(imageSurface);
}
