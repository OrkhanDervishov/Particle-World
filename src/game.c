#include "game.h"

// Time
clock_t sumTime = 0;
clock_t deltaTime = 0;

//Pragram params
int Delay = 20;
int RAD = 6;
int fps = 0;
int drawlines = 0;


int RunParticleGame(ParticleGame* game){

    // ParticleGame* pgame;
    // CreateParticleGame(&pgame);
    Window* win = game->win;
    ChunkSpace* cs = &(game->cs);

    clock_t start = 0, end = 1;
    clock_t draw_start = 0, draw_end = 0;
    clock_t sim_start = 0, sim_end = 0;
    clock_t simh_start = 0, simh_end = 0;
    // Loop

    SetChunkSpace(&(game->cs));
    StartRenderer(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    InitBasicTextRenderer();
    while(win->isrunning){
        deltaTime = end - start;
        sumTime += deltaTime;

        if(sumTime > 500){
            fps = 1000 / (end - start);
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
        start = clock();



        // Input handling
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        // SDL_Event event; 
        // SDL_PollEvent(&event);
        // if(event.type == SDL_QUIT) win->isrunning = FALSE;
        
        ProcessInput(game);


        // Rendering
        Color textColor = {.rgba = 0xFF0000FF};
        draw_start = GetTimeNano()/1000;
        ClearWindow(game->win, game->s_params.bg_color);
        DrawChunkSpace(win, cs, 0, 0);
        BasicTextRender(game->win, typeNameList[game->g_params.selectedParticleType], 10, 10, 1, textColor);
        SDL_UpdateWindowSurface(win->window);
        draw_end = GetTimeNano()/1000;

        // RenderText

        
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

        // End Frame
        SDL_Delay(DELAY);

        end = clock();
    }
    EndBasicTextRenderer();
    EndRenderer();

    printf("game running: %d\n", game->win->isrunning);

    // Destroy
    // SDL_DestroyTexture(bgt);
    // DeleteParticleGame(&game);
    return 0;
}