#include <stdio.h>
#include <string.h>
#include <math.h>

// Third party
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>


typedef struct{
    int x;
    int y;
} Pos;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;



SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
int isrunning = 1;
const char* window_title = "Particle Simulator";
int scrWidth = 640;
int scrHeight = 480;


int InitAll();
void DrawLine(SDL_Surface* surface, Pos prev, Pos next, Color c);
void ClearSurface(SDL_Surface* surface, Color c);

int main(int argc, char* argv[]){
    
    if(InitAll()){
        return 0;
    }

    surface = SDL_GetWindowSurface(window);
    Pos pos;
    Pos mouselast;

    Color red = {255, 0, 0};
    Color green = {0, 255, 0};
    Color blue = {0, 0, 255};
    Color white = {255, 255, 255};
    Color black = {0, 0, 0};
    ClearSurface(surface, white);

    while(isrunning){
        SDL_Event event;
        SDL_GetMouseState(&mouselast.x, &mouselast.y);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) isrunning = 0;
            if(event.button.button == SDL_BUTTON_LEFT){
                SDL_GetMouseState(&pos.x, &pos.y);
                //DrawLine(surface, mouselast, pos, black);
                mouselast = pos;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_c){
                    ClearSurface(surface, white);
                }
                if(event.key.keysym.sym == SDLK_ESCAPE) isrunning = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, 20, 20, 100, 100);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}




int InitAll(){
    if(SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        scrWidth,
        scrHeight,
        SDL_WINDOW_SHOWN
    );

    if(window == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if(renderer == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}


void DrawPixel(SDL_Surface* surface, Pos pos, Color c)
{
    SDL_LockSurface(surface);
    uint8_t bpp = surface->format->BytesPerPixel;
    int sp = surface->pitch;
    uint8_t* pixels = (uint8_t*)surface->pixels;

    if(pos.x < 0 || pos.x >= scrWidth) return;
    if(pos.y < 0 || pos.y >= scrHeight) return;

    pixels[pos.y*sp + pos.x*bpp] = c.b;
    pixels[pos.y*sp + pos.x*bpp + 1] = c.g;
    pixels[pos.y*sp + pos.x*bpp + 2] = c.r;
    SDL_UnlockSurface(surface);
}


void ClearSurface(SDL_Surface* surface, Color c){

    SDL_LockSurface(surface);
    uint8_t bpp = surface->format->BytesPerPixel;
    int sp = surface->pitch;
    uint8_t* pixels = (uint8_t*)surface->pixels;

    for(size_t i = 0; i < scrHeight; i++){
        for(size_t j = 0; j < scrWidth; j++){        
            pixels[i*sp + j*bpp + 0] = c.b;
            pixels[i*sp + j*bpp + 1] = c.g;
            pixels[i*sp + j*bpp + 2] = c.r;            
        }
    }
    SDL_UnlockSurface(surface);
}