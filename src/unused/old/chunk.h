// #ifndef CHUNK_H
// #define CHUNK_H

// #include "engine/engine_api.h"
// #include "partsim/particle_data.h"
// #include "partsim/simulator.h"


// // TODO: chunk doesnt need chunk_pos
// typedef struct{
//     // "chunk_world_pos" Position of a chunk in the game world and 
//     // "chunk_pos" its position in the array of loaded chunks
//     // This data has to be used for saving
//     Pos chunk_world_pos;
//     Pos chunk_pos;

//     // Core
//     int w, h;  
//     bool active;
//     // Particles
//     LinkedList* dirty_rect_list;
//     ParticleMap pmap;
// } Chunk;


// typedef struct{
//     // "chunk_world_pos" Position of a chunk in the game world and 
//     // "chunk_pos" its position in the array of loaded chunks
//     // This data has to be used for saving
//     Pos chunk_world_pos;
//     Pos chunk_pos;

//     // Core
//     int w, h;  
//     bool active;

//     // Simulation
//     LinkedList *dirty_rect_list;
    
//     // Particle attribute arrays
//     char *state;
//     int *type;
//     uint8_t *xvel;
//     uint8_t *yvel;
//     Color *c;
//     uint16_t *effect_t;
//     uint16_t *life_t;
//     int32_t *heat;
//     uint8_t *iflags;
//     uint32_t *durability;
//     void **custom;
// } Chunk2;

// #define CHUNK_GET_PARTICLE(c, x, y) (c)->pmap.map[y][x]
// #define CHUNK_GET_PARTICLE_REF(c, x, y) &(c)->pmap.map[y][x]


// // Chunk functions
// int CreateChunk(Chunk* chunk, int w, int h, Pos worldPos);
// void DeleteChunk(Chunk* chunk);
// void ClearChunk(Chunk* chunk);

// void ChunkFillWithAir(Chunk* chunk);
// void ActivateChunk(Chunk* chunk);
// void SimulateChunk(Chunk* c);

// void ChunkCreateParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y);
// void ChunkCreateReplaceParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y);
// void ChunkReplaceParticle(Chunk* chunk, int sx, int sy, int dx, int dy);
// void ChunkDeleteParticle(Chunk* chunk, int chunk_x, int chunk_y);
// void ChunkSwapParticles(Chunk* chunk, Particle* p1, Particle* p2);
// void ChunkCreateManyParticlesCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad);
// void ChunkCreateManyParticlesWithRarityCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad, int rarity);
// void ChunkDeleteManyParticlesCircle(Chunk* chunk, int chunk_x, int chunk_y, int rad);
// void ChunkWallBox(Chunk* chunk);
// void ChunkExplosion(Chunk* chunk, int chunk_x, int chunk_y, int rad, int power, int replaceWith);
// void ChunkDestructionLine(Chunk* chunk, int cx0, int cy0, int cx1, int cy1, int power, int replaceWith);


// #endif //CHUNK_H