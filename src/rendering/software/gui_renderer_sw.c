#include "gui_renderer_sw.h"

void DrawGuiElement(Window* window, GuiElement* elem, int x, int y){

    switch (elem->type)
    {
    case GUI_BOX:
        DrawGuiBox(window, elem->element, x, y);
        break;
    case BUTTON:
        DrawButton(window, elem->element, x, y);
        break;
    }
}

void DrawGuiBox(Window* window, GuiBox* box, int x, int y){

}

void DrawButton(Window* window, Button* button, int x, int y){
    if(button->bg_draw){
        Rect rect = {x, y, button->sizes.x, button->sizes.y};
        draw_filled_rect_f(window->context, rect, button->color);
    }
    Color textColor = {.rgba=0xFFFFFFFF};
    BasicTextRender(window, button->text, x+1, y+1, 1, textColor);
}