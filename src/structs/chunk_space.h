#ifndef CHUNK_SPACE_H
#define CHUNK_SPACE_H

#include "chunk.h"
#include "region.h"

#define DEFAULT_CS_WIDTH 4
#define DEFAULT_CS_HEIGHT 3

#define CS_WIDTH    DEFAULT_CS_WIDTH
#define CS_HEIGHT   DEFAULT_CS_HEIGHT

typedef struct{
    // Sizes with particles
    size_t width_p;
    size_t height_p;
    size_t size_p;
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

    // For getting elements
    uint8_t width_power;
    uint8_t height_power;
    // size_t chunk_width_mo;
    // size_t chunk_height_mo;


    // Arrays
    Region *regions;
    Chunk *chunks;

    // Other params
    int simStartX;
    int simEndX;
    int simStartY;
    int simEndY;
} ChunkSpace;

// Useless (Not working. delete it)
// #define CS_GET_PART(cs, x, y, param)    CHUNK_GET2((cs)->chunks[((y)>>(cs)->chunk_height)*(cs)->width_c + ((x)>>(cs)->chunk_width)], (param), (x)&(cs)->width_c, (y)&(cs)->height_c)



/*
    If you want to get a particle or chunk from loaded space
    you must enter its coordinates according current ChunkSpace's position
    and call local get functions or macros.

    If you want to get them from the whole world space
    you must enter their global coordinate.

    NOTE: World functions are not implemented yet.
          World functions will be implemented in world.c
    TODO: Implement world functions.
*/
//==============================================================
/*
    Macro definitions of ChunkSpace's chunk/particle gathering functions.
*/

#define CS_GET_CHUNK_INDEX(cs_p, x, y)      ((y)>>(cs_p)->height_power)*(cs_p)->width_c + ((x)>>(cs_p)->width_power)
// Used when you use iterate through chunks.
#define CS_GET_ARRAY_CHUNK(cs_p, x, y)      (cs_p)->chunks[(y)*(cs_p)->width_c + (x)]
// Used when you use real world coordinates.
#define CS_GET_CHUNK(cs_p, x, y)            (cs_p)->chunks[CS_GET_CHUNK_INDEX(cs_p, x, y)]
#define CS_GET_PART_X(cs_p, x)              (x)&((cs_p)->chunk_width-1)
#define CS_GET_PART_Y(cs_p, y)              (y)&((cs_p)->chunk_height-1)

#define CS_GET_STATE(cs_p, x, y)      CHUNK_GET_STATE(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_TYPE(cs_p, x, y)       CHUNK_GET_TYPE(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_XVEL(cs_p, x, y)       CHUNK_GET_XVEL(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_YVEL(cs_p, x, y)       CHUNK_GET_YVEL(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_COLOR(cs_p, x, y)      CHUNK_GET_COLOR(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_EFFECT_T(cs_p, x, y)   CHUNK_GET_EFFECT_T(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_LIFE_T(cs_p, x, y)     CHUNK_GET_LIFE_T(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_HEAT(cs_p, x, y)       CHUNK_GET_HEAT(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_PFLAGS(cs_p, x, y)     CHUNK_GET_PFLAGS(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_DURAB(cs_p, x, y)      CHUNK_GET_DURAB(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))
#define CS_GET_CUSTOM(cs_p, x, y)     CHUNK_GET_CUSTOM(CS_GET_CHUNK((cs_p), (x), (y)), CS_GET_PART_X(cs_p, x), CS_GET_PART_Y(cs_p, y))

//==============================================================
/*
    ChunkSpace's chunk/particle gathering functions.
*/

// TODO: Implement functions for local get.

//==============================================================

int CreateChunkSpace(
    ChunkSpace* cs, size_t w, size_t h, 
    size_t region_width, size_t region_height, 
    size_t chunk_width, size_t chunk_height
);
void DeleteChunkSpace(ChunkSpace* cs);

void ArrangeChunks(ChunkSpace *cs);

void ColorChunkSpace(ChunkSpace* cs);

void SetSimEndpointsChunkSpace(ChunkSpace* cs, int startX, int endX, int startY, int endY);

#endif