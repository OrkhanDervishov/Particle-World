#ifndef PARTICLE_H
#define PARTICLE_H


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

// Third party
#include <SDL2/SDL.h>

// App
#include "particle_data.h"
#include "../engine/engine_api.h"
#include "../structs/cs_ops.h"

// GUI macros
#define MAX_BUTTONS 100
#define ELEMENT_BUTTON_SIZE 16
#define ELEMENT_BUTTONS_MARGIN 4
#define ELEMENT_BUTTON_MARGIN_H 16
#define ELEMENT_BUTTON_MARGIN_W 30

#define M_SAND_BUTTON_COLOR 255, 255, 0, 255
#define M_COAL_BUTTON_COLOR 0, 0, 0, 255
#define M_WATER_BUTTON_COLOR 0, 0, 255, 255
#define M_FUNGUS_BUTTON_COLOR 252, 70, 170, 255
#define M_ACID_BUTTON_COLOR 0, 255, 0, 255
#define M_WALL_BUTTON_COLOR 100, 100, 100, 255
#define M_STEAM_BUTTON_COLOR 150, 150, 255, 255
#define M_FIRE_BUTTON_COLOR 255, 0, 0, 255
#define M_SMOKE_BUTTON_COLOR 40, 40, 40, 255
#define M_OIL_BUTTON_COLOR 131, 66, 0, 255
#define M_LAVA_BUTTON_COLOR 255, 69, 0, 255
#define M_OBSIDIAN_BUTTON_COLOR 20, 20, 20, 255


extern int STEAM_LIFE_TIME;
extern int SMOKE_LIFE_TIME;
extern int ACID_EFFECT_TIME;
extern int FIRE_LIFE_TIME;
extern int FIRE_EFFECT_TIME;
extern int BURNING_COAL_LIFE_TIME;
extern int BURNING_OIL_LIFE_TIME;
extern int BURNING_FUNGUS_LIFE_TIME;

extern int FIRE_HEAT_RELEASE_TEMP;
extern int WATER_TO_STEAM_TEMP;
extern int COAL_TO_FIRE_TEMP;
extern int OIL_TO_FIRE_TEMP;
extern int FUNGUS_TO_FIRE_TEMP;
extern int WATER_HEAT_ABSORB;

void SetChunkSpace(ChunkSpace* cs);

// Simulation
bool BasicDustBehave(int x, int y);
// bool BasicLiquidBehave(int x, int y);
// bool BasicGasBehave(int x, int y);
// bool BasicHeatAbsorberBehave(int x, int y);
// bool BasicHeatReleaserBehave(int x, int y);
// bool BasicAcidicBehave(int x, int y);

bool SandBehave(int x, int y);
// bool WaterBehave(int x, int y);
// bool OilBehave(int x, int y);
// bool AcidBehave(int x, int y);
// bool SteamBehave(int x, int y);
// bool FungusBehave(int x, int y);
// bool FireBehave(int x, int y);
// bool CoalBehave(int x, int y);
// bool SmokeBehave(int x, int y);
// bool LavaBehave(int x, int y);
// bool WoodBehave(int x, int y);
// bool FireSmokeBehave(int x, int y);
// bool PowderBehave(int x, int y);
// bool ObsidianBehave(int x, int y);
// bool PhantomBehave(int x, int y);
// bool SourceBehave(int x, int y);

// extern void (*CreateParticle)(Chunk* chunk, ParticleType type, int px, int py);
// extern void (*CreateReplaceParticle)(Chunk* chunk, ParticleType type, int px, int py);
// extern void (*ReplaceParticle)(Chunk* chunk, int sx, int sy, int dx, int dy);
// extern void (*DeleteParticle)(Chunk* chunk, int px, int py);
// void SwapParticles(Chunk* chunk, int x1, int y1, int x2, int y2);
// void SwapParticles2(Particle* p1, Particle* p2);
#endif  