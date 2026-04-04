#ifndef CONTROLS_H
#define CONTROLS_H

#include "engine_lib.h"
// #include "../partsim/simulator.h"
#include "particle_game.h"
#include "gui_handler.h"

#define LEFT_BUTTON (event.button.button == SDL_BUTTON_LEFT)
#define RIGHT_BUTTON (event.button.button == SDL_BUTTON_RIGHT)
#define C_BUTTON (event.key.keysym.sym == SDLK_c)
#define X_BUTTON (event.key.keysym.sym == SDLK_x)
#define V_BUTTON (event.key.keysym.sym == SDLK_v)
#define R_BUTTON (event.key.keysym.sym == SDLK_r)
#define Z_BUTTON (event.key.keysym.sym == SDLK_z)
#define A_BUTTON (event.key.keysym.sym == SDLK_a)
#define BUTTON_1 (event.key.keysym.sym == SDLK_1)
#define BUTTON_2 (event.key.keysym.sym == SDLK_2)
#define BUTTON_3 (event.key.keysym.sym == SDLK_3)
#define BUTTON_4 (event.key.keysym.sym == SDLK_4)
#define ESC_BUTTON (event.key.keysym.sym == SDLK_ESCAPE)
#define F_BUTTON (event.key.keysym.sym == SDLK_f)
#define P_BUTTON (event.key.keysym.sym == SDLK_p)
#define TAB_BUTTON (event.key.keysym.sym == SDLK_TAB)
#define SPACE_BUTTON (event.key.keysym.sym == SDLK_SPACE)


#define PARTICLE_GENERATION_BUTTON      LEFT_BUTTON
#define DELETE_PARTICLE_BUTTON          RIGHT_BUTTON
#define SCREEN_CLEAR_BUTTON             C_BUTTON
#define COLOR_CHANGE_BUTTON             X_BUTTON
#define PARTICLE_CHANGE_BUTTON          V_BUTTON
#define EXPLOSION_BUTTON                R_BUTTON
#define BRUSH_INCREASE_BUTTON           BUTTON_1
#define BRUSH_DECREASE_BUTTON           BUTTON_2
#define DELAY_INCREASE_BUTTON           BUTTON_3
#define DELAY_DECREASE_BUTTON           BUTTON_4
#define QUIT_BUTTON                     ESC_BUTTON
#define HEATMAP_OF_BUTTON               F_BUTTON
#define PAUSE_OF_BUTTON                 P_BUTTON
#define SIM_CHANGE_OF_BUTTON            TAB_BUTTON


void ProcessInput(ParticleGame* game);


extern void (*MouseLeftEvent)(ParticleGame* game);
extern void (*MouseRightEvent)(ParticleGame* game);
extern void (*MouseScrollEvent)(ParticleGame* game);
extern void (*SpaceButtonEvent)(ParticleGame* game);
extern void (*cButtonEvent)(ParticleGame* game);
extern void (*vButtonEvent)(ParticleGame* game);
extern void (*xButtonEvent)(ParticleGame* game);
extern void (*zButtonEvent)(ParticleGame* game);


#endif