#include "game_gui.h"

SDL_Rect buttons[MAX_BUTTONS];
int buttonCount = 0;




void DrawGui(Window* win, SDL_Rect* buttons, int buttonCount){
    for(int i = 0; i < buttonCount; i++){
        Color c = typeButtonColorList[i];
        SDL_SetRenderDrawColor(win->renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(win->renderer, &buttons[i]);
    }

    SDL_SetRenderDrawColor(win->renderer, MWHITE);
    SDL_RenderDrawRect(win->renderer, &buttons[selectedType]);
}

int CheckGuiButtons(Window* win, int mx, int my){

    for(int i = 0; i < buttonCount; i++){
        if(my > buttons[i].y && mx > buttons[i].x && 
        my <= buttons[i].y + buttons[i].h && mx <= buttons[i].x + buttons[i].w){
            selectedType = i;
            return 1;
        }
    }
    return 0;
}

void CreateGui(Window* win, SDL_Rect* buttons, int* buttonCount, int elementCount){
    SDL_Rect button = {
        win->w - ELEMENT_BUTTON_MARGIN_W,
        ELEMENT_BUTTON_MARGIN_H,
        ELEMENT_BUTTON_SIZE,
        ELEMENT_BUTTON_SIZE
    };
    
    for(int i = 0; i < elementCount; i++){
        button.y = ELEMENT_BUTTON_MARGIN_H + i * (ELEMENT_BUTTON_SIZE + ELEMENT_BUTTONS_MARGIN);
        buttons[i] = button; 
    }
    *buttonCount = elementCount;
}