#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include "gui.h"

void init_gui_handler();

GuiElement* gui_handle(GuiElement* elem, int x, int y, int mx, int my);
GuiElement* on_guibox(GuiElement* elem, int mx, int my);
bool on_button(GuiElement* elem, int mx, int my);

#endif