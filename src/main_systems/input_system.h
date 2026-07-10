#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "sdl2_layer.h"
#include "timing.h"

typedef enum{
    BUTTON_UNKNOWN = 0,
    BUTTON_MOUSE_LEFT = 1,
    BUTTON_MOUSE_RIGHT,
    BUTTON_MOUSE_MIDDLE,
    BUTTON_MOUSE_X1,
    BUTTON_MOUSE_X2,
    BUTTON_SPACE,
    BUTTON_ENTER,
    BUTTON_TAB,
    BUTTON_LSHIFT,
    BUTTON_RSHIFT,
    BUTTON_LCTRL,
    BUTTON_RCTRL,
    BUTTON_LALT,
    BUTTON_RALT,
    BUTTON_BACKSPACE,
    BUTTON_ESCAPE,
    BUTTON_ARROW_UP,
    BUTTON_ARROW_DOWN,
    BUTTON_ARROW_LEFT,
    BUTTON_ARROW_RIGHT,
    BUTTON_F1,
    BUTTON_F2,
    BUTTON_F3,
    BUTTON_F4,
    BUTTON_F5,
    BUTTON_F6,
    BUTTON_F7,
    BUTTON_F8,
    BUTTON_F9,
    BUTTON_F10,
    BUTTON_F11,
    BUTTON_F12,
    BUTTON_0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_A,
    BUTTON_B,
    BUTTON_C,
    BUTTON_D,
    BUTTON_E,
    BUTTON_F,
    BUTTON_G,
    BUTTON_H,
    BUTTON_I,
    BUTTON_J,
    BUTTON_K,
    BUTTON_L,
    BUTTON_M,
    BUTTON_N,
    BUTTON_O,
    BUTTON_P,
    BUTTON_Q,
    BUTTON_R,
    BUTTON_S,
    BUTTON_T,
    BUTTON_U,
    BUTTON_V,
    BUTTON_W,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_Z,
} KeyCode;


typedef uint32_t action_t;

typedef struct{
    bool down;
    bool pressed;
    bool released;
} ButtonState;

typedef struct{
    KeyCode key;
    action_t action;
} KeyBinding;

#define MAX_KEYS_FOR_ACTION 4
typedef struct{
    KeyCode list[MAX_KEYS_FOR_ACTION];
    uint8_t count;
} KeyList;

typedef struct{
    float x, y;
    float xprev, yprev;
    float dx, dy;
    float xrel, yrel;

    float xwheel, ywheel;


    union{
        struct{
            ButtonState left;
            ButtonState right;
            ButtonState middle;
            ButtonState x1;
            ButtonState x2;
        };
        ButtonState buttons[5];
    };
} MouseInput;

#define MAX_INPUT_BUTTONS 1024
#define MAX_BINDINGS 4096
#define MAX_SDL_SCANCODE_TO_KEY_MAP 512
typedef struct{
    int window_id;
    ButtonState     buttons[MAX_INPUT_BUTTONS];   // KeyCodes are indices
    KeyList         bindings[MAX_BINDINGS]; // Actions are indices
    KeyCode         scancode_keycode_map[MAX_SDL_SCANCODE_TO_KEY_MAP];
    
    MouseInput      mouse;
    // Will not be used probably
    // KeyBinding   bindings[MAX_BINDINGS]; // Must be iterated to check
} InputSystem;

#define SCANCODE_TO_KEYCODE(in_sys_p, scancode) (in_sys_p)->scancode_keycode_map[(scancode)]

void init_input_system(InputSystem* is, int window_id);
void update_input_system(InputSystem* is);
void add_binding(InputSystem* is, KeyCode keycode, action_t action);
void remove_binding(InputSystem* is, KeyCode keycode, action_t action);
bool button_down(InputSystem* is, KeyCode keycode);
bool button_pressed(InputSystem* is, KeyCode keycode);
bool button_released(InputSystem* is, KeyCode keycode);
bool action_down(InputSystem* is, action_t action);
bool action_pressed(InputSystem* is, action_t action);
bool action_released(InputSystem* is, action_t action);
void update_mouse(InputSystem* is);

void reset_button_states(InputSystem* is);
void update_sdl_event_input_system(InputSystem *is, SDL_Event event, bool mouse_update);

// #undef MAX_BUTTONS
// #undef MAX_BINDINGS
#endif //INPUT_SYSTEM_H