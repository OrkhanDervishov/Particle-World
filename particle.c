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

typedef struct
{
    int x, y;
} Pos;



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
    LYELLOW,
    GRAY,
    PURPLE
} Colors;


typedef struct{
    int r, g, b, a;
} Color;


typedef struct{
    Pos p;
    int id;
    Color c;
    PartType t;
} Particle;


typedef struct{
    Particle** list;
    size_t size;
    size_t elems;
} PartList;


typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    int isrunning;
    const char* window_title;
    int scrWidth;
    int scrHeight;    
} Window;


typedef struct
{
    int partSide;
    int simRows;
    int simCols;
    Particle** pMap;
    int pCount;
} Simulator;


//Time
clock_t oldtime;
clock_t deltatime;
clock_t load;


//Pragram params
int mode = 1;
int drawlines = 0;


//Window
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
int isrunning = 1;
const char* window_title = "SDL Playground";
int scrWidth = 1080;
int scrHeight = 720;


//Simulator params
int partSide = 16;
int simRows;
int simCols;
Particle** pMap;
int pCount = 0;

//Color
Color ZeroColor = {0, 0, 0, 255};
Colors ColorSeq[] = {BLUE, RED, GREEN, YELLOW, LYELLOW, PURPLE};
int currentColor = 0;
int colorSeqSize = 6;

//Particles
PartList partList;
Colors PartSeq[] = {SAND, WATER, BORDER};
int currentPart = 0;
int partSeqSize = 3;
int bound;
PartType genType = SAND;


int InitAll();
void ClearMap(Particle** pMap, int rows, int cols);
void** Malloc2D(int rows, int cols, int elemSize);
void ChangeColor(Color* c, Colors cs);
void CopyColor(Color* dest, Color* src);
int CompColors(Color* c1, Color* c2);
void SandBehave(int* x, int* y, PartType l, PartType ld, PartType d, PartType r, PartType rd);
void LiquidBehave(int* x, int* y, PartType l, PartType ld, PartType d, PartType r, PartType rd);
void Simulate(Particle** pMap, PartList partList, int rows, int cols);
clock_t FindDelta(clock_t old);
int CreateParticle(Particle** pMap, int px, int py, Color* color, PartType type);
void DrawScene(SDL_Renderer* renderer, PartList partList);
void ProcessInput(SDL_Event event, int* isrunning, Particle** pMap, PartList* partList, Color* color);

int main(int argc, char* argv[]){
    
    // Inits
    InitAll();
    
    simRows = scrHeight/partSide;
    simCols = scrWidth/partSide;
    bound = scrHeight - partSide;
    Color color;
    ChangeColor(&color, BLACK);

    partList.elems = 0;
    partList.size = simRows*simCols;
    partList.list = (Particle**)malloc(partList.size * sizeof(Particle*));
    pMap = (Particle**)Malloc2D(simRows, simCols, sizeof(Particle));

    ClearMap(pMap, simRows, simCols);

    // Loop
    SDL_Event event;
    oldtime = clock();
    while(isrunning){
        deltatime = FindDelta(oldtime);
        load += deltatime;

        SDL_SetRenderDrawColor(renderer, MWHITE);
        SDL_RenderClear(renderer);
        
        // Input handling

        //SDL_Event event;
        //SDL_GetMouseState(&mx, &my);
        while(SDL_PollEvent(&event)){
            ProcessInput(event, &isrunning, pMap, &partList, &color);
        }


        // Render
        SDL_SetRenderDrawColor(renderer, MBLACK);

        if(drawlines){
            for(int i = 0; i < scrWidth; i += partSide){
                SDL_RenderDrawLine(renderer, i, 0, i, scrHeight);
            }
            
            for(int i = 0; i < scrHeight; i += partSide){
                SDL_RenderDrawLine(renderer, 0, i, scrWidth, i);
            }
        }

        DrawScene(renderer, partList);

        if(mode == 1)
            Simulate(pMap, partList, simRows, simCols);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }


    // Destroy
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
            pMap[i][j].id = -1;
            pMap[i][j].t = EMPTY;
        }
    }
    partList.elems = 0;
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
    case LYELLOW:
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


