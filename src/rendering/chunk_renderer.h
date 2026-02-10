#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H

#include "../engine/engine_lib.h"
#include "../structs/chunk.h"
#include "../structs/region.h"
#include "../structs/chunk_space.h"

//System
int StartRenderer(int chunk_w, int chunk_h, int partSize);
int ChangeChunkSettings(int chunk_w, int chunk_h, int partSize);
void EndRenderer();


// Common functions
Surface* ChunkToSurface(Chunk* chunk);
Surface* ImageToSurfaceRGBA(Image* img);

// Rendering with SDL_Renderer
void ChunkRender(Window* window, Chunk* chunk, int x, int y);
void RegionRender(Window* window, Region* region, int x, int y);
void RenderFrame(Window* window);


// Software rendering
void ShowChunk(Window* window, Chunk* chunk, int x, int y);
void ChunkDraw(Window* window, Chunk* chunk, int x, int y);
void RegionDraw(Window* window, Region* region, int x, int y);
void DrawImageInWindowSW(Window* window, Image* img, int x, int y, int scale);
void DrawChunkSpace(Window* window, ChunkSpace* cs, int x, int y);

// Image software rendering
void DrawImageInWindowSW(Window* window, Image* img, int x, int y, int scale);

#endif