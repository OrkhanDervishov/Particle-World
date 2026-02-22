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

extern int selectedType;

extern int PART_SIDE;
extern int DELAY;
extern int RADIUS;
extern int CSIDE;

// int InitParticleSimulator(ParticleSimulator** sim, int w, int h, int ps);
// void DestroyParticleSimulator(ParticleSimulator** sim);
// void ChangeParticleSimulator(ParticleSimulator** sim, int w, int h, int ps);

void ClearMap(ParticleMap* pmap);

// Simulate
void SimulateAll(ParticleMap* pmap);
Rect SimulateRect(ParticleMap* pmap, Rect rect);
void SimulateRects(Chunk* chunk, LinkedList* list);

void SimulateChunkSpace(ChunkSpace *cs);

// TODO: Create new file for heat simulation
// void SimulateHeatMap(ParticleSimulator* sim);


// Simulator Control
void SlowDownParticleSimulator();
void SpeedUpParticleSimulator();
void IncreaseBrushRadius();
void DecreaseBrushRadius();

#endif