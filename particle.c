#include <stdio.h>
#include <string.h>
#include <malloc.h>

//Third party
#include <SDL2/SDL.h>


typedef enum{
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    ORANGE,
    GRAY,
    PURPLE
} Colors;


typedef struct{
    int r;
    int g;
    int b;
    int a;
} Color;


typedef struct{
    int x, y;
    Color c;
} Particle;


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
int isrunning = 1;
const char* window_title = "SDL Playground";
int scrWidth = 640;
int scrHeight = 480;

int partSide = 32;
int simRows;
int simCols;

Particle** pMap;
SDL_Rect pList[500];
int pCount = 0;


Colors ColorSeq[] = {BLUE, RED, GREEN, YELLOW, ORANGE, PURPLE};
int currentColor = 0;
int seqSize = 6;


int InitAll();
void ClearList(int* pMap, int size);
void** Malloc2D(int rows, int cols, int elemSize);
void ChangeColor(Color* c, Colors cs);

int main(int argc, char* argv[]){
    
    InitAll();
    
    simRows = scrHeight/partSide;
    simCols = scrWidth/partSide;
    Color color = {255, 255, 255, 255};

    pMap = (Particle**)Malloc2D(simRows, simCols, sizeof(Particle));

    for(size_t i = 0; i < simRows; i++){
        for(size_t j = 0; j < simCols; j++){
            pMap[i][j].x = 0;
            pMap[i][j].y = 0;
            pMap[i][j].c.r = 0, pMap[i][j].c.g = 0, pMap[i][j].c.b = 0, pMap[i][j].c.a = 0;
        }
    }

    int mx, my;

    while(isrunning){

        SDL_SetRenderDrawColor(renderer, MBLACK);
        SDL_RenderClear(renderer);
        
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            
            if(event.type == SDL_QUIT) isrunning = 0;
            if(event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&mx, &my);
            if(event.button.button == SDL_BUTTON_LEFT){
                int px = ((mx) / partSide);
                int py = ((my) / partSide);
                if(px < simCols && py < simRows){
                    pMap[py][px].x = (px * partSide)+1;
                    pMap[py][px].y = (py * partSide)+1;
                    pMap[py][px].c.r = color.r;
                    pMap[py][px].c.g = color.g;
                    pMap[py][px].c.b = color.b;
                    pMap[py][px].c.a = color.a;
                }
            }
            if(event.button.button == SDL_BUTTON_RIGHT){

            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE) isrunning = 0;
                if(event.key.keysym.sym == SDLK_a){
                    
                    for(size_t i = 0; i < simRows; i++){
                        for(size_t j = 0; j < simCols; j++){
                            pMap[i][j].x = 0;
                            pMap[i][j].y = 0;
                            pMap[i][j].c.r = 0, pMap[i][j].c.g = 0, pMap[i][j].c.b = 0, pMap[i][j].c.a = 0;
                        }
                    }
                }
                if(event.key.keysym.sym == SDLK_c){
                    currentColor = (currentColor + 1) % seqSize;
                    ChangeColor(&color, ColorSeq[currentColor]);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, MWHITE);

        for(int i = 0; i < scrWidth; i += partSide){
            SDL_RenderDrawLine(renderer, i, 0, i, scrHeight);
        }
        
        for(int i = 0; i < scrHeight; i += partSide){
            SDL_RenderDrawLine(renderer, 0, i, scrWidth, i);
        }

        for(size_t i = 0; i < simRows; i++){
            for(size_t j = 0; j < simCols; j++){
                SDL_SetRenderDrawColor(
                    renderer,
                    pMap[i][j].c.r, 
                    pMap[i][j].c.g, 
                    pMap[i][j].c.b, 
                    pMap[i][j].c.a);
                SDL_Rect r = {pMap[i][j].x, pMap[i][j].y, partSide-1, partSide-1};
                SDL_RenderFillRect(renderer, &r);                           
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }


    for(int i = 0; i < simRows; i++){
        free(pMap[i]);
    }
    free(pMap);

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


// void ClearList(int* pMap, int size){
//     for(size_t i = 0; i < 16; i++){
//         for(size_t j = 0; j < 21; j++){
//             pMap[i][j] = 0;
//         }
//     }
// }


void** Malloc2D(int rows, int cols, int elemSize){
    void** arr = malloc(rows * sizeof(void*));
    for(int i = 0; i < rows; i++){
        arr[i] = malloc(cols * elemSize);
    }
    return arr;
}


void ChangeColor(Color *c, Colors cs){
    switch (cs)
    {
    case WHITE:
        c->r = 255, c->g = 255, c->b = 255, c->a = 255;
        break;
    case BLACK:
        c->r = 0, c->g = 0, c->b = 0, c->a = 255;
        break;
    case RED:
        c->r = 255, c->g = 0, c->b = 0, c->a = 255;
        break;
    case GREEN:
        c->r = 0, c->g = 255, c->b = 0, c->a = 255;
        break;
    case BLUE:
        c->r = 0, c->g = 0, c->b = 255, c->a = 255;
        break;
    case YELLOW:
        c->r = 255, c->g = 255, c->b = 0, c->a = 255;
        break;
    case ORANGE:
        c->r = 255, c->g = 255, c->b = 100, c->a = 255;
        break;
    case GRAY:
        c->r = 100, c->g = 100, c->b = 100, c->a = 255;
        break;
    case PURPLE:
        c->r = 255, c->g = 0, c->b = 255, c->a = 255;
        break;
    
    default:
        break;
    }
}