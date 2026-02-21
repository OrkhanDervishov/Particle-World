#include "gui.h"

void CreateGuiBox(
    GuiBox* box, const char* name, Color color, int8_t layer, 
    bool bg_draw, vec2 pos, vec2 sizes
){
    box = (GuiBox*)malloc(sizeof(GuiBox));
    box->elements = (GuiElement*)malloc(sizeof(GuiElement)*MAX_GUIBOX_ELEMENTS);
    box->elemCount = 0;
    strcpy(box->name, name);
    box->color = color;
    box->layer = layer;
    box->bg_draw = bg_draw;
    box->pos = pos;
    box->sizes = sizes;
}

void DeleteGuiBox(GuiBox* box){
    if(box->elements != NULL){
        free(box->elements);
    }
    if(box != NULL){
        free(box);
    }
}

void CreateButton(
    Button** button, const char *text, Color color,
    bool bg_draw, vec2 pos, vec2 sizes, void *func
){
    *button = (Button*)malloc(sizeof(Button));
    (*button)->text = (char*)malloc(256);
    strcpy((*button)->text, text);
    (*button)->color = color;
    (*button)->bg_draw = bg_draw;
    (*button)->pos = pos;
    (*button)->sizes = sizes;
    (*button)->func = func;
}

void PrintButtonParams(Button* button){
    printf("Allocated: %s\n", button != NULL ? "TRUE" : "FALSE");
    printf("Text: %s\n", button->text);
    printf("Color: r:%d g:%d b:%d a:%d\n", button->color.r, button->color.g, button->color.b, button->color.a);
    printf("bd_draw: %s\n", button->bg_draw ? "TRUE" : "FALSE");
    printf("Pos: x:%d y:%d\n", button->pos.x, button->pos.y);
    printf("Sizes: w:%d h:%d\n", button->sizes.x, button->sizes.y);
}

void DeleteButton(Button** button){
    if((*button)->text != NULL){
        free((*button)->text);
    }
    if(*button != NULL){
        free(*button);
    }
}


void AddButton(GuiBox *box, Button *button){
    if(box->elemCount < MAX_GUIBOX_ELEMENTS) return;
    GuiElement elem = {.type = BUTTON, .element = (void*)button};
    box->elements[box->elemCount++] = elem;
}


// void DrawGuiBox(Window* window, GuiBox* box, int x, int y){

// }

// void DrawButton(Window* window, Button* button){

// }
