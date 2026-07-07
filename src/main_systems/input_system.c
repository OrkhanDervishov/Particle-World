#include "input_system.h"



void add_binding(InputSystem* is, KeyCode keycode, action_t action){
    if(keycode == BUTTON_UNKNOWN) return;
    if(is->bindings[action].count >= MAX_KEYS_FOR_ACTION) return;

    // Prevent duplicate keycodes
    for(uint8_t i = 0; i < is->bindings[action].count; i++){
        if(is->bindings[action].list[i] == keycode) return;
    }

    is->bindings[action].list[is->bindings[action].count++] = keycode;
}

void remove_binding(InputSystem* is, KeyCode keycode, action_t action){
    if(keycode == BUTTON_UNKNOWN) return;

    for(uint8_t i = 0; i < is->bindings[action].count; i++){
        KeyCode kc = is->bindings[action].list[i];
        if(kc == keycode){
            if(i == is->bindings[action].count-1){
                is->bindings[action].list[i] = BUTTON_UNKNOWN;
            }
            else{
                // Left shift all keycodes
                for(uint8_t j = i; j < is->bindings[action].count-1; j++){
                    is->bindings[action].list[j] = is->bindings[action].list[j+1];
                }
            }
            is->bindings[action].count--;
        }
    }
}



bool button_down(InputSystem* is, KeyCode keycode){
    return is->buttons[keycode].down;
}

bool button_pressed(InputSystem* is, KeyCode keycode){
    return is->buttons[keycode].pressed;
}

bool button_released(InputSystem* is, KeyCode keycode){
    return is->buttons[keycode].released;
}

bool action_down(InputSystem* is, action_t action){
    for(uint8_t i = 0; i < is->bindings[action].count; i++){
        KeyCode kc = is->bindings[action].list[i];
        if(is->buttons[kc].down){
            return TRUE;
        }
    }
    return FALSE;
}

bool action_pressed(InputSystem* is, action_t action){
    for(uint8_t i = 0; i < is->bindings[action].count; i++){
        KeyCode kc = is->bindings[action].list[i];
        if(is->buttons[kc].pressed){
            return TRUE;
        }
    }
    return FALSE;
}

bool action_released(InputSystem* is, action_t action){

    for(uint8_t i = 0; i < is->bindings[action].count; i++){
        KeyCode kc = is->bindings[action].list[i];
        if(is->buttons[kc].released){
            return TRUE;
        }
    }
    return FALSE;
}


