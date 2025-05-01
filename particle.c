#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

//Third party
#include <SDL2/SDL.h>


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255

typedef enum{
    EMPTY = 0,
    SAND,
    WATER,
    BORDER
} PartType;


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
    PartType t;
} Particle;

//Time
clock_t oldtime;
clock_t deltatime;
clock_t load;


//Window
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
int isrunning = 1;
const char* window_title = "SDL Playground";
int scrWidth = 640;
int scrHeight = 480;


//Simulator params
int partSide = 32;
int simRows;
int simCols;
Particle** pMap;
int pCount = 0;

//Color
Color ZeroColor = {0, 0, 0, 255};
Colors ColorSeq[] = {BLUE, RED, GREEN, YELLOW, ORANGE, PURPLE};
int currentColor = 0;
int colorSeqSize = 6;

//Particles
Colors PartSeq[] = {SAND};
int currentPart = 0;
int partSeqSize = 1;
int bound;


int InitAll();
void ClearMap(Particle** pMap, int rows, int cols);
void** Malloc2D(int rows, int cols, int elemSize);
void ChangeColor(Color* c, Colors cs);
void CopyColor(Color* dest, Color* src);
int CompColors(Color* c1, Color* c2);
void SandBehave(int* x, int* y, PartType ld, PartType d, PartType rd);
void Simulate(Particle** pMap, int rows, int cols);
clock_t FindDelta(clock_t old);

int main(int argc, char* argv[]){
    
    InitAll();
    
    simRows = scrHeight/partSide;
    simCols = scrWidth/partSide;
    bound = scrHeight - partSide;
    Color color;
    ChangeColor(&color, BLACK);
    PartType part = SAND;

    pMap = (Particle**)Malloc2D(simRows, simCols, sizeof(Particle));

    ClearMap(pMap, simRows, simCols);

    int mx, my;

    oldtime = clock();
    while(isrunning){
        deltatime = FindDelta(oldtime);
        load += deltatime;

        SDL_SetRenderDrawColor(renderer, MWHITE);
        SDL_RenderClear(renderer);
        
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            
            if(event.type == SDL_QUIT) isrunning = 0;
            if(event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&mx, &my);
            if(event.button.button == SDL_BUTTON_LEFT){
                int px = ((mx) / partSide);
                int py = ((my) / partSide);
                if(px >= 0 && py >= 0 && px < simCols && py < simRows){
                    pMap[py][px].x = (px * partSide)+1;
                    pMap[py][px].y = (py * partSide)+1;
                    CopyColor(&pMap[py][px].c, &color);
                    pMap[py][px].t = SAND;
                }
            }
            if(event.button.button == SDL_BUTTON_RIGHT){

            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE) isrunning = 0;
                if(event.key.keysym.sym == SDLK_a){
                    ClearMap(pMap, simRows, simCols);
                }
                if(event.key.keysym.sym == SDLK_c){
                    currentColor = (currentColor + 1) % colorSeqSize;
                    ChangeColor(&color, ColorSeq[currentColor]);
                }
                if(event.key.keysym.sym == SDLK_x){
                    currentPart = (currentPart + 1) % partSeqSize;
                    part = PartSeq[currentPart];
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, MBLACK);

        for(int i = 0; i < scrWidth; i += partSide){
            SDL_RenderDrawLine(renderer, i, 0, i, scrHeight);
        }
        
        for(int i = 0; i < scrHeight; i += partSide){
            SDL_RenderDrawLine(renderer, 0, i, scrWidth, i);
        }

        for(size_t i = 0; i < simRows; i++){
            for(size_t j = 0; j < simCols; j++){
                if(pMap[i][j].x == -1 || pMap[i][j].y == -1) continue;
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

        Simulate(pMap, simRows, simCols);
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


void ClearMap(Particle** pMap, int rows, int cols){

    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            pMap[i][j].x = -1;
            pMap[i][j].y = -1;
            pMap[i][j].t = EMPTY;
        }
    }
}


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


void CopyColor(Color* dest, Color* src){
    dest->r = src->r;
    dest->g = src->g;
    dest->b = src->b;
    dest->a = src->a;
}


int CompColors(Color* c1, Color* c2){
    if(c1->r != c2->r) return 1;
    if(c1->g != c2->g) return 1;
    if(c1->b != c2->b) return 1;
    if(c1->a != c2->a) return 1;
    return 0;
}


void SandBehave(int* x, int* y, PartType ld, PartType d, PartType rd){
    if(d == EMPTY){ (*y) += partSide; return; }
    if(ld == EMPTY){ (*y) += partSide, (*x) -= partSide; return; }
    if(rd == EMPTY){ (*y) += partSide, (*x) += partSide; return; }
}


void Simulate(Particle** pMap, int rows, int cols){

    if(load < 10000) return;   
    load = 0; 

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(pMap[i][j].t == SAND){
                if(pMap[i][j].y >= bound) continue;
                SandBehave(
                    &pMap[i][j].x, 
                    &pMap[i][j].y, 
                    pMap[i + 1][j - 1].t, 
                    pMap[i + 1][j].t, 
                    pMap[i + 1][j + 1].t);
            }
        }
    }
}


clock_t FindDelta(clock_t old){
    return clock() - old;
}