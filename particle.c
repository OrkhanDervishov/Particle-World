#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

//Third party
#include <SDL2/SDL.h>


#define SCR_WIDTH 1080
#define SCR_HEIGHT 720
#define TITLE "SDL Playground"
#define MAX_TITLE_LEN 64
#define PART_SIDE 2
#define DELAY 2
#define RADIUS 4


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255

#define SWAP(a, b, t) t = a, a = b, b = t 


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
    PURPLE,
    BROWN
} Colors;


typedef struct
{
    int x, y;
} Pos;


typedef struct{
    int r, g, b, a;
} Color;


typedef struct{
    Pos p;
    int id;
    Color c;
    PartType t;
    int dens;
    int vel;
} Particle;


typedef struct{
    Particle** list;
    size_t size;
    size_t elems;
} PartList;


typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int isrunning;
    char title[MAX_TITLE_LEN];
    int w;
    int h;    
} Window;


typedef struct
{
    int pSide;
    int rows;
    int cols;
    Particle** pMap;
    PartList pList;
} Simulator;


//Pragram params
int mode = 1;
int drawlines = 0;


//Color
Color ZeroColor = {0, 0, 0, 255};
Colors ColorSeq[] = {BLUE, RED, GREEN, YELLOW, LYELLOW, PURPLE};
int currentColor = 0;
int colorSeqSize = 6;

//Particles
// PartList pList;
Colors PartSeq[] = {SAND, WATER, BORDER};
int currentPart = 0;
int partSeqSize = 3;
int bound;
PartType genType = SAND;

// Declarations

// Inits
int InitWindow(Window** win, int w, int h, const char* title);
void DestroyWindow(Window** win);
int InitSimulator(Simulator** sim, int w, int h, int ps);
void DestroySimulator(Simulator** sim);

// System
void** Malloc2D(int rows, int cols, int elemSize);

// Drawing
void DrawCage(Window* win, Simulator* sim, int r, int g, int b, int a);
void DrawScene(Window* win, Simulator* sim);
void ClearMap(Simulator* sim);

// Other
clock_t FindDelta(clock_t old);
void ProcessInput(SDL_Event event, Window* win, Simulator* sim, Color* color);

// Color
void ChangeColor(Color* c, Colors cs);
void CopyColor(Color* dest, Color* src);
int CompColors(Color* c1, Color* c2);

// Simulation
void SandBehave(int* x, int* y, PartType l, PartType ld, PartType d, PartType r, PartType rd);
void LiquidBehave(int* x, int* y, PartType l, PartType ld, PartType d, PartType r, PartType rd);
void Simulate(Simulator* sim);

// Particle
int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type);
void CreateManyParticles(Simulator* sim, int px, int py, int rad, Color* color, PartType type);
void SwapParticles(Simulator* sim, Particle* p1, Particle* p2);


int main(int argc, char* argv[]){
    
    // Inits
    Simulator* sim;
    Window* win;
    if(InitWindow(&win, SCR_WIDTH, SCR_HEIGHT, TITLE)) return 1;
    if(InitSimulator(&sim, SCR_WIDTH, SCR_HEIGHT, PART_SIDE)) return 1;

    Color color;
    ChangeColor(&color, BLACK);

    // Loop
    SDL_Event event;
    while(win->isrunning){
        SDL_SetRenderDrawColor(win->renderer, MWHITE);
        SDL_RenderClear(win->renderer);
        
        // Input handling
        while(SDL_PollEvent(&event)){
            ProcessInput(event, win, sim, &color);
        }

        // Render
        if(drawlines){
            DrawCage(win, sim, MBLACK);
        }

        DrawScene(win, sim);

        // Calculations
        if(mode == 1)
            Simulate(sim);

        
        // End Frame
        SDL_RenderPresent(win->renderer);
        SDL_Delay(DELAY);
    }


    // Destroy
    DestroySimulator(&sim);
    DestroyWindow(&win);
    return 0;
}



