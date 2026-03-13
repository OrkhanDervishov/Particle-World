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
void call_all_callbacks(ParticleGame* game){
    for(int i = 0; i < game->cbCount; i++){
        game->callbacks[i](game);
    }
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
    // Loop

    Color buttonColor = {.rgba = 0xFF0000FF};
    vec2 pos = {0, 0};
    vec2 sizes = {50, 25};
    Button* button;
    CreateButton(&button, "Hello", buttonColor, TRUE, pos, sizes, NULL);
    // PrintButtonParams(button);

    // Create Objects
    Image spellImage;
    spellImage.buffer = NULL;
    load_png(&spellImage, "./resources/spell2.png");
    ParticleImage pimg = GenerateMagicFromImage(spellImage, DEFAULT_PARTICLE_SIZE, 0xFFFFFFFF);
    

    char fpstext[64];
    char typetext[64];
    char brushtext[64];

    SetChunkSpace(&(game->cs));
    StartChunkRendererSW(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    Color textColor = {.r=255, .g=0, .b=0, .a=255};
    InitBasicTextRenderer();
    while(win->isrunning){
        iter_start = clock();
        start = clock();
        
        ProcessInput(game);

        // Rendering
        fill_f(win->context, game->s_params.bg_color);;
        draw_start = GetTimeNano()/1000;
        DrawChunkSpaceSW(win, cs, 0, 0);
        
        // RenderText
        BasicTextRender(game->win, fpstext,     10, 10, 2, textColor);  
        BasicTextRender(game->win, typetext,    10, 30, 2, textColor);  
        BasicTextRender(game->win, brushtext,   10, 50, 2, textColor);  
        Guide(game, textColor);
        
        // Call ParticleGame callbacks
        call_all_callbacks(game);
        
        // DrawButton(win, button, 10, 500);
        SDL_UpdateWindowSurface(win->window);
        draw_end = GetTimeNano()/1000;
        

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
                    DELAY
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
    
    // Delete Objects
    DestroyParticleImage(pimg);
    delete_image(&spellImage);

    DeleteButton(&button);

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
}


