#include "particle_game.h"


// Time
clock_t sumTime = 0;
clock_t deltaTime = 0;

//Pragram params
int Delay = 20;
int RAD = 6;
int fps = 0;
int drawlines = 0;

int CreateParticleGame(ParticleGame** game){
    (*game) = (ParticleGame*)malloc(sizeof(ParticleGame));
    if(CreateWindow(&((*game)->win), SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, FALSE)) return 1;

    ChunkSpace* cs = &((*game)->cs);
    if(CreateChunkSpace(
        cs, CS_WIDTH, CS_HEIGHT, 
        DEFAULT_REGION_WIDTH, DEFAULT_REGION_HEIGHT,
        DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE
    )) return 2;
    ArrangeChunks(cs);
    // CreateGui((*game)->win, buttons, &buttonCount, countParticleTypes - 1);
    // TODO: Solve problem
    InitParticles();
    srand(time(NULL));
    
    (*game)->bg_color.rgba = 0xFFFFFFFF;
    (*game)->hm_mode = FALSE;
    (*game)->paused = FALSE;
    return 0;
}

void DeleteParticleGame(ParticleGame** game){
    CONSOLE("delete world\n");
    DeleteChunkSpace(&(*game)->cs);
    CONSOLE("delete window\n");
    DestroyWindow(&((*game)->win));
    CONSOLE("delete game\n");
    free(*game);
}

SDL_Texture* GetTexture(SDL_Renderer* renderer, SDL_Texture* bgt, char* path){
    SDL_Surface* bgs = SDL_LoadBMP(BG_PATH);
    SDL_Rect bgrect;
    bgrect.x = 0;
    bgrect.y = 0;

    // TODO: Give screen width and height
    bgrect.w = 700;
    bgrect.h = 700;
    if(bgs == NULL) printf("background fail\n");
    else {
        bgt = SDL_CreateTextureFromSurface(renderer, bgs);
        return bgt;
    }
    SDL_FreeSurface(bgs);
}





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

    Color clearColor = {.rgba = 0xFF181818};
    SetChunkSpace(&(game->cs));
    StartRenderer(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
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
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) win->isrunning = FALSE;
        // ProcessInput(game);


        // Rendering
        draw_start = GetTimeNano()/1000;
        ClearWindow(game->win, clearColor);
        DrawChunkSpace(win, cs, -DEFAULT_CHUNK_SIZE, -DEFAULT_CHUNK_SIZE);
        SDL_UpdateWindowSurface(win->window);
        draw_end = GetTimeNano()/1000;


        
        // Simulations
        if(!game->paused){

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
    EndRenderer();

    // Destroy
    // SDL_DestroyTexture(bgt);
    // DeleteParticleGame(&game);
    return 0;
}



// Initialization



// Gameplay

// void CreateManyParticles(World* world, int px, int py, int rad, int t){
    // int begy = py - rad;
    // int begx = px - rad;
    // int endy = py + rad;
    // int endx = px + rad;
    // int srad = rad * rad;

    // for(int i = begy; i < endy; i++){
    //     int di = abs(py - i);
    //     for(int j = begx; j < endx; j++){
    //         int dj = abs(px - j);
    //         if(di*di + dj*dj < srad)
    //             if(j >= 0 && i >= 0 && j < sim->cols && i < sim->rows)
    //                 if(sim->pMap[i][j].id < 0)
    //                     CreateReplaceParticle(sim, j, i, t);
    //     }
    // }
// }

// void CreateManyParticlesWithRarity(World* world, int px, int py, int rad, int t, int rarity){
    // int begy = py - rad;
    // int begx = px - rad;
    // int endy = py + rad;
    // int endx = px + rad;
    // int srad = rad * rad;

    // for(int i = begy; i < endy; i++){
    //     int di = abs(py - i);
    //     for(int j = begx; j < endx; j++){
    //         int dj = abs(px - j);
    //         if(di*di + dj*dj < srad){
    //             if(rand() % rarity != 1) continue;
    //             if(j >= 0 && i >= 0 && j < sim->cols && i < sim->rows)
    //                 if(sim->pMap[i][j].id < 0){
    //                     CreateReplaceParticle(sim, j, i, t);
    //                 }
    //         }
    //     }
    // }
// }

