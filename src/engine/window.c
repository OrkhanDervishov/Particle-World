#include "window.h"


FormatImage sdl_surface_to_fimage(SDL_Surface *surf){
    // To manipulate SDL's window surface through custom image object
    FormatImage fimage;
    fimage.width = surf->w;
    fimage.height = surf->h;
    fimage.stride = surf->w;
    fimage.format = pnt_create_format(0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    fimage.buffer = surf->pixels;
    return fimage;
}


int sdl2_init(Window* win){
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    // printf("works3\n");
    // const char* glsl_version = "#version 130";
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    // printf("works4\n");
    
    // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // printf("works5\n");
    // win->gl_context = SDL_GL_CreateContext(win->window);
    // if(win->gl_context == NULL){
    //     printf("ERROR: SDL_GL_CreateContext failed\n");
    //     return 2;
    // }
    // printf("works6\n");
    // *(*win)->gl_context = SDL_GL_CreateContext(*win);
    return 0;
}


int pw_window_create(Window** win, int w, int h, const char* title, bool fs, PWWindowRenderingApi api){
    *win = (Window*)malloc(sizeof(Window));
    
    if(strlen(title) < MAX_TITLE_LEN)
    strcpy((*win)->title, title);
    (*win)->isrunning = 1;
    (*win)->w = w;
    (*win)->h = h;
    
    sdl2_init(*win);

    // WINDOW
    /************************************************* */
    uint32_t flags = 0;

    if(api == PW_WINDOW_OPENGL)
        flags |= SDL_WINDOW_OPENGL;

    if(fs){
        // SDL_SetWindowFullscreen((*win)->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    (*win)->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        flags
    );
    if(fs){
        SDL_GetWindowSize((*win)->window, &(*win)->w, &(*win)->h);
    }
    
    if((*win)->window == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }
    
    (*win)->id = SDL_GetWindowID((*win)->window);
    

    (*win)->render_api = api;
    SDL_GetWindowSize((*win)->window, &(*win)->w, &(*win)->h);

    /************************************************* */
    
    
    // CONTEXT
    /************************************************* */
    // To manipulate SDL's window surface through custom image object
    (*win)->context.buffer = NULL;
    pnt_create_image(&(*win)->context, (*win)->w, (*win)->h);

    if(api == PW_WINDOW_SURFACE){
        Surface *surf = SDL_GetWindowSurface((*win)->window);
        // (*win)->w = surf->w;
        // (*win)->h = surf->h;
        (*win)->fcontext = sdl_surface_to_fimage(surf);
    }

    if(api == PW_WINDOW_RENDERER){
        (*win)->renderer = SDL_CreateRenderer(
            (*win)->window, 
            -1, 
            SDL_RENDERER_ACCELERATED
        );

        (*win)->context_texture = SDL_CreateTexture(
            (*win)->renderer, 
            // SDL_PIXELFORMAT_RGBA8888, 
            SDL_PIXELFORMAT_RGBA32, 
            SDL_TEXTUREACCESS_STREAMING, 
            (*win)->context.width,
            (*win)->context.height
        );
    }

    int ww, wh;
    int rw, rh;

    SDL_GetWindowSize((*win)->window, &ww, &wh);
    SDL_GetRendererOutputSize((*win)->renderer, &rw, &rh);
    /************************************************* */
    //SDL_ShowCursor(SDL_DISABLE);
    // (*win)->renderer = SDL_CreateRenderer((*win)->window, -1, SDL_RENDERER_SOFTWARE);
    // if((*win)->renderer == NULL){
    //     fprintf(stderr, "Error: %s\n", SDL_GetError());
    //     return 1;
    // }
    // SDL_SetRenderDrawBlendMode((*win)->renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

void pw_window_destroy(Window** win){
    if((*win)->renderer != NULL)
        SDL_DestroyRenderer((*win)->renderer);
    pnt_delete_image(&(*win)->context);
    if((*win)->window != NULL)
        SDL_DestroyWindow((*win)->window);
    if((*win) != NULL)
        free(*win);
    (*win) = NULL;
    SDL_Quit();
}



void pw_window_present(Window* win){
    if(win->render_api != PW_WINDOW_SURFACE) return;
    pnt_image_to_fimage(win->context, win->fcontext);
    SDL_UpdateWindowSurface(win->window);
}

void pw_window_prepare_renderer(Window* win){
    if(win->render_api != PW_WINDOW_RENDERER) return;
    SDL_Rect context_rect = {
        0, 0, win->context.width, win->context.height
    };
    SDL_UpdateTexture(win->context_texture, NULL, win->context.buffer, win->context.stride*sizeof(pnt_color_t));
    SDL_RenderCopy(win->renderer, win->context_texture, &context_rect, &context_rect);
}

void pw_window_present_renderer(Window* win){
    if(win->render_api != PW_WINDOW_RENDERER) return;
    SDL_RenderPresent(win->renderer);
}


void pw_window_clear(Window* win){
    // SDL_SetRenderDrawColor(win->renderer, MGOOD_GRAY);
    // SDL_RenderClear(win->renderer);
}


int pw_window_set_icon(Window* window, const char* path){
    if(path == NULL) return 1;
    SDL_Surface* icon = SDL_LoadBMP(path);
    if(icon == NULL){
        return 2;
    }
    SDL_SetWindowIcon(window->window, icon);
    SDL_FreeSurface(icon);
    return 0;
}

void pw_window_get_info(Window* window){
    // SDL_RendererInfo info;
    // if(window->renderer == NULL) printf("Renderer is null\n");
    // SDL_GetRendererInfo(window->renderer, &info);
    // printf("%s\n", info.name);
}

void pw_window_visible(Window* window, bool visible){
    if(visible)
        SDL_ShowWindow(window->window);
    else
        SDL_HideWindow(window->window);
}