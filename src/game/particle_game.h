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
    int selectedParticleType;
} GameParameters;

typedef struct{
    Window* win;
    ChunkSpace cs;
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

#endif //PARTICLE_GAME_H