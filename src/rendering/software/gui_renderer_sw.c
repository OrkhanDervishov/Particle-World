#include "gui_renderer_sw.h"

#include "basic_text_renderer.h"
void (*elems[64])(Window* window, GuiElement* box, int x, int y);

void InitGuiRenderer(){
    elems[GUI_BOX] = DrawGuiBox;
    elems[BUTTON] = DrawButton;
}


void DrawGuiElement(Window* window, GuiElement* elem, int x, int y){
    elems[elem->type](window, elem, x, y);
}

void DrawGuiBox(Window* window, GuiElement* elem, int x, int y){
    if(elem->type != GUI_BOX) return;
    GuiBox* gb = (GuiBox*)elem->element;
    int rel_x = x + gb->pos.x;
    int rel_y = y + gb->pos.y;
    if(gb->bg_draw){
        Rect rect = {rel_x, rel_y, gb->sizes.x, gb->sizes.y};
        draw_filled_rect_f(window->context, rect, gb->color);
    }

    for(int i = 0; i < gb->elem_count; i++){
        GuiElement ge = gb->elements[i];
        elems[ge.type](window, &ge, rel_x, rel_y);
    }
}

void DrawButton(Window* window, GuiElement* elem, int x, int y){
    if(elem->type != BUTTON) return;
    Button* button = (Button*)elem->element;
    int rel_x = x + button->pos.x;
    int rel_y = y + button->pos.y;
    if(button->bg_draw){
        Rect rect = {rel_x, rel_y, button->sizes.x, button->sizes.y};
        draw_filled_rect_f(window->context, rect, button->color);
    }
    Color textColor = {.rgba=0xFFFFFFFF};
    BasicTextRender(
        window, button->text, rel_x+(button->sizes.x>>2), 
        rel_y+(button->sizes.y>>2), 1, get_negative(button->color)
    );
}