#ifndef GUI_RENDERER_SW_H
#define GUI_RENDERER_SW_H

#include "gui.h"
#include "basic_text_renderer.h"

void DrawGuiElement(Window* window, GuiElement* elem, int x, int y);
void DrawGuiBox(Window* window, GuiBox* box, int x, int y);
void DrawButton(Window* window, Button* button, int x, int y);

#endif