#ifndef CHUNK_SPACE_H
#define CHUNK_SPACE_H

#include "chunk.h"
#include "region.h"

#define DEFAULT_CS_WIDTH 2
#define DEFAULT_CS_HEIGHT 1

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
// Used when you iterate through chunks.
#define CS_GET_ARRAY_CHUNK(cs_p, x, y)      (cs_p)->chunks[(y)*(cs_p)->width_c + (x)]

// Get chunk via chunk space coordinates
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

// Getters
static inline part_state_t GetStateCS(ChunkSpace* cs, int x, int y){
    return CS_GET_STATE(cs, x, y);
}

static inline part_type_t GetTypeCS(ChunkSpace* cs, int x, int y){
    return CS_GET_TYPE(cs, x, y);
}

static inline part_xvel_t GetXVelCS(ChunkSpace* cs, int x, int y){
    return CS_GET_XVEL(cs, x, y);
}

static inline part_yvel_t GetYVelCS(ChunkSpace* cs, int x, int y){
    return CS_GET_YVEL(cs, x, y);
}

static inline part_color_t GetColorCS(ChunkSpace* cs, int x, int y){
    return CS_GET_COLOR(cs, x, y);
}

static inline part_effectt_t GetEffectTimeCS(ChunkSpace* cs, int x, int y){
    return CS_GET_EFFECT_T(cs, x, y);
}

static inline part_lifet_t GetLifeTimeCS(ChunkSpace* cs, int x, int y){
    return CS_GET_LIFE_T(cs, x, y);
}

static inline part_heat_t GetHeatCS(ChunkSpace* cs, int x, int y){
    return CS_GET_HEAT(cs, x, y);
}

static inline part_pflags_t GetParticleFlagsCS(ChunkSpace* cs, int x, int y){
    return CS_GET_PFLAGS(cs, x, y);
}

static inline part_durab_t GetDurabilityCS(ChunkSpace* cs, int x, int y){
    return CS_GET_DURAB(cs, x, y);
}

static inline part_custom_t GetCustomCS(ChunkSpace* cs, int x, int y){
    return CS_GET_CUSTOM(cs, x, y);
}



// Setters
static inline void SetStateCS(ChunkSpace* cs, int x, int y, part_state_t state){
    CS_GET_STATE(cs, x, y) = state;
}

static inline void SetTypeCS(ChunkSpace* cs, int x, int y, part_type_t type){
    CS_GET_TYPE(cs, x, y) = type;
}

static inline void etXVelCS(ChunkSpace* cs, int x, int y, part_xvel_t xvel){
    CS_GET_XVEL(cs, x, y) = xvel;
}

static inline void SetYVelCS(ChunkSpace* cs, int x, int y, part_yvel_t yvel){
    CS_GET_YVEL(cs, x, y) = yvel;
}

static inline void SetColorCS(ChunkSpace* cs, int x, int y, part_color_t color){
    CS_GET_COLOR(cs, x, y) = color;
}

static inline void SetEffectTimeCS(ChunkSpace* cs, int x, int y, part_effectt_t et){
    CS_GET_EFFECT_T(cs, x, y) = et;
}

static inline void SetLifeTimeCS(ChunkSpace* cs, int x, int y, part_lifet_t lt){
    CS_GET_LIFE_T(cs, x, y) = lt;
}

static inline void SetHeatCS(ChunkSpace* cs, int x, int y, part_heat_t heat){
    CS_GET_HEAT(cs, x, y) = heat;
}

static inline void SetParticleFlagsCS(ChunkSpace* cs, int x, int y, part_pflags_t pf){
    CS_GET_PFLAGS(cs, x, y) = pf;
}

static inline void SetDurabilityCS(ChunkSpace* cs, int x, int y, part_durab_t durab){
    CS_GET_DURAB(cs, x, y) = durab;
}

static inline void SetCustomCS(ChunkSpace* cs, int x, int y, part_custom_t value){
    CS_GET_CUSTOM(cs, x, y) = value;
}


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