void SandBehave(
    int* x, 
    int* y, 
    PartType l, 
    PartType ld, 
    PartType d, 
    PartType r, 
    PartType rd)
{
    // printf("SAND\nx: %d, y: %d\n", *x, *y);
    if(d == EMPTY){ (*y) += 1; }
    else if(ld == EMPTY && l == EMPTY){ (*y) += 1, (*x) -= 1; }
    else if(rd == EMPTY && r == EMPTY){ (*y) += 1, (*x) += 1; }
    // printf("x: %d, y: %d\n\n", *x, *y);
}


void LiquidBehave(
    int* x,
    int* y, 
    PartType l, 
    PartType ld, 
    PartType d, 
    PartType r, 
    PartType rd)
{
    if(d == EMPTY){ (*y) += 1; }
    else if(ld == EMPTY && l == EMPTY){ (*y) += 1, (*x) -= 1; }
    else if(rd == EMPTY && r == EMPTY){ (*y) += 1, (*x) += 1; }
    //else if(r == EMPTY){ (*x) += 1; }
    else if(l == EMPTY){ (*x) -= 1; }
    else if(r == EMPTY){ (*x) += 1; }

}


void Simulate(Particle** pMap, PartList partList, int rows, int cols){

    // if(load < 10000) return;   
    // load = 0;

    int a = 0;
    for(size_t k = 0; k < partList.elems; k++){

        int j = (partList.list[k])->p.x;
        int i = (partList.list[k])->p.y;

        if(pMap[i][j].p.y >= rows-1 || pMap[i][j].t == EMPTY || pMap[i][j].t == BORDER) {continue;}
        int* x = &pMap[i][j].p.x;
        int* y = &pMap[i][j].p.y;

        PartType type = pMap[i][j].t;

        if(type == SAND){  
            SandBehave(
                x, 
                y, 
                ((*x > 0) ? pMap[i][j - 1].t : BORDER),
                ((*x > 0) ? pMap[i + 1][j - 1].t : BORDER), 
                pMap[i + 1][j].t,
                ((*x < cols - 1) ? pMap[i][j + 1].t : BORDER),
                ((*x < cols - 1) ? pMap[i + 1][j + 1].t : BORDER));
        }
        else if(type == WATER){
            LiquidBehave(
                x, 
                y, 
                ((*x > 0) ? pMap[i][j - 1].t : BORDER),
                ((*x > 0) ? pMap[i + 1][j - 1].t : BORDER), 
                pMap[i + 1][j].t,
                ((*x < cols - 1) ? pMap[i][j + 1].t : BORDER),
                ((*x < cols - 1) ? pMap[i + 1][j + 1].t : BORDER));
        }

        if(*y == i && *x == j) continue;
        partList.list[k] = &pMap[*y][*x];
        pMap[i][j].t = type;
        pMap[*y][*x] = pMap[i][j];
        pMap[i][j].id = -1;
        pMap[i][j].t = EMPTY;
    }
    
    return;

    for(int i = rows-1; i >= 0; i--){
        for(int j = cols-1; j >= 0; j--){
            if(pMap[i][j].p.y >= rows-1 || pMap[i][j].t == EMPTY || pMap[i][j].t == BORDER) {continue;}
            int* x = &pMap[i][j].p.x;
            int* y = &pMap[i][j].p.y;

            PartType type = pMap[i][j].t;

            if(type == SAND){
                SandBehave(
                    x, 
                    y, 
                    ((*x > 0) ? pMap[i][j - 1].t : BORDER),
                    ((*x > 0) ? pMap[i + 1][j - 1].t : BORDER), 
                    pMap[i + 1][j].t,
                    ((*x < cols - 1) ? pMap[i][j + 1].t : BORDER),
                    ((*x < cols - 1) ? pMap[i + 1][j + 1].t : BORDER));
            }
            else if(type == WATER){
                LiquidBehave(
                    x, 
                    y, 
                    ((*x > 0) ? pMap[i][j - 1].t : BORDER),
                    ((*x > 0) ? pMap[i + 1][j - 1].t : BORDER), 
                    pMap[i + 1][j].t,
                    ((*x < cols - 1) ? pMap[i][j + 1].t : BORDER),
                    ((*x < cols - 1) ? pMap[i + 1][j + 1].t : BORDER));
            }
            //printf("\n");
            if(*y == i && *x == j) continue;
            pMap[i][j].t = type;
            pMap[*y][*x] = pMap[i][j];
            pMap[i][j].id = -1;
            pMap[i][j].t = EMPTY;
        }
    }
}


