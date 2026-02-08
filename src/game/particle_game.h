#ifndef PARTICLE_GAME_H
#define PARTICLE_GAME_H

// #include "engine/engine_api.h"
// #include "partsim/particle_data.h"
// #include "partsim/particle_load.h"
// #include "partsim/particle.h"
// #include "partsim/simulator.h"
// // #include "game_gui.h"
// // #include "world.h"
// #include "chunk.h"
// // #include "drawer.h"

#include "../engine/engine_lib.h"
#include "../partsim/particle_load.h"
#include "../structs/chunk.h"
#include "../structs/region.h"
#include "../structs/chunk_space.h"
#include "../rendering/chunk_renderer.h"
#include "../rendering/renderer.h"
#include "../partsim/dirtyrect.h"
#include "../partsim/particle_data.h"
#include "../partsim/simulator.h"

#define ICON_PATH "./src/images/sand.bmp"
#define BG_PATH "./src/images/background.bmp"

typedef struct{
    bool paused;
    int delay;
    bool frameKeepEnabled;
    int frameKeep;

    Color bg_color;
    bool hm_mode;
} GameSystemParameters;

typedef struct{
    int brush_size;
    int selectedType;
} GameParameters;

typedef struct{
    Window* win;
    ChunkSpace cs;
    SDL_Event event;
    GameSystemParameters s_params;
    GameParameters g_params;
} ParticleGame;


#define PAUSE_GAME(g) (*g)->s_params.pause = TRUE
#define CONTINUE_GAME(g) (*g)->s_params.pause = FALSE
#define ENABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = TRUE
#define DISABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = FALSE

int CreateParticleGame(ParticleGame** game);
void DeleteParticleGame(ParticleGame** game);

int BuildLabEnv(ParticleGame* game);
int RunParticleGame(ParticleGame* game);

// Particle Inits
void InitParticles();
void InitParticleSimulator();

// TODO: Rigid Inits


// TODO: Entity Inits


// TODO: Alphabet Inits


// Gameplay
// void CreateManyParticles(World* world, int px, int py, int rad, int t);
// void CreateManyParticlesWithRarity(World* world, int px, int py, int rad, int t, int rarity);
// // void DeleteParticle(World* world, int px, int py);
// void DeleteManyParticles(World* world, int px, int py, int rad);
// // void SwapParticles(World* world, Particle* p1, Particle* p2);
// void WallBox(World* world);
// void Explosion(World* world, int px, int py, int rad, int power, int replaceWith);
// void DestructionLine(World* world, int px0, int py0, int px1, int py1, int power, int replaceWith);

#endif //PARTICLE_GAME_H