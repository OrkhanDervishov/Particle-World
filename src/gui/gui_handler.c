#include "gui_handler.h"

bool (*handlers[64])(GuiElement* elem, int mx, int my);

void init_gui_handler(){
    handlers[BUTTON] = on_button;
}

GuiElement* gui_handle(GuiElement* elem, int x, int y, int mx, int my){
    mx -= x;
    my -= y;
    if(elem->type == GUI_BOX){
        GuiElement* e = on_guibox(elem, mx, my);
        return e;
    }
    else{
        bool val = handlers[elem->type](elem, mx, my);
        if(val) return elem;
    }
    return NULL;
}

GuiElement* on_guibox(GuiElement* elem, int mx, int my){
    if(elem->type != GUI_BOX) return NULL;
    GuiBox* gb = (GuiBox*)elem->element;
    int rel_x = mx - gb->pos.x;
    int rel_y = my - gb->pos.y;

    bool val = FALSE;
    for(int i = 0; i < gb->elem_count; i++){
        GuiElement ge = gb->elements[i];
        val = handlers[ge.type](&ge, rel_x, rel_y);
        if(val){
            return &gb->elements[i];
        }
    }
    return NULL;
}

bool on_button(GuiElement* elem, int mx, int my){
    if(elem->type != BUTTON) return FALSE;
    Button* button = (Button*)elem->element;
    int rel_x = mx - button->pos.x;
    int rel_y = my - button->pos.y;

    Rect rect = {
        0,
        0,
        button->sizes.x,
        button->sizes.y
    };
    
    // printf("works\n");
    // CONSOLE_RECT(rect);
    // printf("button relx:%d rely:%d\n", rel_x, rel_y);
    // printf("x:%d y:%d\n", mx, my);
    if(
        rect.x < rel_x && rect.y < rel_y &&
        rect.w > rel_x && rect.h > rel_y
    ){
        return TRUE;
    }
    return FALSE;
}