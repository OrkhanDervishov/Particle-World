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

#include "engine_lib.h"
#include "particle_load.h"
#include "chunk.h"
#include "region.h"
#include "chunk_space.h"
#include "chunk_renderer.h"
#include "renderer.h"
#include "dirtyrect.h"
#include "particle_data.h"
#include "simulator.h"
#include "gui_handler.h"
#include "mouse.h"
#include "camera.h"

#define ICON_PATH "./src/images/sand.bmp"
#define BG_PATH "./src/images/background.bmp"


typedef struct{
    bool paused;
    int delay;
    bool frameLockEnabled;
    int frameLock;
    
    Color bg_color;
    bool hm_mode;
} GameSystemParameters;

typedef struct{
    int brush_size;
    int selectedParticleType;
} GameParameters;

#define CB_COUNT_MAX 100
typedef struct ParticleGame{
    Window* win;
    ChunkSpace cs;
    GuiElement gui;
    GameSystemParameters s_params;
    GameParameters g_params;
    Mouse mouse;
    Camera2D camera;
    int cbCount;
    void (*callbacks[CB_COUNT_MAX])(struct ParticleGame* game);
} ParticleGame;
#define PARTICLE_GAME_FUNC void (*)(ParticleGame* game)


#define PAUSE_GAME(g) (*g)->s_params.pause = TRUE
#define CONTINUE_GAME(g) (*g)->s_params.pause = FALSE
#define ENABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = TRUE
#define DISABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = FALSE


int CreateParticleGame(ParticleGame** game);
void DeleteParticleGame(ParticleGame** game);

int add_callback_pg(ParticleGame* game, void (*callback)(ParticleGame* game));
void delete_callback_pg(ParticleGame* game, int cb_index);
// void call_all_callbacks(ParticleGame* game);

int BuildLabEnv(ParticleGame* game);

#endif //PARTICLE_GAME_H