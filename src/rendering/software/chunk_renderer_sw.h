#ifndef CHUNK_RENDERER_SOFTWARE_H
#define CHUNK_RENDERER_SOFTWARE_H

#include "engine_lib.h"
#include "chunk.h"
#include "region.h"
#include "chunk_space.h"

//System
int StartChunkRendererSW(int chunk_w, int chunk_h, int partSize);
int ChangeChunkRenderSettingsSW(int chunk_w, int chunk_h, int partSize);
void EndChunkRendererSW();

// Common functions
Surface* ChunkToSurface(Chunk* chunk);
Surface* ImageToSurfaceRGBA(Image* img);

// Software rendering
void ShowChunkSpaceDirtyRectsSW(Window* window, ChunkSpace* cs, int x, int y);
void ShowChunkSpaceSW(Window* window, ChunkSpace* cs, int x, int y);
void ShowChunkSpaceAllSW(Window* window, ChunkSpace* cs, int x, int y);
void DrawChunkSW(Window* window, Chunk* chunk, int x, int y);
void DrawRegionSW(Window* window, Region* region, int x, int y);
void DrawChunkSpaceSW(Window* window, ChunkSpace* cs, int x, int y);

// Image software rendering
// void DrawImageInWindowSW(Window* window, Image* img, int x, int y, int scale);

#endif