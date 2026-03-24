#include "input_system.h"

#include "particle_generation.h"

vec2 startPosMouseHold;
bool startMouseLeftHold = FALSE;
bool startMouseRightHold = FALSE;

void function1(ParticleGame* game);
void function2(ParticleGame* game);
void ClearFunction(ParticleGame* game);
void ChangeParticleType(ParticleGame* game);
void ToggleGravity(ParticleGame* game);
void BrushIncrease(ParticleGame* game);
void BrushDecrease(ParticleGame* game);
void TakeScreenShot(ParticleGame* game);
void Explode(ParticleGame *game);
void CreateLine(ParticleGame *game);
void CreateSpell(ParticleGame* game);
void ShowChunks(ParticleGame* game);

void (*MouseLeftEvent)(ParticleGame* game) = function1;
void (*MouseRightEvent)(ParticleGame* game) = function2;
void (*MouseScrollEvent)(ParticleGame* game) = NULL;
void (*SpaceButtonEvent)(ParticleGame* game) = ShowChunks;
void (*TabButtonEvent)(ParticleGame* game) = TakeScreenShot;
void (*cButtonEvent)(ParticleGame* game) = ClearFunction;
void (*vButtonEvent)(ParticleGame* game) = ChangeParticleType;
void (*xButtonEvent)(ParticleGame* game) = ToggleGravity;
void (*zButtonEvent)(ParticleGame* game) = CreateLine;
void (*rButtonEvent)(ParticleGame* game) = Explode;
void (*aButtonEvent)(ParticleGame* game) = CreateSpell;
void (*oneButtonEvent)(ParticleGame* game) = BrushIncrease;
void (*twoButtonEvent)(ParticleGame* game) = BrushDecrease;


vec2 GetMousePos(){
    vec2 mousepos;
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    return mousepos;
}

vec2 GetMousePosInCS(){
    vec2 mousepos;
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    mousepos.x = ((mousepos.x) / DEFAULT_PARTICLE_SIZE);
    mousepos.y = ((mousepos.y) / DEFAULT_PARTICLE_SIZE);
    return mousepos;
}



void ProcessInput(ParticleGame* game)
{
    SDL_StopTextInput();

    Window* win = game->win;
    SDL_Event event;
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);

    
    // Particle generation and GUI Interraction
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)) MouseLeftEvent(game);
    if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) MouseRightEvent(game);


    // Other events
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) win->isrunning = 0;
        
        if(event.type == SDL_MOUSEBUTTONUP){
            if(event.button.button == SDL_BUTTON_LEFT){
                startMouseLeftHold = FALSE;
            }
            // It does not work(at least for now)
            if(event.button.button == SDL_BUTTON_RIGHT){
                startMouseRightHold = FALSE;
            }
        }

        if(event.type == SDL_KEYUP){
            if(QUIT_BUTTON) win->isrunning = 0;
            
            if(SCREEN_CLEAR_BUTTON){
                cButtonEvent(game);
            }
            if(PARTICLE_CHANGE_BUTTON){
                vButtonEvent(game);
            }
            if(X_BUTTON){
                xButtonEvent(game);
            }
            if(PAUSE_OF_BUTTON){
                game->s_params.paused = game->s_params.paused ? 0 : 1;
            }
            if(HEATMAP_OF_BUTTON){
                game->s_params.hm_mode = game->s_params.hm_mode ? 0 : 1;
            }
            if(R_BUTTON){
                rButtonEvent(game);
            }
            if(Z_BUTTON){
                zButtonEvent(game);
            }
            if(A_BUTTON){
                aButtonEvent(game);
            }
            if(BRUSH_INCREASE_BUTTON) oneButtonEvent(game);
            if(BRUSH_DECREASE_BUTTON) twoButtonEvent(game);
            // if(DELAY_INCREASE_BUTTON) SpeedUpParticleSimulator();
            // if(DELAY_DECREASE_BUTTON) SlowDownParticleSimulator();
            if(TAB_BUTTON) TabButtonEvent(game); 
            if(SPACE_BUTTON) SpaceButtonEvent(game);
        }
    }
}

void function1(ParticleGame* game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    // if(!CheckGuiButtons(game->win, mx, my)){
        int created = 0;
        int px = ((mx) / DEFAULT_PARTICLE_SIZE);
        int py = ((my) / DEFAULT_PARTICLE_SIZE);

        if(startMouseLeftHold == FALSE){
            startMouseLeftHold = TRUE;
            startPosMouseHold = GetMousePosInCS();
        } else {
            vec2 endPosMouseHold = GetMousePosInCS();
            CreationLineCS(
                &(game->cs), startPosMouseHold.x, startPosMouseHold.y, 
                endPosMouseHold.x, endPosMouseHold.y, 
                game->g_params.brush_size, game->g_params.selectedParticleType
            );
            
            startPosMouseHold = endPosMouseHold;
        }
}

