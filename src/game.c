#include "game.h"

// Time
clock_t sumTime = 0;
clock_t deltaTime = 0;

//Pragram params
int Delay = 20;
int RAD = 6;
float fps = 0;
int drawlines = 0;


void Guide(ParticleGame* game, Color textColor);
void init_buttons(ParticleGame* game, Button** buttons);

bool cursor_enabled = FALSE;
void toggle_cursor(ParticleGame* game){
    cursor_enabled = cursor_enabled ? FALSE : TRUE;
}

bool guide_enabled = FALSE;
void toggle_guide(ParticleGame* game){
    guide_enabled = guide_enabled ? FALSE : TRUE;
}

void call_all_callbacks(ParticleGame* game){
    for(int i = 0; i < CB_COUNT_MAX; i++){
        if(game->callbacks[i] != NULL)
            game->callbacks[i](game);
    }
}

void func(){
    printf("Hello\n");
}

void clear_space(ParticleGame* game){
    ClearFullCS(&game->cs);
}

void select_sand(ParticleGame* game){
    game->g_params.selectedParticleType = SAND;
}
void select_water(ParticleGame* game){
    game->g_params.selectedParticleType = WATER;
}
void select_steam(ParticleGame* game){
    game->g_params.selectedParticleType = STEAM;
}
void select_acid(ParticleGame* game){
    game->g_params.selectedParticleType = ACID;
}
void select_wood(ParticleGame* game){
    game->g_params.selectedParticleType = WOOD;
}
void select_wall(ParticleGame* game){
    game->g_params.selectedParticleType = WALL;
}
void select_fire(ParticleGame* game){
    game->g_params.selectedParticleType = FIRE;
}
void select_fire_smoke(ParticleGame* game){
    game->g_params.selectedParticleType = FIRE_SMOKE;
}
void select_fire_liquid(ParticleGame* game){
    game->g_params.selectedParticleType = FIRE_LIQUID;
}
void select_smoke(ParticleGame* game){
    game->g_params.selectedParticleType = SMOKE;
}
void select_coal(ParticleGame* game){
    game->g_params.selectedParticleType = COAL;
}
void select_powder(ParticleGame* game){
    game->g_params.selectedParticleType = POWDER;
}
void select_oil(ParticleGame* game){
    game->g_params.selectedParticleType = OIL;
}
void select_lava(ParticleGame* game){
    game->g_params.selectedParticleType = LAVA;
}
void select_fungus(ParticleGame* game){
    game->g_params.selectedParticleType = FUNGUS;
}
void select_obsidian(ParticleGame* game){
    game->g_params.selectedParticleType = OBSIDIAN;
}
void select_phantom(ParticleGame* game){
    game->g_params.selectedParticleType = PHANTOM;
}
void select_source(ParticleGame* game){
    game->g_params.selectedParticleType = SOURCE;
}



