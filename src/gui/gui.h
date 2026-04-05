#ifndef GUI_H
#define GUI_H

#include "engine_lib.h"
// #include "particle_game.h"



typedef enum{
    BUTTON = 0,
    GUI_BOX
} GuiElementType;

typedef struct
{
    GuiElementType type;
    void *element;
    float cooldown;
    float last_time;
} GuiElement;


#define MAX_GUIBOX_ELEMENTS 20
typedef struct{
    char* name;
    Color color;
    int8_t layer;
    bool active;
    bool bg_draw;
    bool element_draw;
    vec2 pos;
    vec2 sizes;
    int8_t elem_count;
    GuiElement* elements;
} GuiBox;

#define MAX_BUTTON_TEXT_LEN 64
typedef struct{
    char* text;
    Color color;
    bool active;
    bool bg_draw;
    vec2 pos;
    vec2 sizes;
    void (*func)();
} Button;

void CreateGuiBox(
    GuiBox** box, const char* name, Color color, int8_t layer, 
    bool bg_draw, vec2 pos, vec2 sizes
);
void PrintGuiBoxParams(GuiBox* box);
void DeleteGuiBox(GuiBox** box);
void CreateButton(
    Button** button, const char *text, Color color,
    bool bg_draw, vec2 pos, vec2 sizes, void (*func)()
);
void PrintButtonParams(Button* button);
void DeleteButton(Button** button);
void AddButton(GuiBox *box, Button *button, float cooldown);

void add_button_gui(GuiElement *elem, Button *button, float cooldown);

#endif