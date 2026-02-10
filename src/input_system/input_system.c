#include "input_system.h"


void function1(ParticleGame* game);
void function2(ParticleGame* game);
void ClearFunction(ParticleGame* game);

void (*MouseLeftEvent)(ParticleGame* game) = function1;
void (*MouseRightEvent)(ParticleGame* game) = function2;
void (*MouseScrollEvent)(ParticleGame* game) = ClearFunction;
void (*SpaceButtonEvent)(ParticleGame* game) = ClearFunction;
void (*cButtonEvent)(ParticleGame* game) = ClearFunction;
void (*vButtonEvent)(ParticleGame* game) = ClearFunction;
void (*xButtonEvent)(ParticleGame* game) = ClearFunction;
void (*zButtonEvent)(ParticleGame* game) = ClearFunction;

void ProcessInput(ParticleGame* game)
{
    Window* win = game->win;
    SDL_Event event = game->event;
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    // int px = ((mx) / sim->pSide);
    // int py = ((my) / sim->pSide);

    // Particle generation and GUI Interraction
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)) MouseLeftEvent(game);
    if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) MouseRightEvent(game);


    // Other events
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) win->isrunning = 0;

        if(event.type == SDL_KEYDOWN){
            // printf("works\n");
            if(QUIT_BUTTON) win->isrunning = 0;
            if(SCREEN_CLEAR_BUTTON){
                
            }
            if(PARTICLE_CHANGE_BUTTON){
                selectedType = (selectedType + 1) % countParticleTypes;
            }
            if(PAUSE_OF_BUTTON){
                game->s_params.paused = game->s_params.paused ? 0 : 1;
            }
            if(HEATMAP_OF_BUTTON){
                game->s_params.hm_mode = game->s_params.hm_mode ? 0 : 1;
            }
            if(EXPLOSION_BUTTON){
                int px = ((mx) / PART_SIDE);
                int py = ((my) / PART_SIDE);
                // Explosion(game->w, px, py, 3*RADIUS, 200, PHANTOM);
            }
            if(BRUSH_INCREASE_BUTTON) IncreaseBrushRadius();
            if(BRUSH_DECREASE_BUTTON) DecreaseBrushRadius();
            if(DELAY_INCREASE_BUTTON) SpeedUpParticleSimulator();
            if(DELAY_DECREASE_BUTTON) SlowDownParticleSimulator();
        }
    }
}



void function1(ParticleGame* game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    // if(!CheckGuiButtons(game->win, mx, my)){
        int created = 0;
        int px = ((mx) / DEFAULT_PARTICLE_SIZE);
        int py = ((my) / DEFAULT_PARTICLE_SIZE);

        int bs = game->g_params.brush_size;

        CreateParticlesRectCS(&(game->cs), px - bs, py - bs, 2*bs, 2*bs, selectedType);
        // AddDirtyRect(sim, px, py, RADIUS);
    // }
}

void function2(ParticleGame* game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    int px = ((mx) / DEFAULT_PARTICLE_SIZE);
    int py = ((my) / DEFAULT_PARTICLE_SIZE);

    int bs = game->g_params.brush_size;
    DeleteParticlesRectCS(&(game->cs), px - bs, py - bs, 2*bs, 2*bs);
    // AddDirtyRect(sim, px, py, RADIUS);
}


void ClearFunction(ParticleGame* game){

}