clock_t FindDelta(clock_t old){
    return clock() - old;
}


int CreateParticle(Particle** pMap, int px, int py, Color* color, PartType type){
    if(px >= 0 && py >= 0 && px < simCols && py < simRows){
        //printf("works\n");
        // printf("px: %d, py: %d\n", px, py);
        if(pMap[py][px].id != -1) return 0;
        pMap[py][px].p.x = px;
        pMap[py][px].p.y = py;
        pMap[py][px].id = 1;
        CopyColor(&pMap[py][px].c, color);
        pMap[py][px].t = type;
        return 1;
    }
    return 0;
}


void DrawScene(SDL_Renderer* renderer, PartList partList){
    
    for(size_t i = 0; i < partList.elems; i++){
        SDL_SetRenderDrawColor(
            renderer,
            partList.list[i]->c.r, 
            partList.list[i]->c.g, 
            partList.list[i]->c.b, 
            partList.list[i]->c.a);
        SDL_Rect r = {
            partList.list[i]->p.x*partSide+1, 
            partList.list[i]->p.y*partSide+1, 
            partSide-1, 
            partSide-1};
        SDL_RenderFillRect(renderer, &r);                          
    }
}


void ProcessInput(
    SDL_Event event, 
    int* isrunning, 
    Particle** pMap, 
    PartList* partList,
    Color* color
)
{
    // printf("works\n");
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    if(event.type == SDL_QUIT) *isrunning = 0;
    // if(event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&mx, &my);
    if(event.button.button == SDL_BUTTON_LEFT){
        // printf("works\n");
        int created = 0;
        int px = ((mx) / partSide);
        int py = ((my) / partSide);
        if(mode == 0)
            CreateParticle(pMap, px, py, color, genType);
        if(mode == 1){
            switch (genType)
            {
            case SAND:
                ChangeColor(color, LYELLOW);
                created = CreateParticle(pMap, px, py, color, genType);
                break;
            case WATER:
                ChangeColor(color, BLUE);
                created = CreateParticle(pMap, px, py, color, genType);
                break;
            case BORDER:
                ChangeColor(color, GRAY);
                created = CreateParticle(pMap, px, py, color, genType);
                break;
            }
            if(created)
                partList->list[partList->elems++] = &pMap[py][px];
                //printf("elems: %d\n", partList.elems);
        }
    }

    if(event.type == SDL_KEYDOWN){
        // printf("works\n");
        if(event.key.keysym.sym == SDLK_ESCAPE) *isrunning = 0;
        if(event.key.keysym.sym == SDLK_a){
            ClearMap(pMap, simRows, simCols);
        }
        if(event.key.keysym.sym == SDLK_c){
            currentColor = (currentColor + 1) % colorSeqSize;
            ChangeColor(color, ColorSeq[currentColor]);
        }
        if(event.key.keysym.sym == SDLK_x){
            currentPart = (currentPart + 1) % partSeqSize;
            genType = PartSeq[currentPart];
        }
    }
}