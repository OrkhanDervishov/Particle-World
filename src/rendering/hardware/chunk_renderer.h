#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H

#include "engine_lib.h"
#include "chunk.h"
#include "region.h"
#include "chunk_space.h"

//System
int StartRenderer(int chunk_w, int chunk_h, int partSize);
int ChangeChunkSettings(int chunk_w, int chunk_h, int partSize);
void EndRenderer();

// Rendering with SDL_Renderer
void ChunkRender(Window* window, Chunk* chunk, int x, int y);
void RegionRender(Window* window, Region* region, int x, int y);
void RenderFrame(Window* window);

#endif