// void DeleteManyParticles(World* world, int px, int py, int rad){
    // int begy = py - rad;
    // int begx = px - rad;
    // int endy = py + rad;
    // int endx = px + rad;

    // for(int i = begy; i < endy; i++){
    //     if(i <= 0 || i >= sim->rows - 1) continue;
    //     int di = abs(py - i);
    //     for(int j = begx; j < endx; j++){
    //         if(j <= 0 || j >= sim->cols - 1) continue;
    //         int dj = abs(px - j);
    //         if(di*di + dj*dj < rad*rad)
    //             DeleteParticle(sim, j, i);
    //     }
    // }
// }


// ParticleSimulator
// void WallBox(World* world){
    
    // for(int j = 0; j < sim->cols; j++){
    //     CreateParticle(sim, j, 0, WALL);
    // }
    // for(int j = 0; j < sim->cols; j++){
    //     CreateParticle(sim, j, sim->rows - 1, WALL);
    // }
    // for(int i = 1; i < sim->rows - 1; i++){
    //     CreateParticle(sim, 0, i, &c, WALL);
    //     CreateParticle(sim, sim->cols - 1, i, WALL);
    // }

// }

// ParticleSimulator
// void Explosion(World* world, int px, int py, int rad, int power, int replaceWith){
    // int x = rad - 1;
    // int y = 0;
    // int dx = 1;
    // int dy = 1;
    // int err = dx - (rad << 1);

    // while(x >= y){
    //     int cx = x;
    //     int cy = y;
    //     DestructionLine(sim, px, py, px + cx, py + cy, power, replaceWith);
    //     DestructionLine(sim, px, py, px + cy, py + cx, power, replaceWith);
    //     DestructionLine(sim, px, py, px - cy, py + cx, power, replaceWith);
    //     DestructionLine(sim, px, py, px - cx, py + cy, power, replaceWith);
    //     DestructionLine(sim, px, py, px - cx, py - cy, power, replaceWith);
    //     DestructionLine(sim, px, py, px - cy, py - cx, power, replaceWith);
    //     DestructionLine(sim, px, py, px + cy, py - cx, power, replaceWith);
    //     DestructionLine(sim, px, py, px + cx, py - cy, power, replaceWith);

    //     if(err <= 0){
    //         y++;
    //         err += dy;
    //         dy += 2;  
    //     }
    //     if(err > 0){
    //         x--;
    //         dx += 2;
    //         err += dx - (rad << 1);
    //     }
    // }
// }

// ParticleSimulator
// void DestructionLine(World* world, int px0, int py0, int px1, int py1, int power, int replaceWith){
    // int dx = abs(px1 - px0);
    // int sx = px0 < px1 ? 1 : -1;
    // int dy = -abs(py1 - py0);
    // int sy = py0 < py1 ? 1 : -1;
    // int error = dx + dy;

    // while(1){
    //     if(px0 >= 0 && py0 >= 0 && px0 < sim->cols && py0 < sim->rows){
    //         if(sim->pMap[py0][px0].type == WALL) break;
    //         CreateReplaceParticle(sim, px0, py0, replaceWith);
    //         sim->pMap[py0][px0].heat = power * 10;
    //         if (px0 == px1 && py0 == py1) break;
    //         int e2 = 2 * error;
    //         if(e2 >= dy){
    //             if(px0 == px1) break;
    //             error += dy;
    //             px0 += sx;
    //         }
    //         if(e2 <= dx){
    //             if(py0 == py1) break;
    //             error += dx;
    //             py0 += sy;
    //         }
    //     }
    //     else{
    //         break;
    //     }
    // }

    // // int dx = abs(px1 - px0);
    // // int sx = px0 < px1 ? 1 : -1;
    // // int dy = -abs(py1 - py0);
    // // int sy = py0 < py1 ? 1 : -1;
    // // int error = dx + dy;

    // // Color c;
    // // ChangeColor(&c, FIRE_COLORS);

    // // while (1) {
    // //     CreateReplaceParticle(sim, px0, py0, &c, FIRE);

    // //     if (px0 == px1 && py0 == py1) break;

    // //     int e2 = 2 * error;

    // //     if (e2 >= dy) {
    // //         error += dy;
    // //         px0 += sx;
    // //     }

    // //     if (e2 <= dx) {
    // //         error += dx;
    // //         py0 += sy;
    // //     }
    // // }
// }
