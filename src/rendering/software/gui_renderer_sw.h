#ifndef GUI_RENDERER_SW_H
#define GUI_RENDERER_SW_H

#include "gui.h"
#include "window.h"

void InitGuiRenderer();
void DrawGuiElement(Window* window, GuiElement* elem, int x, int y);
void DrawGuiBox(Window* window, GuiElement* elem, int x, int y);
void DrawButton(Window* window, GuiElement* elem, int x, int y);

#endif