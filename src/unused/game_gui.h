#ifndef GUI_H
#define GUI_H

#include "engine/engine_api.h"
#include "partsim/simulator.h"

// GUI macros
#define MAX_BUTTONS 100
#define ELEMENT_BUTTON_SIZE 16
#define ELEMENT_BUTTONS_MARGIN 4
#define ELEMENT_BUTTON_MARGIN_H 16
#define ELEMENT_BUTTON_MARGIN_W 30

#define M_SAND_BUTTON_COLOR 255, 255, 0, 255
#define M_COAL_BUTTON_COLOR 0, 0, 0, 255
#define M_WATER_BUTTON_COLOR 0, 0, 255, 255
#define M_FUNGUS_BUTTON_COLOR 252, 70, 170, 255
#define M_ACID_BUTTON_COLOR 0, 255, 0, 255
#define M_WALL_BUTTON_COLOR 100, 100, 100, 255
#define M_STEAM_BUTTON_COLOR 150, 150, 255, 255
#define M_FIRE_BUTTON_COLOR 255, 0, 0, 255
#define M_SMOKE_BUTTON_COLOR 40, 40, 40, 255
#define M_OIL_BUTTON_COLOR 131, 66, 0, 255
#define M_LAVA_BUTTON_COLOR 255, 69, 0, 255
#define M_OBSIDIAN_BUTTON_COLOR 20, 20, 20, 255


extern SDL_Rect buttons[MAX_BUTTONS];
extern int buttonCount;


void CreateGui(Window* win, SDL_Rect* buttons, int* buttonCount, int elementCount);
void DrawGui(Window* win, SDL_Rect* buttons, int buttonCount);
int CheckGuiButtons(Window* win, int mx, int my);


#endif