int RunParticleGame(ParticleGame* game){
    
    Window* win = game->win;
    ChunkSpace* cs = &(game->cs);
    
    clock_t minTime = 1000/game->s_params.frameLock;


    clock_t iter_start = 0, iter_end = 1;
    clock_t start = 0, end = 1;
    clock_t draw_start = 0, draw_end = 0;
    clock_t sim_start = 0, sim_end = 0;
    clock_t simh_start = 0, simh_end = 0;
    
    Color buttonColor = {.rgba = 0xFF0000FF};
    vec2 pos = {10, win->h -50};
    vec2 pos2 = {65, win->h -50};
    vec2 sizes = {50, 25};

    Button* buttons[100];
    init_buttons(game, buttons);
    
    // Create Objects
    Image final_image;
    Image part_map;
    Image light_map;
    Image blurred;
    final_image.buffer = NULL;
    part_map.buffer = NULL;
    light_map.buffer = NULL;
    blurred.buffer = NULL;
    create_image(&final_image, win->context.width/DEFAULT_PARTICLE_SIZE, win->context.height/DEFAULT_PARTICLE_SIZE);
    create_similar(&part_map, final_image);
    create_similar(&light_map, final_image);
    create_similar(&blurred, final_image);

    char fpstext[64];
    char typetext[64];
    char brushtext[64];
    SetChunkSpace(&(game->cs));
    InitGuiRenderer();
    start_particle_lighting_sw(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    StartChunkRendererSW(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    // start_particle_lighting_sw(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, 1);
    // StartChunkRendererSW(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, 1);
    InitBasicTextRenderer();
    Color textColor = {.r=255, .g=0, .b=0, .a=255};
    Color textColor2 = {.r=0, .g=255, .b=0, .a=255};
    // Loop
    while(win->isrunning){
        update_global_time();
        iter_start = clock();
        start = clock();
        
        ProcessInput(game);
        
        // Rendering
        
        draw_start = GetTimeNano()/1000;
        fill_f(win->context, (Color){.rgba = 0xFFFFFFFF});
        fill_image(final_image, game->s_params.bg_color);
        fill_image(part_map, (Color){.rgba=0xFF000000});
        fill_image(light_map, (Color){.rgba=0xFF000000});
        DrawChunkSpaceSW(part_map, cs, 0, 0);
        draw_cs_lightmap(light_map, cs, 0, 0);
        // blur_lightmap2(&blurred, light_map, 3);
        blur_lightmap_strong(&blurred, light_map, 9, 1);
        // save_image_png(&blurred, "lightmap.png");
        additive_blend(final_image, part_map);
        additive_blend(final_image, blurred);
        // printf("works\n");
        // printf("xf:%f yf:%f\n", game->camera.pos.x, game->camera.pos.y);
        draw_image_on_fimage_scaled(win->context, final_image, (int)game->camera.pos.x, (int)game->camera.pos.y, DEFAULT_PARTICLE_SIZE, DEFAULT_PARTICLE_SIZE);
        // draw_image_on_fimage_scaled(win->context, final_image, -10, 0, DEFAULT_PARTICLE_SIZE, DEFAULT_PARTICLE_SIZE);

        // Call ParticleGame callbacks
        call_all_callbacks(game);
        
        Color mouse_color = {.rgba = 0xFFFFFFFF};
        int mx, my;
        int state = SDL_GetMouseState(&mx, &my);
        int px = mx / DEFAULT_PARTICLE_SIZE;
        int py = my / DEFAULT_PARTICLE_SIZE;
        DrawGuiElement(win, &game->gui, 0, 0);
        draw_circle_f(win->context, mx, my, game->g_params.brush_size*DEFAULT_PARTICLE_SIZE, mouse_color, 2);
        Rect rect = {mx-1, my-1, 2, 2};
        draw_filled_rect_f(win->context, rect, mouse_color);
        char pointer_text[64];
        if(px >= 0 && px < cs->width_p && py >= 0 && py < cs->height_p){
            sprintf(pointer_text, "%s %d, %d", typeNameList[CS_GET_TYPE(cs, px, py)], px, py);
            BasicTextRender(game->win, pointer_text, mx+5, my, 1, textColor2);
        }  

        // RenderText
        BasicTextRender(game->win, fpstext,     10, 10, 2, textColor);  
        BasicTextRender(game->win, typetext,    10, 30, 2, textColor);  
        BasicTextRender(game->win, brushtext,   10, 50, 2, textColor); 
        if(guide_enabled) Guide(game, textColor);
        
        if(cursor_enabled) draw_cursor(win->context, game->mouse);

        SDL_UpdateWindowSurface(win->window);
        draw_end = GetTimeNano()/1000;
        
        // Simulations
        if(!game->s_params.paused){
            WallBoxCS(cs);
            
            // Particle simulation
            sim_start = GetTimeNano()/1000;
            SimulateChunkSpace(cs);
            RefreshChunkSpace(cs);
            sim_end = GetTimeNano()/1000;
            
            // Heatmap simulation
            simh_start = GetTimeNano()/1000;
            simh_end = GetTimeNano()/1000;
        }
        
        
        end = clock();
        
        
        deltaTime = end - start;
        
        
        // End Frame
        if(game->s_params.frameLockEnabled){
            if(deltaTime < minTime){
                SDL_Delay((minTime - deltaTime));
            }
        }
        else
        SDL_Delay(game->s_params.delay);
        
        iter_end = clock();
        sumTime += deltaTime;

        if(sumTime > 200){
        fps = 1000.0f / ((float)iter_end - (float)iter_start);
        sumTime = 0;
            sprintf(win->title, "Particles: %d FPS: %,0f Draw time: %d Sim time: %d HeatSim time: %d Overall: %d Delay: %d", 
                    0, 
                    fps, 
                    draw_end - draw_start,
                    sim_end - sim_start,
                    simh_end - simh_start,
                    end - start,
                    game->s_params.delay
                );
            SDL_SetWindowTitle(win->window, win->title);
            sprintf(fpstext, "fps: %.0f / draw_time: %.2fms / sim_time: %.2fms", 
                fps, (float)(draw_end - draw_start)/1000.f, (float)(sim_end - sim_start)/1000.f);
        }
        sprintf(typetext, "selected type: %s", typeNameList[game->g_params.selectedParticleType]);
        sprintf(brushtext, "brush radius: %d", game->g_params.brush_size);
    }

    EndBasicTextRenderer();
    EndChunkRendererSW();
    end_particle_lighting_sw();
    
    // Delete Objects

    // DeleteButton(&button1);
    // DeleteButton(&button2);

    printf("game running: %d\n", game->win->isrunning);
    return 0;
}






const char* guideChangeType =       "Change particle type - V";
const char* guideCreate =           "Create particles - Mouse left";
const char* guideDelete =           "Delete particles - Mouse right";
const char* guideClear =            "Delete all particles - C";
const char* guideBrushIncrease =    "Increase brush size - 1";
const char* guideBrushDecrease =    "Decrease brush size - 2";
const char* guideGravity =          "Reverse gravity - X";
const char* guideLine =             "Create particles along a line - Z";
const char* guideScreenshot =       "Take screenshot - TAB";
const char* guideSpell =            "Cast spell - A";
const char* guideChunks =           "Show chunks - SPACE";
const char* guideExit =             "Exit - ESC";
void Guide(ParticleGame* game, Color textColor){
    // int startX = game->win->screen.width-350;
    int startX = 10;
    int startY = 100;
    BasicTextRender(game->win, guideChangeType,     startX, startY, 2, textColor);  
    BasicTextRender(game->win, guideCreate,         startX, startY+20, 2, textColor);  
    BasicTextRender(game->win, guideDelete,         startX, startY+40, 2, textColor);  
    BasicTextRender(game->win, guideClear,          startX, startY+60, 2, textColor);  
    BasicTextRender(game->win, guideBrushIncrease,  startX, startY+80, 2, textColor);  
    BasicTextRender(game->win, guideBrushDecrease,  startX, startY+100, 2, textColor);  
    BasicTextRender(game->win, guideGravity,        startX, startY+120, 2, textColor);  
    BasicTextRender(game->win, guideLine,           startX, startY+140, 2, textColor);  
    BasicTextRender(game->win, guideScreenshot,     startX, startY+160, 2, textColor);  
    BasicTextRender(game->win, guideSpell,          startX, startY+180, 2, textColor);  
    BasicTextRender(game->win, guideChunks,         startX, startY+200, 2, textColor);  
    BasicTextRender(game->win, guideExit,           startX, startY+220, 2, textColor);  
}


void init_buttons(ParticleGame* game, Button** buttons){
    Color buttonColor = {.rgba = 0xFF0000FF};
    vec2 sizes = {70, 30};

    CreateButton(&buttons[0],   "Clear",      buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*0,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)clear_space);
    CreateButton(&buttons[18],  "Cursor",     buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*1,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)toggle_cursor);
    CreateButton(&buttons[19],  "Guide",      buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*2,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)toggle_guide);
    
    CreateButton(&buttons[1],   "Sand",       typeColorList[SAND][0],           TRUE, (Pos){10+(sizes.x+5)*0,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_sand);
    CreateButton(&buttons[2],   "Water",      typeColorList[WATER][0],          TRUE, (Pos){10+(sizes.x+5)*1,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_water);
    CreateButton(&buttons[3],   "Steam",      typeColorList[STEAM][0],          TRUE, (Pos){10+(sizes.x+5)*2,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_steam);
    CreateButton(&buttons[4],   "Acid",       typeColorList[ACID][0],           TRUE, (Pos){10+(sizes.x+5)*3,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_acid);
    CreateButton(&buttons[5],   "Wood",       typeColorList[WOOD][0],           TRUE, (Pos){10+(sizes.x+5)*4,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_wood);
    CreateButton(&buttons[6],   "Wall",       typeColorList[WALL][0],           TRUE, (Pos){10+(sizes.x+5)*5,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_wall);
    CreateButton(&buttons[7],   "Fire",       typeColorList[FIRE][0],           TRUE, (Pos){10+(sizes.x+5)*6,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire);
    CreateButton(&buttons[8],   "Fire Smoke", typeColorList[FIRE_SMOKE][0],     TRUE, (Pos){10+(sizes.x+5)*7,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire_smoke);
    CreateButton(&buttons[9],   "Fire Liquid",typeColorList[FIRE_LIQUID][0],    TRUE, (Pos){10+(sizes.x+5)*8,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire_liquid);
    CreateButton(&buttons[10],   "Smoke",     typeColorList[SMOKE][0],          TRUE, (Pos){10+(sizes.x+5)*9,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_smoke);
    CreateButton(&buttons[11],  "Coal",       typeColorList[COAL][0],           TRUE, (Pos){10+(sizes.x+5)*10,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_coal);
    CreateButton(&buttons[12],  "Powder",     typeColorList[POWDER][0],         TRUE, (Pos){10+(sizes.x+5)*11,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_powder);
    CreateButton(&buttons[13],  "Oil",        typeColorList[OIL][0],            TRUE, (Pos){10+(sizes.x+5)*12,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_oil);
    CreateButton(&buttons[14],  "Lava",       typeColorList[LAVA][0],           TRUE, (Pos){10+(sizes.x+5)*13,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_lava);
    CreateButton(&buttons[15],  "Fungus",     typeColorList[FUNGUS][0],         TRUE, (Pos){10+(sizes.x+5)*14,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fungus);
    CreateButton(&buttons[16],  "Phantom",    typeColorList[PHANTOM][0],        TRUE, (Pos){10+(sizes.x+5)*15,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_phantom);
    CreateButton(&buttons[17],  "Source",     typeColorList[SOURCE][0],         TRUE, (Pos){10+(sizes.x+5)*16,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_source);
    
    // PrintButtonParams(buttons[0]);
    for(int i = 0; i < 20; i++){
        add_button_gui(&game->gui, buttons[i], 0.2f);
    }
    // add_button_gui(&game->gui, button2);
}