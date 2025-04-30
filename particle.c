#include <stdio.h>
#include <string.h>

//Third party
#include <SDL2/SDL.h>

#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255
#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
int isrunning = 1;
const char* window_title = "SDL Playground";
int scrWidth = 640;
int scrHeight = 480;

int partSide = 32;



SDL_Rect pList[500];
int pCount = 0;



int InitAll();
void ClearList(int* pMap, int size);

int main(int argc, char* argv[]){
    
    InitAll();
    int mx, my;

    while(isrunning){

        SDL_SetRenderDrawColor(renderer, BLACK);
        SDL_RenderClear(renderer);
        
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) isrunning = 0;
            if(event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&mx, &my);
            if(event.button.button == SDL_BUTTON_LEFT){
                int px = (mx / partSide);
                int py = (my / partSide);
                SDL_Rect rect = {(px * partSide)+1, (py * partSide)+1, partSide-1, partSide-1};
                pList[px*py] = rect;
            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE) isrunning = 0;
                //if(event.key.keysym.sym == SDLK_c) ClearList(pMap, listSize);
            }
        }

        SDL_SetRenderDrawColor(renderer, WHITE);

        for(int i = 0; i < scrWidth; i += partSide){
            SDL_RenderDrawLine(renderer, i, 0, i, scrHeight);
        }

        for(int i = 0; i < scrHeight; i += partSide){
            SDL_RenderDrawLine(renderer, 0, i, scrWidth, i);
        }

        SDL_SetRenderDrawColor(renderer, BLUE);
        for(size_t i = 0; i < 16; i++){
            for(size_t j = 0; j < 21; j++){
                SDL_RenderFillRect(renderer, &pList[i*j]);                                
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
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


// void ClearList(int* pMap, int size){
//     for(size_t i = 0; i < 16; i++){
//         for(size_t j = 0; j < 21; j++){
//             pMap[i][j] = 0;
//         }
//     }
// }