#ifndef CHUNK_SPACE_H
#define CHUNK_SPACE_H

#include "chunk.h"
#include "region.h"

#define DEFAULT_CS_WIDTH 3
#define DEFAULT_CS_HEIGHT 2

typedef struct{
    // Sizes with regions
    size_t width_r;
    size_t height_r;
    size_t size_r;
    // Sizes with chunks
    size_t width_c;
    size_t height_c;
    size_t size_c;
    // Region params
    size_t region_width;
    size_t region_height;
    // Chunk params
    size_t chunk_width;
    size_t chunk_height;

    // Arrays
    Region *regions;
    Chunk *chunks;
} ChunkSpace;

#define CS_GET(cs, x, y, param)     CHUNK_GET2((cs)->chunks[((y)&(cs)->chunk_height)*(cs)->width_c + ((x)&(cs)->chunk_width)], (param), (x), (y))

int CreateChunkSpace(ChunkSpace* cs, size_t w, size_t h, size_t region_width, size_t region_height);
void DeleteChunkSpace(ChunkSpace* cs);

void ArrangeChunks(ChunkSpace *cs);
void SimulateChunkSpace(ChunkSpace* cs);

void ColorChunkSpace(ChunkSpace* cs);

#endif