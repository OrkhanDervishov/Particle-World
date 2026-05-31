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
    // Initializing camera
    (*game)->camera.pos = (vec2f){0.0f, 0.0f};
    //******************************************/
    
    //******************************************/
    // Initializing gui system
    init_input_system(&(*game)->is);
    //******************************************/

    //******************************************/
    // Initializing entity system
    POOL_INIT((*game)->ep);
    //******************************************/
    
    //******************************************/
    // Initializing text rendering system
    InitBasicTextRenderer();
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
    (*game)->s_params.is_running = TRUE;
    (*game)->s_params.paused = FALSE;
    (*game)->s_params.hm_mode = FALSE;
    (*game)->s_params.delay = 0;
    (*game)->s_params.frameLockEnabled = TRUE;
    (*game)->s_params.frameLock = 90;
    (*game)->s_params.clear_color.rgba = 0x00181818;
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

// SDL_Texture* GetTexture(SDL_Renderer* renderer, SDL_Texture* bgt, char* path){
//     // SDL_Surface* bgs = SDL_LoadBMP(BG_PATH);
//     SDL_Rect bgrect;
//     bgrect.x = 0;
//     bgrect.y = 0;

//     // TODO: Give screen width and height
//     bgrect.w = 700;
//     bgrect.h = 700;
//     if(bgs == NULL) printf("background fail\n");
//     else {
//         bgt = SDL_CreateTextureFromSurface(renderer, bgs);
//         return bgt;
//     }
//     SDL_FreeSurface(bgs);
// }


int BuildLabEnv(ParticleGame* game){
    ChunkSpace* cs = &(game->cs);

    WallBoxCS(cs);
    SetSimEndpointsChunkSpace(cs, 0, cs->width_c, 0, cs->height_c);

    return 0;
}