void function2(ParticleGame* game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    int px = ((mx) / DEFAULT_PARTICLE_SIZE);
    int py = ((my) / DEFAULT_PARTICLE_SIZE);
    
    if(startMouseRightHold == FALSE){
        startMouseRightHold = TRUE;
        startPosMouseHold = GetMousePosInCS();
    } else {
        int bs = game->g_params.brush_size;
        vec2 endPosMouseHold = GetMousePosInCS();
        DeletionLineCS(
            &(game->cs), startPosMouseHold.x, startPosMouseHold.y, 
            endPosMouseHold.x, endPosMouseHold.y, 
            game->g_params.brush_size
        );
        
        startPosMouseHold = endPosMouseHold;
    }
}


void ClearFunction(ParticleGame* game){
    ClearFullCS(&game->cs);
}

void ChangeParticleType(ParticleGame* game){
    game->g_params.selectedParticleType++;
    game->g_params.selectedParticleType %= GetParticleTypeCount();
}

void ToggleGravity(ParticleGame* game){
    gravity = gravity == 1 ? -1 : 1;
}

void BrushIncrease(ParticleGame* game){
    if(game->g_params.brush_size >= 20) return;
    game->g_params.brush_size++;
}

void BrushDecrease(ParticleGame* game){
    if(game->g_params.brush_size <= 1){
        game->g_params.brush_size = 1;
        return;
    }
    game->g_params.brush_size--;
}

void TakeScreenShot(ParticleGame* game){
    Image img;  
    img.buffer = NULL;
    create_image(&img, game->win->context.width, game->win->context.height);
    fimage_to_image(game->win->context, img);
    char path[64];
    // char filename[64];
    char path_log[96];
    time_t t;
    t = time(&t);
    snprintf(path, 64, "./screenshots/%d.png", t);
    save_image_png(&img, path);
    // printf("works\n");
    // Color info_color = {.rgba = INFO_TEXT_COLOR};
    // Color message_color = {.rgba = 0xFFFFFFFF};
    // snprintf(path_log, 96, "image saved in %s", path);
    // BasicTextRender(game->win, "[INFO] ", 10, 300, 2, message_color);
    // BasicTextRender(game->win, path_log, 40, 300, 2, message_color);
    // printf("%s\n", path_log);
}

void Explode(ParticleGame *game){
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    int px = ((mx) / DEFAULT_PARTICLE_SIZE);
    int py = ((my) / DEFAULT_PARTICLE_SIZE);
    Explosion(&(game->cs), px, py, game->g_params.brush_size*2, 1000, FIRE_SMOKE);
}

bool startLine = FALSE;
Pos startPos;
void mouse_points_draw_line(ParticleGame *game){
    if(startLine == FALSE) return;
    vec2 endPos = GetMousePosInCS();

    Color color = {.rgba = 0xFF00FF00};
    draw_line_f(
        game->win->context, color, 
        startPos.x*DEFAULT_PARTICLE_SIZE, startPos.y*DEFAULT_PARTICLE_SIZE, 
        endPos.x*DEFAULT_PARTICLE_SIZE, endPos.y*DEFAULT_PARTICLE_SIZE
    );
}

int cb_index_line = -1;
void CreateLine(ParticleGame *game){

    if(startLine == FALSE){
        startLine = TRUE;
        startPos = GetMousePosInCS();
        cb_index_line = add_callback_pg(game, mouse_points_draw_line);
    } else {
        delete_callback_pg(game, cb_index_line);
        cb_index_line = -1;
        startLine = FALSE;
        vec2 endPos = GetMousePosInCS();
        CreationLineCS(
            &(game->cs), startPos.x, startPos.y, endPos.x, endPos.y, 
            game->g_params.brush_size, game->g_params.selectedParticleType
        );
    }
}

void CreateSpell(ParticleGame* game){
    vec2 pos = GetMousePosInCS();

    Image spellImage;
    spellImage.buffer = NULL;
    load_png(&spellImage, "./resources/spell2.png");
    ParticleImage pimg = GenerateMagicFromImage(spellImage, DEFAULT_PARTICLE_SIZE/4, 0xFFFFFFFF);
    
    PutParticleImageCS(&(game->cs), pos.x - pimg.width/2, pos.y - pimg.height/2, 1, pimg);
    
    DestroyParticleImage(pimg);
    delete_image(&spellImage);
}

bool chunk_show = FALSE;
ParticleGame* this_game;
void show_chunks(){
    ShowChunkSpaceAllSW(this_game->win, &(this_game->cs), 0, 0);
}

int cb_index_chunks = -1;
void ShowChunks(ParticleGame* game){
    if(chunk_show){
        delete_callback_pg(game, cb_index_chunks);
        cb_index_chunks = -1;
        chunk_show = FALSE;
    } else {
        this_game = game;
        cb_index_chunks = add_callback_pg(game, show_chunks);
        chunk_show = TRUE;
    }
}