void update_mouse(InputSystem* is){
    
    is->mouse.dx = (is->mouse.xprev - is->mouse.x);
    is->mouse.dy = (is->mouse.yprev - is->mouse.y);
    
    is->mouse.xprev = is->mouse.x;
    is->mouse.yprev = is->mouse.y;
    
    int x, y;
    uint32_t state = SDL_GetMouseState(&x, &y);
    is->mouse.x = (float)x;
    is->mouse.y = (float)y;
    is->mouse.xrel = 0.0f;
    is->mouse.yrel = 0.0f;
    is->mouse.xwheel = 0.0f;
    is->mouse.ywheel = 0.0f;


    
    if(state & SDL_BUTTON_LMASK){
        if(is->mouse.left.down == TRUE) is->mouse.left.pressed = FALSE;
        else is->mouse.left.pressed = TRUE;
        is->mouse.left.down = TRUE;
        is->mouse.left.released = FALSE;
    }else{
        is->mouse.left.down = FALSE;
        is->mouse.left.released = TRUE;
    }
    if(state & SDL_BUTTON_RMASK){
        if(is->mouse.right.down == TRUE) is->mouse.right.pressed = FALSE;
        else is->mouse.right.pressed = TRUE;
        is->mouse.right.down = TRUE;
        is->mouse.right.released = FALSE;
    }else{
        is->mouse.right.down = FALSE;
        is->mouse.right.released = TRUE;
    }
    if(state & SDL_BUTTON_MMASK){
        if(is->mouse.middle.down == TRUE) is->mouse.middle.pressed = FALSE;
        else is->mouse.middle.pressed = TRUE;
        is->mouse.middle.down = TRUE;
        is->mouse.middle.released = FALSE;
    }else{
        is->mouse.middle.down = FALSE;
        is->mouse.middle.released = TRUE;
    }
    if(state & SDL_BUTTON_X1MASK){
        if(is->mouse.x1.down == TRUE) is->mouse.x1.pressed = FALSE;
        else is->mouse.x1.pressed = TRUE;
        is->mouse.x1.down = TRUE;
        is->mouse.x1.released = FALSE;
    }else{
        is->mouse.x1.down = FALSE;
        is->mouse.x1.released = TRUE;
    }
    if(state & SDL_BUTTON_X2MASK){
        if(is->mouse.x2.down == TRUE) is->mouse.x2.pressed = FALSE;
        else is->mouse.x2.pressed = TRUE;
        is->mouse.x2.down = TRUE;
        is->mouse.x2.released = FALSE;
    }else{
        is->mouse.x2.down = FALSE;
        is->mouse.x2.released = TRUE;
    }

    
    is->buttons[BUTTON_MOUSE_LEFT]      = is->mouse.buttons[0];
    is->buttons[BUTTON_MOUSE_RIGHT]     = is->mouse.buttons[1];
    is->buttons[BUTTON_MOUSE_MIDDLE]    = is->mouse.buttons[2];
    is->buttons[BUTTON_MOUSE_X1]        = is->mouse.buttons[3];
    is->buttons[BUTTON_MOUSE_X2]        = is->mouse.buttons[4];
}



void reset_button_states(InputSystem* is){
    for(int i = 0; i < MAX_INPUT_BUTTONS; i++){
        is->buttons[i].pressed = FALSE;
        is->buttons[i].released = FALSE;
    }
}

void update_input_system(InputSystem* is){
    
    reset_button_states(is);
    update_mouse(is);

    SDL_Event e;
    
    while(SDL_PollEvent(&e)){
        
        if(e.type == SDL_KEYDOWN && !e.key.repeat){
            KeyCode kc = SCANCODE_TO_KEYCODE(is, e.key.keysym.scancode);
            if(kc == BUTTON_UNKNOWN) continue;

            is->buttons[kc].pressed = TRUE;
            is->buttons[kc].down = TRUE;
        }
        else if(e.type == SDL_KEYUP){  
            KeyCode kc = SCANCODE_TO_KEYCODE(is, e.key.keysym.scancode);
            if(kc == BUTTON_UNKNOWN) continue;

            is->buttons[kc].released = TRUE;
            is->buttons[kc].down = FALSE;
        }
        if(e.type == SDL_MOUSEMOTION){
            is->mouse.xrel = e.motion.xrel;
            is->mouse.yrel = e.motion.yrel;
        }
        if(e.type == SDL_MOUSEWHEEL){
            is->mouse.xwheel = (float)e.wheel.x;
            is->mouse.ywheel = (float)e.wheel.y;
            // printf("xwheel:%f ywheel:%f\n", is->mouse.xwheel, is->mouse.ywheel);
        }
    }

    // const uint8_t* keyboard = SDL_GetKeyboardState(NULL);

    // for(int i = 0; i < MAX_SDL_SCANCODE_TO_KEY_MAP; i++){
    //     KeyCode kc = SCANCODE_TO_KEYCODE(is, keyboard[i]);
    //     is->buttons[i].down = keyboard[i] ? TRUE : FALSE;
    // }
}


