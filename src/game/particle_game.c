#include "particle_game.h"
#include "custom_parser.h"
#include "renderer_3d.h"


#define CS_WIDTH    DEFAULT_CS_WIDTH
#define CS_HEIGHT   DEFAULT_CS_HEIGHT

#define REGION_WIDTH DEFAULT_REGION_WIDTH
#define REGION_HEIGHT DEFAULT_REGION_HEIGHT


void pw_engine_read_config(ParticleEngine* pe, const char *conf_path);

int CreateParticleEngine(ParticleEngine** game, const char* conf_path){

    (*game) = (ParticleEngine*)malloc(sizeof(ParticleEngine));

    
    //******************************************/
    // Initializing game's system parameters
    (*game)->s_params.is_running = TRUE;
    (*game)->s_params.paused = FALSE;
    (*game)->s_params.restart = FALSE;
    // (*game)->s_params.hm_mode = FALSE;
    // (*game)->s_params.delay = 0;
    (*game)->s_params.frameLockEnabled = TRUE;
    (*game)->s_params.frameLock = 90;
    (*game)->s_params.clear_color.rgba = 0x00181818;
    (*game)->s_params.window_render_api = PW_WINDOW_SURFACE;
    //******************************************/
    
    //******************************************/
    // Initializing game's gameplay parameters
    (*game)->g_params.brush_size = 3;
    (*game)->g_params.selectedParticleType = 0;
    //******************************************/

    
    pw_engine_read_config(*game, conf_path);
    
    
    //******************************************/
    // Initializing game window
    if(pw_window_create(&((*game)->win), (*game)->s_params.width, (*game)->s_params.height, (*game)->s_params.title, (*game)->s_params.fullscreen, (*game)->s_params.window_render_api)) return 1;
    pw_window_set_icon((*game)->win, (*game)->s_params.icon_path);
    pw_window_visible((*game)->win, (*game)->s_params.window_visible);
    if((*game)->s_params.window_render_api == PW_WINDOW_OPENGL)
        init_opengl((*game)->win);
    //******************************************/
    
    (*game)->camera = pw_create_camera2d((vec2f){0.0f, 0.0f}, (vec2f){(float)(*game)->win->w, (float)(*game)->win->h}, 1.0f);

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
    // Initializing mouse
    if((*game)->s_params.use_custom_cursor){
        pw_mouse_visible(false);
        load_cursor_image(&(*game)->mouse, (*game)->s_params.cursor_path);
    } else {
        pw_mouse_visible(true);
    }
    //******************************************/
    
    //******************************************/
    init_global_time();
    init_input_system(&(*game)->is, (*game)->win->id);
    pw_entity_manager_init(&(*game)->em);
    pw_asset_manager_init(&(*game)->am);
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

    return 0;
}

void DeleteParticleEngine(ParticleEngine** game){
    free((*game)->s_params.title);
    free((*game)->s_params.icon_path);
    free((*game)->s_params.cursor_path);

    pw_asset_manager_free(&(*game)->am);
    free_cursor_image(&(*game)->mouse);
    DeleteGuiBox((GuiBox**)(&(*game)->gui.element));
    DeleteChunkSpace(&(*game)->cs);
    pw_window_destroy(&((*game)->win));
    free(*game);
    *game = NULL;
}

int add_callback_pg(ParticleEngine* game, void (*callback)(ParticleEngine* game)){
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
void delete_callback_pg(ParticleEngine* game, int cb_index){
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

// void call_all_callbacks(ParticleEngine* game){
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


int BuildLabEnv(ParticleEngine* game){
    ChunkSpace* cs = &(game->cs);

    WallBoxCS(cs);
    SetSimEndpointsChunkSpace(cs, 0, cs->width_c, 0, cs->height_c);

    return 0;
}


void pw_engine_read_config(ParticleEngine* pe, const char *conf_path){

    const char* config_text = myconfig_load_config(conf_path);
    if(config_text == NULL){
        perror("ERROR: Could not read engine's config file");
        exit(1);
    }
    ConfigPairs pairs = myconfig_read_all_pairs(&config_text);

    pe->s_params.title = myconfig_get_value_string_new(pairs, "window_title", "window");
    pe->s_params.width = (size_t)myconfig_get_value_number(pairs, "window_width", "window");
    pe->s_params.height = (size_t)myconfig_get_value_number(pairs, "window_height", "window");
    pe->s_params.fullscreen = myconfig_get_value_bool(pairs, "window_fullscreen", "window");
    pe->s_params.window_visible = myconfig_get_value_bool(pairs, "window_visible", "window");
    
    char* api = myconfig_get_value_string_new(pairs, "window_render_api", "window");
    if(strcmp(api, "surface") == 0) pe->s_params.window_render_api = PW_WINDOW_SURFACE;
    if(strcmp(api, "renderer") == 0) pe->s_params.window_render_api = PW_WINDOW_RENDERER;
    free(api);


    pe->s_params.icon_path = myconfig_get_value_string_new(pairs, "icon_image_path", "window");

    pe->s_params.use_custom_cursor = myconfig_get_value_bool(pairs, "use_custom_cursor", "window");
    pe->s_params.cursor_path = myconfig_get_value_string_new(pairs, "custom_cursor_path", "window");

    pe->s_params.frameLockEnabled = myconfig_get_value_bool(pairs, "frame_lock_enabled", "window");
    pe->s_params.frameLock = (int)myconfig_get_value_number(pairs, "frame_lock", "window");
    
    pe->s_params.clear_color.rgba = (pnt_color_t)STR_HEX_TO_NUMBER(myconfig_get_value_string(pairs, "clear_color", "window"));

    myconfig_free_pairs(pairs);
}
