#include "controls.h"


void function1(ParticleGame* game, int mx, int my);
void function2(ParticleGame* game, int mx, int my);

void (*MouseLeftEvent)(ParticleGame* game, int mx, int my) = function1;
void (*MouseRightEvent)(ParticleGame* game, int mx, int my) = function2;

void ProcessInput(ParticleGame* game)
{
    Window* win = game->win;
    SDL_Event event = game->event;
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    // int px = ((mx) / sim->pSide);
    // int py = ((my) / sim->pSide);

    // Particle generation and GUI Interraction
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)) MouseLeftEvent(game, mx, my);
    if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) MouseRightEvent(game, mx, my);


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
                game->paused = game->paused ? 0 : 1;
            }
            if(HEATMAP_OF_BUTTON){
                game->hm_mode = game->hm_mode ? 0 : 1;
            }
            if(EXPLOSION_BUTTON){
                int px = ((mx) / PART_SIDE);
                int py = ((my) / PART_SIDE);
                Explosion(game->w, px, py, 3*RADIUS, 200, PHANTOM);
            }
            if(BRUSH_INCREASE_BUTTON) IncreaseBrushRadius();
            if(BRUSH_DECREASE_BUTTON) DecreaseBrushRadius();
            if(DELAY_INCREASE_BUTTON) SpeedUpParticleSimulator();
            if(DELAY_DECREASE_BUTTON) SlowDownParticleSimulator();
        }
    }
}



void function1(ParticleGame* game, int mx, int my){
    if(!CheckGuiButtons(game->win, mx, my)){
        int created = 0;
        int px = ((mx) / PART_SIDE);
        int py = ((my) / PART_SIDE);
        CreateManyParticles(game->w, px, py, RADIUS, selectedType);
        // AddDirtyRect(sim, px, py, RADIUS);
    }
}

void function2(ParticleGame* game, int mx, int my){
    int px = ((mx) / PART_SIDE);
    int py = ((my) / PART_SIDE);
    DeleteManyParticles(game->w, px, py, RADIUS);
    // AddDirtyRect(sim, px, py, RADIUS);
}