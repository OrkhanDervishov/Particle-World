#ifndef CHUNK_H
#define CHUNK_H

#include "engine_lib.h"
#include "particle_data.h"
#include "dirtyrect.h"



typedef struct{
    // "chunk_world_pos" Position of a chunk in the game world and 
    // "chunk_pos" its position in the array of loaded chunks
    // This data has to be used for saving

    // Core
    int  w, h;
    int  size;
    bool    active;

    // Simulation
    int dr_count;
    Rect* dirty_rect_list;
    
    // Chunk particle data
    void*           data_block;
    // Particle attribute arrays
    part_state_t        *state;
    part_type_t         *type;
    part_xvel_t         *xvel;
    part_yvel_t         *yvel;
    part_color_t        *c;
    part_effectt_t      *effect_t;
    part_lifet_t        *life_t;
    part_heat_t         *heat;
    part_pflags_t       *pflags;
    part_durab_t        *durability;
    part_custom_t       *custom;
} Chunk;

// #define CHUNK_GET_PARTICLE(c, x, y)      (c)->pmap.map[y][x]
// #define CHUNK_GET_PARTICLE_REF(c, x, y)  &(c)->pmap.map[y][x]
// #define CHUNK_GET(chunk_param, x, y)        (chunk_param)[(y)*(chunk).w + (x)]
#define CHUNK_GET2(chunk, param, x, y)      (chunk).(param)[(y)*(chunk).w + (x)]

#define CHUNK_GET_STATE(chunk, x, y)        (chunk).state[(y)*(chunk).w + (x)]
#define CHUNK_GET_TYPE(chunk, x, y)         (chunk).type[(y)*(chunk).w + (x)]
#define CHUNK_GET_XVEL(chunk, x, y)         (chunk).xvel[(y)*(chunk).w + (x)]
#define CHUNK_GET_YVEL(chunk, x, y)         (chunk).yvel[(y)*(chunk).w + (x)]
#define CHUNK_GET_COLOR(chunk, x, y)        (chunk).c[(y)*(chunk).w + (x)]
#define CHUNK_GET_EFFECT_T(chunk, x, y)     (chunk).effect_t[(y)*(chunk).w + (x)]
#define CHUNK_GET_LIFE_T(chunk, x, y)       (chunk).life_t[(y)*(chunk).w + (x)]
#define CHUNK_GET_HEAT(chunk, x, y)         (chunk).heat[(y)*(chunk).w + (x)]
#define CHUNK_GET_PFLAGS(chunk, x, y)       (chunk).pflags[(y)*(chunk).w + (x)]
#define CHUNK_GET_DURAB(chunk, x, y)        (chunk).durability[(y)*(chunk).w + (x)]
#define CHUNK_GET_CUSTOM(chunk, x, y)       (chunk).custom[(y)*(chunk).w + (x)]


#define CHUNK_GETI_STATE(chunk, index)        (chunk).state[(index)]
#define CHUNK_GETI_TYPE(chunk, index)         (chunk).type[(index)]
#define CHUNK_GETI_XVEL(chunk, index)         (chunk).xvel[(index)]
#define CHUNK_GETI_YVEL(chunk, index)         (chunk).yvel[(index)]
#define CHUNK_GETI_COLOR(chunk, index)        (chunk).c[(index)]
#define CHUNK_GETI_EFFECT_T(chunk, index)     (chunk).effect_t[(index)]
#define CHUNK_GETI_LIFE_T(chunk, index)       (chunk).life_t[(index)]
#define CHUNK_GETI_HEAT(chunk, index)         (chunk).heat[(index)]
#define CHUNK_GETI_PFLAGS(chunk, index)       (chunk).pflags[(index)]
#define CHUNK_GETI_DURAB(chunk, index)        (chunk).durability[(index)]
#define CHUNK_GETI_CUSTOM(chunk, index)       (chunk).custom[(index)]

// Chunk functions
int CreateChunk(Chunk* chunk, int w, int h);
void DeleteChunk(Chunk* chunk);
void ClearChunk(Chunk* chunk);

void ActivateChunk(Chunk* chunk);

void ChunkWallBox(Chunk* chunk);
void ColorChunk(Chunk* chunk, Color color);

void ClearChunkFull(Chunk* chunk);
void ClearChunkPartly(Chunk* chunk, int x, int y, int w, int h);

// void ChunkCreateParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y);
// void ChunkCreateReplaceParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y);
// void ChunkReplaceParticle(Chunk* chunk, int sx, int sy, int dx, int dy);
// void ChunkDeleteParticle(Chunk* chunk, int chunk_x, int chunk_y);
// void ChunkSwapParticles(Chunk* chunk, Particle* p1, Particle* p2);
// void ChunkCreateManyParticlesCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad);
// void ChunkCreateManyParticlesWithRarityCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad, int rarity);
// void ChunkDeleteManyParticlesCircle(Chunk* chunk, int chunk_x, int chunk_y, int rad);
// void ChunkExplosion(Chunk* chunk, int chunk_x, int chunk_y, int rad, int power, int replaceWith);
// void ChunkDestructionLine(Chunk* chunk, int cx0, int cy0, int cx1, int cy1, int power, int replaceWith);


#endif //CHUNK_H