#ifndef PARTICLE_LOAD_H
#define PARTICLE_LOAD_H

#include "particle_data.h"
#include "particle.h"


void ReadParticleConfig(const char* path);

void InitParticles();

void AddType(
    char* name, 
    PartType type, 
    Color color, 
    Color buttonColor, 
    int dens, 
    uint32_t flags, 
    uint8_t iflags, 
    bool (*func)(int x, int y),
    int32_t ht,
    uint32_t durability
);


#endif //PARTICLE_LOAD_H