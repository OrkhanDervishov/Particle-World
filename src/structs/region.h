#ifndef REGION_H
#define REGION_H

#include "chunk.h"

#define DEFAULT_REGION_WIDTH 1
#define DEFAULT_REGION_HEIGHT 1

#define REGION_GET_CHUNK(r, x, y) (r).chunks[(y)*(r).w + (x)]

typedef struct{
    Pos region_world_pos;

    bool active;
    size_t w, h;
    Chunk* chunks;
} Region;



// Region functions
int CreateRegion(Region* region, size_t w, size_t h, Pos worldPos);
void DeleteRegion(Region* region);
int ClearRegion(Region* region);
int FillRegion(Region* region);

void ActivateRegion(Region* region);

void ColorRegion(Region* region, Color color);

#endif