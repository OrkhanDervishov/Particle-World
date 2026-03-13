#ifndef PARTICLE_GENERATION_H
#define PARTICLE_GENERATION_H

#include "engine_lib.h"
#include "particle_data.h"


ParticleImage GenerateMagicFromImage(Image img, size_t part_size, int zeroColor);
void DestroyParticleImage(ParticleImage pimg);

#endif