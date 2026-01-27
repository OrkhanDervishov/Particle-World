#ifndef CONTROLS_H
#define CONTROLS_H

#include "engine/engine_api.h"
#include "partsim/simulator.h"
#include "particle_game.h"

#define PARTICLE_GENERATION_BUTTON (event.button.button == SDL_BUTTON_LEFT)
#define DELETE_PARTICLE_BUTTON (event.button.button == SDL_BUTTON_RIGHT)
#define SCREEN_CLEAR_BUTTON (event.key.keysym.sym == SDLK_c)
#define COLOR_CHANGE_BUTTON (event.key.keysym.sym == SDLK_x)
#define PARTICLE_CHANGE_BUTTON (event.key.keysym.sym == SDLK_v)
#define EXPLOSION_BUTTON (event.key.keysym.sym == SDLK_r)
#define BRUSH_INCREASE_BUTTON (event.key.keysym.sym == SDLK_1)
#define BRUSH_DECREASE_BUTTON (event.key.keysym.sym == SDLK_2)
#define DELAY_INCREASE_BUTTON (event.key.keysym.sym == SDLK_3)
#define DELAY_DECREASE_BUTTON (event.key.keysym.sym == SDLK_4)
#define QUIT_BUTTON (event.key.keysym.sym == SDLK_ESCAPE)
#define HEATMAP_OF_BUTTON (event.key.keysym.sym == SDLK_f)
#define PAUSE_OF_BUTTON (event.key.keysym.sym == SDLK_p)
#define SIM_CHANGE_OF_BUTTON (event.key.keysym.sym == SDLK_TAB)

void ProcessInput(ParticleGame* game);

#endif