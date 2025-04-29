#include <stdio.h>
#include <string.h>
#include <math.h>

// Third party
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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
const char* window_title = "SDL Playground";
int scrWidth = 2304;
int scrHeight = 1294;


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

    SDL_Surface* image = SDL_LoadBMP("./images/CHESS.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

    scrWidth = image->w;
    scrHeight = image->h;
    SDL_SetWindowSize(window, image->w, image->h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_Rect r;
    r.h = scrHeight/3;
    r.w = scrWidth/3;
    r.x = 0;
    r.y = 0;

    SDL_Rect r2;
    r2.h = 100;
    r2.w = 200;
    r2.x = 0;
    r2.y = 0;



    while(isrunning){

        // if(r.x == scrWidth - r.w){
        //     r.x = 0;
        //     if(r.y == scrHeight - r.h){
        //         printf("x = %d, Y = %d", r.x, r.y);
        //         r.x = 0, r.y = 0;
        //     }
        //     else r.y += r.h;
        // }

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


        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, mouselast.x, mouselast.y);
        SDL_RenderDrawLine(renderer, scrWidth, scrHeight, mouselast.x, mouselast.y);
        SDL_RenderDrawLine(renderer, 0, scrHeight, mouselast.x, mouselast.y);
        SDL_RenderDrawLine(renderer, scrWidth, 0, mouselast.x, mouselast.y);

        //r
        SDL_RenderDrawLine(renderer, 0, 0, r.x, r.y);
        SDL_RenderDrawLine(renderer, scrWidth, scrHeight, r.x + r.w, r.y + r.h);
        SDL_RenderDrawLine(renderer, 0, scrHeight, r.x, r.y + r.h);
        SDL_RenderDrawLine(renderer, scrWidth, 0, r.x + r.w, r.y);

        //r2
        SDL_RenderDrawLine(renderer, 0, 0, r2.x, r2.y);
        SDL_RenderDrawLine(renderer, scrWidth, scrHeight, r2.x + r2.w, r2.y + r2.h);
        SDL_RenderDrawLine(renderer, 0, scrHeight, r2.x, r2.y + r2.h);
        SDL_RenderDrawLine(renderer, scrWidth, 0, r2.x + r2.w, r2.y);

        //r
        SDL_RenderDrawLine(renderer, r.x, r.y, r.x + r.w, r.y + r.h);
        SDL_RenderDrawLine(renderer, r.x + r.w, r.y, r.x, r.y + r.h);

        //r
        r.x = mouselast.x - r.w/2;
        r.y = mouselast.y - r.h/2;

        //r2
        r2.x = mouselast.x - r2.w/2;
        r2.y = mouselast.y - r2.h/2;

        // r.x++;
        SDL_RenderCopy(renderer, texture, &r, &r);
        SDL_RenderCopy(renderer, texture, &r2, &r2);
        SDL_RenderDrawRect(renderer, &r);
        SDL_RenderDrawRect(renderer, &r2);
        // SDL_RenderFillRect(renderer, &r);


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        SDL_RenderDrawLine(renderer, r.x, r.y, r.x + r.w, r.y + r.h);
        SDL_RenderDrawLine(renderer, r.x + r.w, r.y, r.x, r.y + r.h);

        // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        // SDL_RenderDrawLine(renderer, r2.x, r2.y, r2.x + r2.w, r2.y + r2.h);
        // SDL_RenderDrawLine(renderer, r2.x + r2.w, r2.y, r2.x, r2.y + r2.h);

        SDL_RenderPresent(renderer);
        SDL_Delay(6);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
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