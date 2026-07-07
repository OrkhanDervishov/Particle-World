/* imgui_bridge.h */

#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "cimgui.h"

#ifdef __cplusplus
extern "C" {
#endif

bool cImGui_ImplSDLRenderer2_Init(SDL_Renderer* renderer);
void cImGui_ImplSDLRenderer2_NewFrame(void);
void cImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data, SDL_Renderer* renderer);
void cImGui_ImplSDLRenderer2_Shutdown(void);

bool cImGui_ImplSDL2_InitForOpenGL(SDL_Window*, void*);
bool cImGui_ImplSDL2_InitForSDLRenderer(SDL_Window*, SDL_Renderer*);
void cImGui_ImplSDL2_NewFrame(void);
bool cImGui_ImplSDL2_ProcessEvent(const SDL_Event*);
void cImGui_ImplSDL2_Shutdown(void);

float cImGui_ImplSDL2_GetContentScaleForDisplay(int);

bool cImGui_ImplOpenGL3_Init(const char*);
void cImGui_ImplOpenGL3_NewFrame(void);
void cImGui_ImplOpenGL3_RenderDrawData(ImDrawData*);
void cImGui_ImplOpenGL3_Shutdown(void);

void cImGuiStyle_ScaleAllSizes(ImGuiStyle* style, float scale);

#ifdef __cplusplus
}
#endif