#ifndef CHUNK_SPACE_OPERATIONS_H
#define CHUNK_SPACE_OPERATIONS_H

#include "chunk_space.h"


void CreateParticleCS(ChunkSpace *cs, int x, int y, int type);
void DeleteParticleCS(ChunkSpace *cs, int x, int y);
void SwapParticlesCS(ChunkSpace* cs, int x0, int y0, int x1, int y1);
void ReplaceParticleCS(ChunkSpace *cs, int x, int y, int type);

void CreateParticlesRectCS(ChunkSpace *cs, int startX, int startY, int endX, int endY, int type);
void CreateParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad, int type);

void DeleteParticlesRectCS(ChunkSpace *cs, int startX, int startY, int endX, int endY);
void DeleteParticlesCircleCS(ChunkSpace *cs, int cX, int cY, int rad);

void WallBoxCS(ChunkSpace *cs);
void ClearFullCS(ChunkSpace* cs);

void Explosion(ChunkSpace *cs, int px, int py, int rad, int power, int replaceWith);
void DestructionLine(ChunkSpace *cs, int x0, int y0, int x1, int y1, int power, int replaceWith);

void CreationLineCS(ChunkSpace* cs, int x0, int y0, int x1, int y1, int width, int type);
void DeletionLineCS(ChunkSpace* cs, int x0, int y0, int x1, int y1, int width);
void PutParticleImageCS(ChunkSpace* cs, int x, int y, int scale, ParticleImage pimg);

#endif