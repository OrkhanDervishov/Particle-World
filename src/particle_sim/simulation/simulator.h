#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "engine_lib.h"
#include "dirtyrect.h"
#include "particle_data.h"
#include "chunk_space.h"

extern int PART_SIDE;
extern int DELAY;
extern int RADIUS;
extern int CSIDE;

// typedef struct
// {
//     int pSide;
//     int rows;
//     int cols;
//     int partCount;
//     Particle** pMap;
// } ParticleSimulator;


// ???
#define COPY_PART(s, sx, sy, dx, dy) sim->pMap[sy][sx] = sim->pMap[dy][dx]


// Simulate
Rect SimulateChunkSpaceRect(ChunkSpace* cs, Chunk* chunk, Rect rect, int cs_x, int cs_y);
Rect SimulateRect(Chunk* chunk, Rect rect, int cs_x, int cs_y);
// void SimulateRects(Chunk* chunk, int cs_x, int cs_y);
void SimulateRects(ChunkSpace* cs, Chunk* chunk, int cs_x, int cs_y);

void SimulateChunkSpaceDR(ChunkSpace *cs);
void SimulateChunkSpace(ChunkSpace *cs);
void RefreshChunkSpace(ChunkSpace* cs);

// TODO: Create new file for heat simulation
// void SimulateHeatMap(ParticleSimulator* sim);

#endif