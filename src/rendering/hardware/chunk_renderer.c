#include "chunk_renderer.h"


// Rendering with SDL_Renderer
    
void ChunkRender(Window* window, Chunk* chunk, int x, int y){
    // Surface* chunkImage = ChunkToSurface(chunk);
    // Texture* chunkTexture = SDL_CreateTextureFromSurface(window->renderer, chunkImage);
        
    
    // SDL_Rect srcRect = {0, 0, chunk->w, chunk->h};
    // SDL_Rect dstRect = {x, y, chunk->w*reqParticleSize, chunk->h*reqParticleSize};
    // SDL_RenderCopy(window->renderer, chunkTexture, &srcRect, &dstRect);
    
    // clock_t start = GetTimeNano();
    // SDL_DestroyTexture(chunkTexture);
    // clock_t end = GetTimeNano();
    // printf("%d\n", (end-start)/1000);
    // SDL_FreeSurface(chunkImage);
}



void RegionRender(Window* window, Region* region, int x, int y){
    
    for(int i = 0; i < region->h; i++)
    for(int j = 0; j < region->w; j++){
        // ChunkRender(
        //     window, &region->chunks[i*region->w + j], 
        //     x + j*reqChunkWidth, y + i*reqChunkHeight
        // );
    }
}

//#############################################