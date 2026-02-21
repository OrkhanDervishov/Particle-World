#include "game.h"

// Time
clock_t sumTime = 0;
clock_t deltaTime = 0;

//Pragram params
int Delay = 20;
int RAD = 6;
int fps = 0;
int drawlines = 0;

void Guide(ParticleGame* game, Color textColor);

int RunParticleGame(ParticleGame* game){

    Window* win = game->win;
    ChunkSpace* cs = &(game->cs);


    clock_t minTime = 1000/game->s_params.frameKeep;


    clock_t iter_start = 0, iter_end = 1;
    clock_t start = 0, end = 1;
    clock_t draw_start = 0, draw_end = 0;
    clock_t sim_start = 0, sim_end = 0;
    clock_t simh_start = 0, simh_end = 0;
    // Loop

    Color buttonColor = {.rgba = 0xFF0000FF};
    vec2 pos = {0, 0};
    vec2 sizes = {25, 10};
    Button* button;
    CreateButton(&button, "Hello", buttonColor, TRUE, pos, sizes, NULL);
    // PrintButtonParams(button);

    SetChunkSpace(&(game->cs));
    StartRenderer(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    InitBasicTextRenderer();
    while(win->isrunning){
        iter_start = clock();
        start = clock();
        
        ProcessInput(game);

        // Rendering
        Color textColor = {.rgba = 0xFF0000FF};
        ClearWindow(game->win, game->s_params.bg_color);
        draw_start = GetTimeNano()/1000;
        DrawChunkSpace(win, cs, 0, 0);
        draw_end = GetTimeNano()/1000;
        
        // RenderText
        char selectedStr[64];
        char fpsStr[16];
        sprintf(selectedStr, "Selected type: %s", typeNameList[game->g_params.selectedParticleType]);
        sprintf(fpsStr, "fps: %d", fps);
        BasicTextRender(game->win, selectedStr, 10, 10, 2, textColor);
        BasicTextRender(game->win, fpsStr, 10, 30, 2, textColor);
        Guide(game, textColor);
        
        // Rect rect1 = {10, 30, 100, 100};
        // Rect rect2 = {110, 30, 100, 100};
        // Rect rect3 = {210, 30, 100, 100};
        // Rect rect4 = {310, 30, 100, 100};
        // Color color1 = {.rgba = 0xFF000000};
        // Color color2 = {.rgba = 0xFFFF0000};
        // Color color3 = {.rgba = 0xFF00FF00};
        // Color color4 = {.rgba = 0xFFFFFFFF};
        // DrawFilledRect(&(win->screen), rect1, color1);
        // DrawFilledRect(&(win->screen), rect2, color2);
        // DrawFilledRect(&(win->screen), rect3, color3);
        // DrawFilledRect(&(win->screen), rect4, color4);
        // DrawButton(win, button, 10, 200);
        SDL_UpdateWindowSurface(win->window);
        


        // Simulations
        if(!game->s_params.paused){
            WallBoxCS(cs);
            
            // Particle simulation
            sim_start = GetTimeNano()/1000;
            SimulateChunkSpace(cs);
            sim_end = GetTimeNano()/1000;
            
            // Heatmap simulation
            simh_start = GetTimeNano()/1000;
            simh_end = GetTimeNano()/1000;
        }
        
        
        end = clock();
        
        
        deltaTime = end - start;
        
        
        // End Frame
        if(game->s_params.frameKeepEnabled){
            if(deltaTime < minTime){
                SDL_Delay((minTime - deltaTime));
            }
        }
        else
        SDL_Delay(game->s_params.delay);
        
        iter_end = clock();
        sumTime += deltaTime;

        if(sumTime > 500){
        fps = 1000 / (iter_end - iter_start);
        sumTime = 0;
            sprintf(win->title, "Particles: %d FPS: %d Draw time: %d Sim time: %d HeatSim time: %d Overall: %d Delay: %d", 
                    0, 
                    fps, 
                    draw_end - draw_start,
                    sim_end - sim_start,
                    simh_end - simh_start,
                    end - start,
                    DELAY
                );
            SDL_SetWindowTitle(win->window, win->title);
        }
    }
    EndBasicTextRenderer();
    EndRenderer();
    
    DeleteButton(&button);

    printf("game running: %d\n", game->win->isrunning);

    // Destroy
    // SDL_DestroyTexture(bgt);
    // DeleteParticleGame(&game);
    return 0;
}

const char* guideChangeType =       "Change particle type - V";
const char* guideCreate =           "Create particles - Mouse left";
const char* guideDelete =           "Delete particles - Mouse right";
const char* guideBrushIncrease =    "Increase brush size - 1";
const char* guideBrushDecrease =    "Decrease brush size - 2";
const char* guideGravity =          "Reverse gravity - X";
void Guide(ParticleGame* game, Color textColor){
    BasicTextRender(game->win, guideChangeType,     10, 60, 2, textColor);  
    BasicTextRender(game->win, guideCreate,         10, 80, 2, textColor);  
    BasicTextRender(game->win, guideDelete,         10, 100, 2, textColor);  
    BasicTextRender(game->win, guideBrushIncrease,  10, 120, 2, textColor);  
    BasicTextRender(game->win, guideBrushDecrease,  10, 140, 2, textColor);  
    BasicTextRender(game->win, guideGravity,        10, 160, 2, textColor);  
}