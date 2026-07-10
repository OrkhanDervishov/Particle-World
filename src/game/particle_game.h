#ifndef PARTICLE_GAME_H
#define PARTICLE_GAME_H

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
#include "entity.h"
#include "input_system.h"
#include "constants.h"
#include "asset.h"
#include "asset_renderer.h"


typedef struct{
    // Core
    bool is_running;
    bool paused;
    bool restart;

    // Time
    int delay;
    bool frameLockEnabled;
    int frameLock;
    
    // Window
    char *title;
    size_t width;
    size_t height;
    bool fullscreen;
    bool window_visible;
    PWWindowRenderingApi window_render_api;

    
    // Visual
    bool use_custom_cursor;
    char* cursor_path;

    char* icon_path;
    Color clear_color;
} GameSystemParameters;

typedef struct{
    int brush_size;
    int selectedParticleType;
} GameParameters;

#define CB_COUNT_MAX 100
typedef struct ParticleEngine{
    Window* win;
    ChunkSpace cs;
    GuiElement gui;
    GameSystemParameters s_params;
    GameParameters g_params;
    Mouse mouse;
    PWCamera2D camera;
    InputSystem is;
    PWEntityManager em;
    PWAssetManager am;
    int cbCount;
    void (*callbacks[CB_COUNT_MAX])(struct ParticleEngine* game);
} ParticleEngine;
#define PARTICLE_GAME_FUNC void (*)(ParticleEngine* game)


#define PAUSE_GAME(g) (*g)->s_params.pause = TRUE
#define CONTINUE_GAME(g) (*g)->s_params.pause = FALSE
#define ENABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = TRUE
#define DISABLE_HEATMAP_MOD(g) (*g)->s_params.hm_mode = FALSE


int CreateParticleEngine(ParticleEngine** game, const char* conf_path);
void DeleteParticleEngine(ParticleEngine** game);

int add_callback_pg(ParticleEngine* game, void (*callback)(ParticleEngine* game));
void delete_callback_pg(ParticleEngine* game, int cb_index);
// void call_all_callbacks(ParticleEngine* game);

int BuildLabEnv(ParticleEngine* game);

#endif //PARTICLE_GAME_H