// Implementations

int InitWindow(Window** win, int w, int h, const char* title){
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

    if((*win)->window == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    (*win)->renderer = SDL_CreateRenderer((*win)->window, -1, SDL_RENDERER_SOFTWARE);

    if((*win)->renderer == NULL){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}


void DestroyWindow(Window** win){
    SDL_DestroyRenderer((*win)->renderer);
    SDL_DestroyWindow((*win)->window);
    free(*win);
    SDL_Quit();
}


int InitSimulator(Simulator** sim, int w, int h, int ps){

    *sim = (Simulator*)malloc(sizeof(Simulator));
    
    (*sim)->pSide = ps;
    (*sim)->rows = h/ps;
    (*sim)->cols = w/ps;
    bound = h - ps;

    (*sim)->pList.elems = 0;
    (*sim)->pList.size = (*sim)->rows * (*sim)->cols;
    (*sim)->pList.list = (Particle**)malloc((*sim)->pList.size * sizeof(Particle*));
    (*sim)->pMap = (Particle**)Malloc2D((*sim)->rows, (*sim)->cols, sizeof(Particle));

    if((*sim)->pList.list == NULL) return 1;
    if((*sim)->pMap == NULL) return 1;

    ClearMap((*sim));
    return 0;
}


void DestroySimulator(Simulator** sim){
    for(int i = 0; i < (*sim)->rows; i++){
        free((*sim)->pMap[i]);
    }
    free((*sim)->pMap);

    free((*sim)->pList.list);
    free(*sim);
}


void ClearMap(Simulator* sim){

    for(size_t i = 0; i < sim->rows; i++){
        for(size_t j = 0; j < sim->cols; j++){
            sim->pMap[i][j].id = -1;
            sim->pMap[i][j].t = EMPTY;
        }
    }
    sim->pList.elems = 0;
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
    case BROWN:
        c->r = 150, c->g = 75, c->b = 0, c->a = 255;
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


void Simulate(Simulator* sim){

    // if(load < 10000) return;   
    // load = 0;

    int a = 0;
    for(size_t k = 0; k < sim->pList.elems; k++){

        int j = (sim->pList.list[k])->p.x;
        int i = (sim->pList.list[k])->p.y;

        if(sim->pMap[i][j].p.y >= sim->rows-1 || sim->pMap[i][j].t == EMPTY || sim->pMap[i][j].t == BORDER) {continue;}
        int* x = &sim->pMap[i][j].p.x;
        int* y = &sim->pMap[i][j].p.y;

        PartType type = sim->pMap[i][j].t;

        if(type == SAND){  
            SandBehave(
                x, 
                y, 
                ((*x > 0) ? sim->pMap[i][j - 1].t : BORDER),
                ((*x > 0) ? sim->pMap[i + 1][j - 1].t : BORDER), 
                sim->pMap[i + 1][j].t,
                ((*x < sim->cols - 1) ? sim->pMap[i][j + 1].t : BORDER),
                ((*x < sim->cols - 1) ? sim->pMap[i + 1][j + 1].t : BORDER));
        }
        else if(type == WATER){
            LiquidBehave(
                x, 
                y, 
                ((*x > 0) ? sim->pMap[i][j - 1].t : BORDER),
                ((*x > 0) ? sim->pMap[i + 1][j - 1].t : BORDER), 
                sim->pMap[i + 1][j].t,
                ((*x < sim->cols - 1) ? sim->pMap[i][j + 1].t : BORDER),
                ((*x < sim->cols - 1) ? sim->pMap[i + 1][j + 1].t : BORDER));
        }

        if(*y == i && *x == j) continue;
        sim->pList.list[k] = &sim->pMap[*y][*x];
        sim->pMap[i][j].t = type;
        sim->pMap[*y][*x] = sim->pMap[i][j];
        sim->pMap[i][j].id = -1;
        sim->pMap[i][j].t = EMPTY;
    }
}


clock_t FindDelta(clock_t old){
    return clock() - old;
}




void DrawScene(Window* win, Simulator* sim){
    
    for(size_t i = 0; i < sim->pList.elems; i++){
        SDL_SetRenderDrawColor(
            win->renderer,
            sim->pList.list[i]->c.r, 
            sim->pList.list[i]->c.g, 
            sim->pList.list[i]->c.b, 
            sim->pList.list[i]->c.a);
        SDL_Rect r = {
            sim->pList.list[i]->p.x * sim->pSide + 1, 
            sim->pList.list[i]->p.y * sim->pSide + 1, 
            sim->pSide - 1, 
            sim->pSide - 1};
        SDL_RenderFillRect(win->renderer, &r);                          
    }
}


void DrawCage(
    Window* win, 
    Simulator* sim,
    int r, 
    int g, 
    int b, 
    int a)
{
    SDL_SetRenderDrawColor(win->renderer, r, g, b, a);

    for(int i = 0; i < win->w; i += sim->pSide){
        SDL_RenderDrawLine(win->renderer, i, 0, i, win->h);
    }
    
    for(int i = 0; i < win->h; i += sim->pSide){
        SDL_RenderDrawLine(win->renderer, 0, i, win->w, i);
    }
}


void ProcessInput(
    SDL_Event event, 
    Window* win, 
    Simulator* sim,
    Color* color
)
{
    //printf("works\n");
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    if(event.type == SDL_QUIT) win->isrunning = 0;
    // if(event.type == SDL_MOUSEMOTION) SDL_GetMouseState(&mx, &my);
    if(event.button.button == SDL_BUTTON_LEFT){
        // printf("works\n");
        int created = 0;
        int px = ((mx) / sim->pSide);
        int py = ((my) / sim->pSide);
        if(mode == 0)
            CreateParticle(sim, px, py, color, genType);
        if(mode == 1){

            switch (genType)
            {
            case SAND:
                ChangeColor(color, BROWN);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType);
                break;
            case WATER:
                ChangeColor(color, BLUE);
                //CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType);
                break;
            case BORDER:
                ChangeColor(color, GRAY);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType);
                break;
            }
        }
    }

    if(event.type == SDL_KEYDOWN){
        // printf("works\n");
        if(event.key.keysym.sym == SDLK_ESCAPE) win->isrunning = 0;
        if(event.key.keysym.sym == SDLK_a){
            ClearMap(sim);
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


int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type){
    if(px >= 0 && py >= 0 && px < sim->cols && py < sim->rows){
        //printf("works\n");
        // printf("px: %d, py: %d\n", px, py);
        if(sim->pMap[py][px].id != -1) return 0;
        sim->pMap[py][px].p.x = px;
        sim->pMap[py][px].p.y = py;
        sim->pMap[py][px].id = 1;
        CopyColor(&sim->pMap[py][px].c, color);
        sim->pMap[py][px].t = type;
        sim->pList.list[sim->pList.elems++] = &sim->pMap[py][px];
        return 1;
    }
    return 0;
}

void CreateManyParticles(Simulator* sim, int px, int py, int rad, Color* color, PartType type){
    int begy = py - rad;
    int begx = px - rad;
    int endy = py + rad;
    int endx = px + rad;

    for(int i = begy; i < endy; i++){
        for(int j = begx; j < endx; j++){
            CreateParticle(sim, j, i, color, type);
        }
    }
}


void SwapParticles(Simulator* sim, Particle* p1, Particle* p2){
    Particle tp = *p1;
    sim->pMap[p1->p.y][p1->p.x] = sim->pMap[p2->p.y][p2->p.x];
    sim->pMap[p2->p.y][p2->p.x] = tp;

    int t;
    SWAP(p1->p.x, p1->p.x, t);
    SWAP(p2->p.x, p2->p.x, t);
}