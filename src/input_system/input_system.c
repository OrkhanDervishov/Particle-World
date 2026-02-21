#include "input_system.h"


void function1(ParticleGame* game);
void function2(ParticleGame* game);
void ClearFunction(ParticleGame* game);
void ChangeParticleType(ParticleGame* game);
void ToggleGravity(ParticleGame* game);
void BrushIncrease(ParticleGame* game);
void BrushDecrease(ParticleGame* game);
void TakeScreenShot(ParticleGame* game);

void (*MouseLeftEvent)(ParticleGame* game) = function1;
void (*MouseRightEvent)(ParticleGame* game) = function2;
void (*MouseScrollEvent)(ParticleGame* game) = NULL;
void (*SpaceButtonEvent)(ParticleGame* game) = TakeScreenShot;
void (*TabButtonEvent)(ParticleGame* game) = TakeScreenShot;
void (*cButtonEvent)(ParticleGame* game) = ClearFunction;
void (*vButtonEvent)(ParticleGame* game) = ChangeParticleType;
void (*xButtonEvent)(ParticleGame* game) = ToggleGravity;
void (*zButtonEvent)(ParticleGame* game) = ClearFunction;
void (*oneButtonEvent)(ParticleGame* game) = BrushIncrease;
void (*twoButtonEvent)(ParticleGame* game) = BrushDecrease;

void ProcessInput(ParticleGame* game)
{
    SDL_StopTextInput();

    Window* win = game->win;
    SDL_Event event;
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);

    
    // Particle generation and GUI Interraction
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)) MouseLeftEvent(game);
    if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) MouseRightEvent(game);


    // Other events
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) win->isrunning = 0;
        
        if(event.type == SDL_KEYUP){
            if(QUIT_BUTTON) win->isrunning = 0;
            
            if(SCREEN_CLEAR_BUTTON){
                cButtonEvent(game);
            }
            if(PARTICLE_CHANGE_BUTTON){
                vButtonEvent(game);
            }
            if(X_BUTTON){
                xButtonEvent(game);
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
            if(BRUSH_INCREASE_BUTTON) oneButtonEvent(game);
            if(BRUSH_DECREASE_BUTTON) twoButtonEvent(game);
            if(DELAY_INCREASE_BUTTON) SpeedUpParticleSimulator();
            if(DELAY_DECREASE_BUTTON) SlowDownParticleSimulator();
            if(TAB_BUTTON) TabButtonEvent(game); 
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

        // CreateParticlesRectCS(&(game->cs), px - bs, py - bs, 2*bs, 2*bs, game->g_params.selectedParticleType);
        CreateParticlesCircleCS(&(game->cs), px, py, bs, game->g_params.selectedParticleType);
        // AddDirtyRect(sim, px, py, RADIUS);
    // }
}

void function2(ParticleGame* game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    int px = ((mx) / DEFAULT_PARTICLE_SIZE);
    int py = ((my) / DEFAULT_PARTICLE_SIZE);

    int bs = game->g_params.brush_size;
    // DeleteParticlesRectCS(&(game->cs), px - bs, py - bs, 2*bs, 2*bs);
    DeleteParticlesCircleCS(&(game->cs), px, py, bs);
    // AddDirtyRect(sim, px, py, RADIUS);
}


void ClearFunction(ParticleGame* game){
    ClearFullCS(&game->cs);
}

void ChangeParticleType(ParticleGame* game){
    game->g_params.selectedParticleType++;
    game->g_params.selectedParticleType %= GetParticleTypeCount();
}

void ToggleGravity(ParticleGame* game){
    gravity = gravity == 1 ? -1 : 1;
}

void BrushIncrease(ParticleGame* game){
    if(game->g_params.brush_size >= 20) return;
    game->g_params.brush_size++;
}

void BrushDecrease(ParticleGame* game){
    if(game->g_params.brush_size <= 1){
        game->g_params.brush_size = 1;
        return;
    }
    game->g_params.brush_size--;
}

void TakeScreenShot(ParticleGame* game){
    SaveImagePNG(&(game->win->screen), "picture.png");
    printf("worked\n");
}