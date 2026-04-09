#ifndef CHUNK_LIGHTING_H
#define CHUNK_LIGHTING_H

#include "painter.h"
#include "blur.h"
#include "blend.h"
#include "chunk_space.h"


int start_particle_lighting_sw(int chunk_w, int chunk_h, int partSize);
int change_particle_lighting_sw(int chunk_w, int chunk_h, int partSize);
void end_particle_lighting_sw();

void chunk_to_lightmap(Chunk* chunk);
void draw_chunk_lightmap(Image lightmap, Chunk* chunk, int x, int y);
void draw_cs_lightmap(Image lightmap, ChunkSpace* cs, int x, int y);

void blur_lightmap(Image dest, int times);
void blur_lightmap2(Image dest, Image light_map, int times);

#endif