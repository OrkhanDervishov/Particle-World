#include <stdio.h>

#include "timing.h"
#include "particle_load.h"
#include "chunk.h"
#include "region.h"
#include "chunk_space.h"
// #include "world.h"
#include "game/particle_game.h"
#include "game.h"
#include "chunk_renderer_sw.h"
#include "dirtyrect.h"
#include "particle_data.h"
#include "simulator.h"
#include "da.h"
#include "console.h"

// #include "uchar.h"
// #include "locale.h"



#define TYPE_GAME(type)\
do{\
    type x;\
}while(0)

int main(int argc, char* argv[]){
    // ht__typeof
    // int x;
    // TYPE_GAME(int);
    // test();

    PW_CONSOLE_STR("Hello World!", PW_ANSI_BACK_RED);
    PW_CONSOLE_NL;

    // const char* config_text = myconfig_load_config("./src/confs/game_startup_config.conf");
    // ConfigPairs pairs = myconfig_read_all_pairs(&config_text);
    // printf("%s = %s\n", "window_title", myconfig_get_value_string(pairs, "window_title", "window"));
    // myconfig_free_pairs(pairs);

    // return 0;

    // Ll(int) llist = LL_INITIALIZATION;
    // ll_push_head(llist, 10);
    // ll_push_head(llist, 11);
    // ll_push_head(llist, 12);
    // ll_push_head(llist, 13);
    // ll_push_head(llist, 14);

    // for(int i = 0; i < llist.count; i++){
    //     int var;
    //     ll_get_val(llist, var, i);
    //     printf("%d\n", var);
    // } 

    // setlocale(LC_ALL, "en_US.UTF-8");
    // // char32_t str = "ü";
    // // printf("%s", str);
    // const wchar_t *euro_symbol = L"€";

    // wprintf(L"The Euro symbol is: %ls\n", euro_symbol);
    // wprintf("ü\n");

    
    while(1){
        bool restart = false;
        ParticleEngine* game;
        if(CreateParticleEngine(&game, "./configs/game_conf.conf")){
            printf("failed\n");
            break;
        };
        if (SDL_GetError()[0] != '\0')
            printf("SDL error: %s\n", SDL_GetError());
        RunEntityGame(game);
        if(game->s_params.restart){
            restart = true;
        }
        DeleteParticleEngine(&game);
        if(restart) continue;
        break;
    }



    return 0;
}
