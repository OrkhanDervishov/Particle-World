#include "window.h"


int CreateWindow(Window** win, int w, int h, const char* title, bool fs){
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    *win = (Window*)malloc(sizeof(Window));
    
    if(strlen(title) > MAX_TITLE_LEN)
    strcpy((*win)->title, title);
    (*win)->isrunning = 1;
    (*win)->w = w;
    (*win)->h = h;
    
    (*win)->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_SHOWN
    );

    if(fs)
        SDL_SetWindowFullscreen((*win)->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    if((*win)->window == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    (*win)->renderer = SDL_CreateRenderer((*win)->window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawBlendMode((*win)->renderer, SDL_BLENDMODE_BLEND);

    if((*win)->renderer == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }


    return 0;
}

void DestroyWindow(Window** win){
    printf("works01\n");
    SDL_DestroyRenderer((*win)->renderer);
    printf("works02\n");
    if((*win)->window == NULL) printf("Window deletion error\n");





    
    printf("works03\n");
    SDL_DestroyWindow((*win)->window);
    printf("works04\n");
    SDL_Quit();
    if((*win) == NULL) printf("Window is null\n");
    printf("works05\n");
    free(*win);
    (*win) = NULL;
    printf("works06\n");
}

void Clear(Window* win){
    SDL_SetRenderDrawColor(win->renderer, MGOOD_GRAY);
    SDL_RenderClear(win->renderer);
}


int SetWindowIcon(Window* window, char* path){
    SDL_Surface* icon = SDL_LoadBMP(path);
    if(icon == NULL){
        return 1;
    }
    else SDL_SetWindowIcon(window->window, icon);
    SDL_FreeSurface(icon);
    return 0;
}

void GetWindowInfo(Window* window){
    SDL_RendererInfo info;
    if(window->renderer == NULL) printf("Renderer is null\n");
    SDL_GetRendererInfo(window->renderer, &info);
    printf("%s\n", info.name);
}