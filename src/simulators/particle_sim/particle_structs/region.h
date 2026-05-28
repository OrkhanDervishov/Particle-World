#ifndef REGION_H
#define REGION_H

#include "chunk.h"

#define DEFAULT_REGION_WIDTH 3
#define DEFAULT_REGION_HEIGHT 3

#define REGION_GET_CHUNK(r, x, y) (r).chunks[(y)*(r).w + (x)]

typedef struct{

    bool active;
    size_t w, h;
    size_t chunk_width, chunk_height;
    Chunk* chunks;
} Region;



// Region functions
int CreateRegion(Region* region, size_t region_w, size_t region_h, size_t chunk_w, size_t chunk_h);
void DeleteRegion(Region* region);
int ClearRegion(Region* region);
int FillRegion(Region* region);

void ActivateRegion(Region* region);

void ColorRegion(Region* region, Color color);

#endif