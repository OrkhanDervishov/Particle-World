#include "renderer_sw.h"

void clear_window(Window *window, Color color){
    // printf("works1\n");
    // SDL_FillRect(
    //     SDL_GetWindowSurface(window->window), NULL, 
    //     SDL_MapRGB(
    //         SDL_GetWindowSurface(window->window)->format, 
    //         color.r, color.g, color.b
    //     )
    // );
    // printf("works2\n");
    fill_f(window->context, color);
}

void gen_light_chunk(Chunk* chunk){
    
    Image img;
    img.buffer = NULL;
    create_image(&img, chunk->w, chunk->h);

    for(int i = 0; i < chunk->h; i++)
    for(int j = 0; j < chunk->w; j++){
        IMG_GET(img, j, i) = CHUNK_GET_TYPE(*chunk, j, i) == LAVA ? CHUNK_GET_COLOR(*chunk, j, i) : (Color){.rgba=0xFF000000};
    }

    save_image_png(&img, "light_chunk.png");
}