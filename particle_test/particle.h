#ifndef PARTICLE_H
#define PARTICLE_H

#include "common.h"

typedef enum{
    EMPTY = 0,
    SAND,
    WATER,
    BORDER
} PartType;

typedef enum{
    EMPTY_DENSITY = 0,
    WATER_DENSITY = 50,
    SAND_DENSITY = 100,
    BORDER_DENSITY = 1000000
} PartDensity;

typedef struct{
    Pos p;
    int id;
    Color c;
    PartType t;
    int dens;
    int velo;
} Particle;

#endif //PARTICLE_H