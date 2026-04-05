#include "particle_game.h"



#define CS_WIDTH    DEFAULT_CS_WIDTH
#define CS_HEIGHT   DEFAULT_CS_HEIGHT

#define REGION_WIDTH DEFAULT_REGION_WIDTH
#define REGION_HEIGHT DEFAULT_REGION_HEIGHT


int CreateParticleGame(ParticleGame** game){
    //******************************************/
    // Initializing game window
    (*game) = (ParticleGame*)malloc(sizeof(ParticleGame));
    if(CreateWindow(&((*game)->win), SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, TRUE)) return 1;
    //******************************************/
    
    //******************************************/
    // Initializing chunk system
    ChunkSpace* cs = &((*game)->cs);
    if(CreateChunkSpace(
        cs, CS_WIDTH, CS_HEIGHT, 
        DEFAULT_REGION_WIDTH, DEFAULT_REGION_HEIGHT,
        DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE
    )) return 2;
    ArrangeChunks(cs);
    //******************************************/
    
    // TODO: Solve problem
    //******************************************/
    // Initializing particle system
    InitParticles();
    srand(time(NULL));
    //******************************************/
    
    //******************************************/
    // Initializing global timer
    init_global_time();
    //******************************************/
    
    //******************************************/
    // Initializing mouse
    int loaded = load_cursor_image(&(*game)->mouse, "./resources/standard_cursor.png");
    printf("cursor loaded: %d\n", loaded ? FALSE : TRUE);
    //******************************************/
    
    //******************************************/
    // Initializing gui system
    GuiBox* gb;
    init_gui_handler();
    CreateGuiBox(&gb, "gui_box", (Color){.rgba=0xFFFFFFFF}, 0, FALSE, (Pos){0, 0}, (Pos){(*game)->win->w, (*game)->win->h});
    (*game)->gui.type = GUI_BOX;
    (*game)->gui.element = (void*)gb;
    //******************************************/
    
    
    //******************************************/
    // Initializing game's callback system
    for(int i = 0; i < CB_COUNT_MAX; i++){
        (*game)->callbacks[i] = NULL;
    }
    (*game)->cbCount = 0;
    //******************************************/
    
    //******************************************/
    // Initializing game's system parameters
    (*game)->s_params.bg_color.rgba = 0xFF181818;
    (*game)->s_params.hm_mode = FALSE;
    (*game)->s_params.paused = FALSE;
    (*game)->s_params.delay = 0;
    (*game)->s_params.frameLockEnabled = TRUE;
    (*game)->s_params.frameLock = 90;
    //******************************************/
    
    
    //******************************************/
    // Initializing game's gameplay parameters
    (*game)->g_params.brush_size = 3;
    (*game)->g_params.selectedParticleType = 0;
    //******************************************/

    return 0;
}

void DeleteParticleGame(ParticleGame** game){
    // save_image_png(&(*game)->mouse.cursor_img_normal, "hello.png");
    free_cursor_image(&(*game)->mouse);
    DeleteGuiBox((GuiBox**)(&(*game)->gui.element));
    DeleteChunkSpace(&(*game)->cs);
    DestroyWindow(&((*game)->win));
    free(*game);
}

int add_callback_pg(ParticleGame* game, void (*callback)(ParticleGame* game)){
    if(game->cbCount >= CB_COUNT_MAX){
        game->cbCount = CB_COUNT_MAX;
        printf("callback array is filled up\n");
        return -1;
    }

    int index = -1;
    for(int i = 0; i < CB_COUNT_MAX; i++){
        if(game->callbacks[i] == NULL){
            game->callbacks[i] = callback;
            index = i;
            game->cbCount++;
            break;
        }
    }

    return index;
}

int cmp_greater(const void* a, const void* b){
    return *(int*)a - *(int*)b;
}


#define INF 0x3F3F3F3F
void delete_callback_pg(ParticleGame* game, int cb_index){
    if(game->cbCount <= 0){
        game->cbCount = 0;
        printf("callback array is empty\n");
        return;
    }

    if(cb_index < 0){
        return;
    }

    if(cb_index >= CB_COUNT_MAX){
        cb_index = CB_COUNT_MAX - 1;
        printf("callback array invalid index\n");
        return;
    }

    game->callbacks[cb_index] = NULL;
    game->cbCount--;
}

// void call_all_callbacks(ParticleGame* game){
//     for(int i = 0; i < game->cbCount; i++){
//         game->callbacks[i](game);
//     }
// }

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


int BuildLabEnv(ParticleGame* game){
    ChunkSpace* cs = &(game->cs);

    WallBoxCS(cs);
    SetSimEndpointsChunkSpace(cs, 0, cs->width_c, 0, cs->height_c);

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
