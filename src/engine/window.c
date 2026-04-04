#include "window.h"


int CreateWindow(Window** win, int w, int h, const char* title, bool fs){
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    *win = (Window*)malloc(sizeof(Window));
    
    if(strlen(title) < MAX_TITLE_LEN)
        strcpy((*win)->title, title);
    (*win)->isrunning = 1;
    (*win)->w = w;
    (*win)->h = h;
    
    (*win)->window = SDL_CreateWindow   (
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
    
    // To manipulate SDL's window surface through custom image object
    Surface *surf = SDL_GetWindowSurface((*win)->window);
    (*win)->context.width = surf->w;
    (*win)->context.height = surf->h;
    (*win)->context.format = create_format(0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    (*win)->context.buffer = surf->pixels;
    (*win)->w = surf->w;
    (*win)->h = surf->h;



    SDL_ShowCursor(SDL_DISABLE);
    // (*win)->renderer = SDL_CreateRenderer((*win)->window, -1, SDL_RENDERER_SOFTWARE);
    // if((*win)->renderer == NULL){
    //     fprintf(stderr, "Error: %s\n", SDL_GetError());
    //     return 1;
    // }
    // SDL_SetRenderDrawBlendMode((*win)->renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

void DestroyWindow(Window** win){
    // if((*win)->renderer != NULL)
    //     SDL_DestroyRenderer((*win)->renderer);
    if((*win)->window != NULL)
        SDL_DestroyWindow((*win)->window);
    if((*win) != NULL)
        free(*win);
    (*win) = NULL;
    SDL_Quit();
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