void update_sdl_event_input_system(InputSystem *is, SDL_Event e){
    // printf("window_id: %d\n", e.window.windowID);
    // printf("window_id: %d\n\n", is->window_id);
    if(e.window.windowID != is->window_id) return;

    update_mouse(is);

    if(e.type == SDL_KEYDOWN && !e.key.repeat){
        KeyCode kc = SCANCODE_TO_KEYCODE(is, e.key.keysym.scancode);
        if(kc == BUTTON_UNKNOWN) return;

        is->buttons[kc].pressed = TRUE;
        is->buttons[kc].down = TRUE;
    }
    else if(e.type == SDL_KEYUP){  
        KeyCode kc = SCANCODE_TO_KEYCODE(is, e.key.keysym.scancode);
        if(kc == BUTTON_UNKNOWN) return;

        is->buttons[kc].released = TRUE;
        is->buttons[kc].down = FALSE;
    }
    if(e.type == SDL_MOUSEMOTION){
        is->mouse.xrel = e.motion.xrel;
        is->mouse.yrel = e.motion.yrel;
    }
    if(e.type == SDL_MOUSEWHEEL){
        is->mouse.xwheel = (float)e.wheel.x;
        is->mouse.ywheel = (float)e.wheel.y;
        // printf("xwheel:%f ywheel:%f\n", is->mouse.xwheel, is->mouse.ywheel);
    }
}





void init_input_system(InputSystem* is, int window_id){
    *is = (InputSystem){0};
    is->window_id = window_id;
    // memset(is->buttons, 0, MAX_INPUT_BUTTONS*sizeof(ButtonState));
    // memset(is->bindings, 0, MAX_BINDINGS*sizeof(KeyCode));

    for(int i = 0; i < MAX_SDL_SCANCODE_TO_KEY_MAP + 1; i++){
        SCANCODE_TO_KEYCODE(is, i) = BUTTON_UNKNOWN;
    }

    // Numbers
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_0) = BUTTON_0;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_1) = BUTTON_1;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_2) = BUTTON_2;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_3) = BUTTON_3;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_4) = BUTTON_4;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_5) = BUTTON_5;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_6) = BUTTON_6;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_7) = BUTTON_7;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_8) = BUTTON_8;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_9) = BUTTON_9;

    // Letters
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_A) = BUTTON_A;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_B) = BUTTON_B;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_C) = BUTTON_C;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_D) = BUTTON_D;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_E) = BUTTON_E;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F) = BUTTON_F;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_G) = BUTTON_G;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_H) = BUTTON_H;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_I) = BUTTON_I;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_J) = BUTTON_J;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_K) = BUTTON_K;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_L) = BUTTON_L;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_M) = BUTTON_M;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_N) = BUTTON_N;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_O) = BUTTON_O;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_P) = BUTTON_P;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_Q) = BUTTON_Q;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_R) = BUTTON_R;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_S) = BUTTON_S;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_T) = BUTTON_T;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_U) = BUTTON_U;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_V) = BUTTON_V;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_W) = BUTTON_W;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_X) = BUTTON_X;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_Y) = BUTTON_Y;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_Z) = BUTTON_Z;
     
    // F's
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F1)    = BUTTON_F1;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F2)    = BUTTON_F2;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F3)    = BUTTON_F3;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F4)    = BUTTON_F4;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F5)    = BUTTON_F5;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F6)    = BUTTON_F6;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F7)    = BUTTON_F7;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F8)    = BUTTON_F8;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F9)    = BUTTON_F9;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F10)   = BUTTON_F10;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F11)   = BUTTON_F11;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_F12)   = BUTTON_F12;

    // Other
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_ESCAPE)    = BUTTON_ESCAPE;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_SPACE)     = BUTTON_SPACE;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_TAB)       = BUTTON_TAB;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_BACKSPACE) = BUTTON_BACKSPACE;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_LSHIFT)    = BUTTON_LSHIFT;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_RSHIFT)    = BUTTON_RSHIFT;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_LCTRL)     = BUTTON_LCTRL;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_RCTRL)     = BUTTON_RCTRL;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_LALT)      = BUTTON_LALT;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_RALT)      = BUTTON_RALT;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_UP)        = BUTTON_ARROW_UP;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_DOWN)      = BUTTON_ARROW_DOWN;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_LEFT)      = BUTTON_ARROW_LEFT;
    SCANCODE_TO_KEYCODE(is, SDL_SCANCODE_RIGHT)     = BUTTON_ARROW_RIGHT;
}