#include "particle_game.h"

#define GAS_GRID_TILE_SIZE 4

typedef struct{
    float density;
    float pressure;
    vec2f velocity;
    float total_energy;
} GasTile;

void euler_gas_sim(ParticleEngine *game){
    vec2f *gas_grid = (vec2f*)malloc(sizeof(vec2f) * game->win->w * game->win->h / GAS_GRID_TILE_SIZE / GAS_GRID_TILE_SIZE);
    
    action_t act_exit = 0;
    add_binding(&game->is, BUTTON_ESCAPE, act_exit);

    while(game->s_params.is_running){
        update_global_time();
        update_input_system(&game->is);

        if(action_pressed(&game->is, act_exit)){
            game->s_params.is_running = FALSE;
        }


    }

    free(gas_grid);
}