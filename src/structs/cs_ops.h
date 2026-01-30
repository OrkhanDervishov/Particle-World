#ifndef CS_OPS_H
#define CS_OPS_H

#include "chunk_space.h"
#include "../partsim/simulator.h"


void CreateParticleCS(ChunkSpace *cs, int x, int y, int type);
void DeleteParticleCS(ChunkSpace *cs, int x, int y);
void SwapParticlesCS(ChunkSpace* cs, int x0, int y0, int x1, int y1);
void ReplaceParticleCS(ChunkSpace *cs, int x, int y, int type);

void CreateParticlesRectCS(ChunkSpace *cs, int startX, int startY, int endX, int endY, int type);
void CreateParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad, int type);

void DeleteParticlesRectCS(ChunkSpace *cs, int startX, int startY, int endX, int endY);
void DeleteParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad);

// void SimulateChunkSpace(ChunkSpace *cs);

#endif