#ifndef WORLD_H
#define WORLD_H

#include "engine/engine_api.h"
#include "partsim/particle_data.h"
#include "chunk.h"
#include "partsim/simulator.h"

// #define WORLD_GET_PARTICLE(w, wx, wy) &(w)->chunks[((wy) >> 8) - (w)->chunks[0][0].chunk_pos.y][((wx) >> 8) - (w)->chunks[0][0].chunk_pos.x].pmap.map[(wy) & ((w)->CHUNK_SIZE - 1)][(wx) & ((w)->CHUNK_SIZE - 1)]
#define MAX_CHUNK_LOAD_DISTANCE 5
#define MAX_WORLD_ROWS (2*MAX_CHUNK_LOAD_DISTANCE + 1)
#define MAX_WORLD_COLS (2*MAX_CHUNK_LOAD_DISTANCE + 1)
#define CHUNK_START(w) (MAX_CHUNK_LOAD_DISTANCE - (w)->chunk_load_distance - 1)
#define CHUNK_END(w) (MAX_CHUNK_LOAD_DISTANCE + (w)->chunk_load_distance + 1)

typedef struct{
    Chunk** chunks;
    uint16_t chunk_load_distance;
    uint16_t chunk_simulation_distance;
    uint32_t CHUNK_SIZE;
    uint32_t PARTICLE_SIZE;
    long seed;
} World;


int CreateWorld(World** world);
int DeleteWorld(World** world);
void ClearWorld(World* world);
void SetChunkLoadDistance(World* world, int distance);

void LoadChunks(World* world);
void SimulateWorld(World* world);

#endif //WORLD_H