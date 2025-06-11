#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

//Third party
#include <SDL2/SDL.h>


#define SCR_WIDTH 1080
#define SCR_HEIGHT 720
#define TITLE "SDL Playground"
#define MAX_TITLE_LEN 64
#define PART_SIDE 4
#define DELAY 8
#define RADIUS 2
#define STEAM_DISAPPEAR_TIME 400


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255
#define MGOOD_GRAY 58, 58, 58, 255


#define PARTICLE_GENERATION_BUTTON (event.button.button == SDL_BUTTON_LEFT)
#define PARTICLE_CHANGE_MOUSE_BUTTON (event.button.button == SDL_BUTTON_RIGHT)
#define SCREEN_CLEAR_BUTTON (event.key.keysym.sym == SDLK_c)
#define COLOR_CHANGE_BUTTON (event.key.keysym.sym == SDLK_x)
#define PARTICLE_CHANGE_BUTTON (event.key.keysym.sym == SDLK_v)
#define QUIT_BUTTON (event.key.keysym.sym == SDLK_ESCAPE)

#define SWAP(a, b, t) t = a, a = b, b = t 


typedef enum{
    AIR = 0,
    STEAM,
    SAND,
    FUNGUS,
    WATER,
    ACID,
    FIRE,
    WALL
} PartType;

typedef enum{
    AIR_DENSITY = 50,
    STEAM_DENSITY = 20,
    WATER_DENSITY = 100,
    ACID_DENSITY = 10000,
    SAND_DENSITY = 500,
    FUNGUS_DENSITY = 400,
    WALL_DENSITY = 1000000
} PartDensity;


typedef enum{
    WHITE,
    BLACK,
    RED,
    PINK,
    GREEN,
    BLUE,
    LIGHT_BLUE,
    YELLOW,
    LYELLOW,
    GRAY,
    GOOD_GRAY,
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
    char id;
    Color c;
    PartType t;
    int dens;
    char velo;
    int sim_t;
} Particle;


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
} Simulator;


// Main objects
Simulator* sim;
Window* win;


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
Colors PartSeq[] = {SAND, FUNGUS, WATER, ACID, STEAM, WALL};
int currentPart = 0;
int partSeqSize = 6;
int bound;
PartType genType = SAND;

// Declarations
int* shuffledIndexesX;
int* shuffledIndexesY;
int mov = 0;

Particle air = {{-1, -1}, -1, {255, 255, 255, 255}, AIR, AIR_DENSITY, 0, 0};

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
void SandBehave(Simulator* sim, int* x, int* y);
void LiquidBehave(Simulator* sim, int* x, int* y);
void SteamBehave(Simulator* sim, int* x, int* y);
void FungusBehave(Simulator* sim, int* x, int* y);
void Simulate(Simulator* sim);

// Particle
int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens);
void CreateManyParticles(Simulator* sim, int px, int py, int rad, Color* color, PartType type, int dens);
void DeleteParticle(Simulator* sim, int px, int py);
void DeleteManyParticles(Simulator* sim, int px, int py, int rad);
void SwapParticles(Simulator* sim, Particle* p1, Particle* p2);

void SetCircle(int** circle, int rad);
void SetFilledCircle(int** filledCircle, int rad);
void AddCircle(Window* win, Simulator* sim, int* circle, int rad, int x, int y);

void CreateShuffleIndexes(int rows, int cols);
void DeleteSuffledIndexes();

void WallBox(Simulator* sim);

int main(int argc, char* argv[]){
    
    // Inits
    if(InitWindow(&win, SCR_WIDTH, SCR_HEIGHT, TITLE)) return 1;
    if(InitSimulator(&sim, SCR_WIDTH, SCR_HEIGHT, PART_SIDE)) return 1;
    WallBox(sim);
    Color color;
    ChangeColor(&color, BLACK);

    // Loop
    SDL_Event event;
    while(win->isrunning){
        
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
        if(mode == 1){
            Simulate(sim);
        }
        
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

    (*sim)->pMap = (Particle**)Malloc2D((*sim)->rows, (*sim)->cols, sizeof(Particle));

    if((*sim)->pMap == NULL) return 1;

    ClearMap((*sim));
    return 0;
}


void DestroySimulator(Simulator** sim){
    for(int i = 0; i < (*sim)->rows; i++){
        free((*sim)->pMap[i]);
    }
    free((*sim)->pMap);

    free(*sim);
}


void ClearMap(Simulator* sim){

    for(size_t i = 0; i < sim->rows; i++){
        for(size_t j = 0; j < sim->cols; j++){
            sim->pMap[i][j].id = -1;
            sim->pMap[i][j].t = AIR;
            sim->pMap[i][j].dens = AIR_DENSITY;
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
    case PINK:
        c->r = 252, c->g = 70, c->b = 170, c->a = 255;
        break;
    case GREEN:
        c->r = 0, c->g = 255, c->b = 0, c->a = 255;
        break;
    case BLUE:
        c->r = 0, c->g = 0, c->b = 255, c->a = 255;
        break;
    case LIGHT_BLUE:
        c->r = 150, c->g = 150, c->b = 255, c->a = 255;
        break;
    case YELLOW:
        c->r = 255, c->g = 255, c->b = 0, c->a = 255;
        break;
    case LYELLOW:
        c->r = 255, c->g = 255, c->b = 100, c->a = 255;
        break;
    case GRAY:
        c->r = 135, c->g = 135, c->b = 135, c->a = 255;
        break;
    case GOOD_GRAY:
        c->r = 58, c->g = 58, c->b = 58, c->a = 255;
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


int swapped = 0;


void SandBehave(Simulator* sim, int* x, int* y){
    Particle* p = &sim->pMap[*y][*x];
    Particle* d = &sim->pMap[*y + 1][*x];
    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    Particle* rd = &sim->pMap[*y + 1][*x + 1];

    if(d->dens < p->dens){ (*y) += 1; }
    else if(ld->dens < p->dens){ (*y) += 1, (*x) -= 1; }
    else if(rd->dens < p->dens){ (*y) += 1, (*x) += 1;}
}

void LiquidBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];

    Particle* d = &sim->pMap[*y + 1][*x];
    if(d->dens < p->dens){ (*y) += 1; return;}

    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    if(ld->dens < p->dens){ (*y) += 1, (*x) -= 1; return;}
    Particle* rd = &sim->pMap[*y + 1][*x + 1];
    if(rd->dens < p->dens){ (*y) += 1, (*x) += 1; return;}

    Particle* l = &sim->pMap[*y][*x - 1];
    if(l->dens < p->dens){ (*x) -= 1; return;}
    Particle* r = &sim->pMap[*y][*x + 1];
    if(r->dens < p->dens){ (*x) += 1; return;}
}

void SteamBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    Particle* u = &sim->pMap[*y - 1][*x];
    Particle* lu = &sim->pMap[*y - 1][*x - 1];
    Particle* ru = &sim->pMap[*y - 1][*x + 1];
    Particle* l = &sim->pMap[*y][*x - 1];
    Particle* r = &sim->pMap[*y][*x + 1];

    p->sim_t++;
    if(p->sim_t > STEAM_DISAPPEAR_TIME){p->id = -2; return;}

    if(u->dens > p->dens){ (*y) -= 1; }
    else if(lu->dens > p->dens){ (*y) -= 1, (*x) -= 1; }
    else if(ru->dens > p->dens){ (*y) -= 1, (*x) += 1;}
    else if(l->dens > p->dens){ (*x) -= 1; }
    else if(r->dens > p->dens){ (*x) += 1;}
}

void FungusBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    Particle* d = &sim->pMap[*y + 1][*x];

    if(d->dens < p->dens){ (*y) += 1; }
}

void Simulate(Simulator* sim){

    for(int i = sim->rows - 1; i >= 0; i--){
        mov = mov ? 0 : 1;
        int nj = mov ? sim->cols - 1 : 0;
        for(int j = nj; mov ? j >= 0 : j < sim->cols; mov ? j-- : j++){
            
            int oldx = j;
            int oldy = i;

            if(sim->pMap[oldy][oldx].id > 0){
                PartType type = sim->pMap[oldy][oldx].t;
                if(sim->pMap[oldy][oldx].p.y >= sim->rows-1 || type == AIR || type == WALL) {continue;}
                
                int x = oldx;
                int y = oldy;

                // Particle logic
                switch (type)
                {
                case SAND:
                    SandBehave(sim, &x, &y);
                    break;
                case FUNGUS:
                    FungusBehave(sim, &x, &y);
                    break;
                case WATER:
                    LiquidBehave(sim, &x, &y);
                    break;
                case ACID:
                    LiquidBehave(sim, &x, &y);
                    break;
                case STEAM:
                    SteamBehave(sim, &x, &y);
                    break;
                }


                // Particle swapping
                if(y == oldy && x == oldx){
                    if(sim->pMap[y][x].id == -2){
                        sim->pMap[y][x].t = AIR;
                        sim->pMap[y][x].dens = AIR_DENSITY;
                        sim->pMap[y][x].id = -1;
                        continue;
                    }
                    continue;
                }
                if(x < 0 || x >= sim->cols) continue;
                if(y < 0 || y >= sim->rows) continue;
                if(sim->pMap[y][x].t == WALL) continue;

                if(type == ACID){
                    if(sim->pMap[y][x].t != AIR){
                        sim->pMap[y][x] = air;
                        sim->pMap[oldy][oldx] = air;
                        continue;
                    }
                }
                Particle temp = sim->pMap[y][x];
                sim->pMap[y][x] = sim->pMap[oldy][oldx];
                sim->pMap[oldy][oldx] = temp;
                sim->pMap[y][x].p.x = x;
                sim->pMap[y][x].p.y = y;
                sim->pMap[oldy][oldx].p.x = oldx;
                sim->pMap[oldy][oldx].p.y = oldy;
                sim->pMap[y][x].id = 0;
            }
        }
    }


    for(int i = 0; i < sim->rows; i++){
        for(int j = 0; j < sim->cols; j++){
            if(sim->pMap[i][j].id == 0) sim->pMap[i][j].id = 1;
        }
    }
}


clock_t FindDelta(clock_t old){
    return clock() - old;
}

void DrawScene(Window* win, Simulator* sim){
    
    SDL_SetRenderDrawColor(win->renderer, MGOOD_GRAY);
    SDL_RenderClear(win->renderer);

    for(int i = 0; i < sim->rows; i++){
        for(int j = 0; j < sim->cols; j++){
            if(sim->pMap[i][j].id < 0) continue;
            SDL_SetRenderDrawColor(
                win->renderer,
                sim->pMap[i][j].c.r,
                sim->pMap[i][j].c.g,
                sim->pMap[i][j].c.b,
                sim->pMap[i][j].c.a
            );
            SDL_Rect r = {
                sim->pMap[i][j].p.x * sim->pSide + 1, 
                sim->pMap[i][j].p.y * sim->pSide + 1, 
                sim->pSide, 
                sim->pSide};
            SDL_RenderFillRect(win->renderer, &r); 
        }
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
    if(PARTICLE_GENERATION_BUTTON){
        // printf("works\n");
        int created = 0;
        int px = ((mx) / sim->pSide);
        int py = ((my) / sim->pSide);
        if(mode == 0)
            CreateParticle(sim, px, py, color, genType, WALL_DENSITY);
        if(mode == 1){

            switch (genType)
            {
            case SAND:
                ChangeColor(color, BROWN);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, SAND_DENSITY);
                break;
            case FUNGUS:
                ChangeColor(color, PINK);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, SAND_DENSITY);
                break;
            case WATER:
                ChangeColor(color, BLUE);
                //CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, WATER_DENSITY);
                break;
            case ACID:
                ChangeColor(color, GREEN);
                //CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, ACID_DENSITY);
                break;
            case WALL:
                ChangeColor(color, GRAY);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, WALL_DENSITY);
                break;
            case STEAM:
                ChangeColor(color, LIGHT_BLUE);
                // created = CreateParticle(sim, px, py, color, genType);
                CreateManyParticles(sim, px, py, RADIUS, color, genType, STEAM_DENSITY);
                break;
            }
        }
    }

    if(PARTICLE_CHANGE_MOUSE_BUTTON){
        int px = ((mx) / sim->pSide);
        int py = ((my) / sim->pSide);
        DeleteManyParticles(sim, px, py, RADIUS);
    }

    if(event.type == SDL_KEYDOWN){
        // printf("works\n");
        if(QUIT_BUTTON) win->isrunning = 0;
        if(SCREEN_CLEAR_BUTTON){
            ClearMap(sim);
            WallBox(sim);
        }
        if(COLOR_CHANGE_BUTTON){
            currentColor = (currentColor + 1) % colorSeqSize;
            ChangeColor(color, ColorSeq[currentColor]);
        }
        if(PARTICLE_CHANGE_BUTTON){
            currentPart = (currentPart + 1) % partSeqSize;
            genType = PartSeq[currentPart];
        }
    }
}


int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens){
    if(px >= 0 && py >= 0 && px < sim->cols && py < sim->rows){
        //printf("works\n");
        // printf("px: %d, py: %d\n", px, py);
        if(sim->pMap[py][px].id != -1) return 0;
        Particle part;
        part.p.x = px;
        part.p.y = py;
        part.id = 1;
        CopyColor(&part.c, color);
        part.t = type;
        part.velo = 0;
        part.dens = dens;
        part.sim_t = 0;

        sim->pMap[py][px] = part;
        return 1;
    }
    return 0;
}

void CreateManyParticles(Simulator* sim, int px, int py, int rad, Color* color, PartType type, int dens){
    int begy = py - rad;
    int begx = px - rad;
    int endy = py + rad;
    int endx = px + rad;

    for(int i = begy; i < endy; i++){
        for(int j = begx; j < endx; j++){
            CreateParticle(sim, j, i, color, type, dens);
        }
    }
}

void DeleteParticle(Simulator* sim, int px, int py){
    sim->pMap[py][px].id = -1;
    sim->pMap[py][px].t = AIR;
    sim->pMap[py][px].dens = AIR_DENSITY;
}

void DeleteManyParticles(Simulator* sim, int px, int py, int rad){
    int begy = py - rad;
    int begx = px - rad;
    int endy = py + rad;
    int endx = px + rad;

    for(int i = begy; i < endy; i++){
        for(int j = begx; j < endx; j++){
            DeleteParticle(sim, j, i);
        }
    }
}


void SwapParticles(Simulator* sim, Particle* p1, Particle* p2){
    Particle tp;
    int t;

    SWAP(sim->pMap[p1->p.y][p1->p.x], sim->pMap[p2->p.y][p2->p.x], tp);
    SWAP(p1->p.x, p2->p.x, t);
    SWAP(p1->p.y, p2->p.y, t);
}


void SetCircle(int** circle, int rad){
    if((*circle) != NULL) return;
    int size = sizeof(int) * rad*rad;
    (*circle) = (int*)malloc(size);
    memset((*circle), 0, size);

    float frad = (float)rad;

    int side = 2 * rad;
    float fi = -5.0f;
    for(int i = 0; i < side; i++){
        float fj = -5.0f;
        for(int j = 0; j < side; j++){
            if(sqrtf(fi*fi + fj*fj) == frad) (*circle)[i*rad + j] = 1;
            fj += 1.0f;
        }
        fi += 1.0f;
    }
}

void SetFilledCircle(int** filledCircle, int rad){
    if((*filledCircle) != NULL) return;
    int size = sizeof(int) * (2*rad + 1)*(2*rad + 1);
    (*filledCircle) = (int*)malloc(size);
    memset((*filledCircle), 0, size);

    int x = rad;
    int y = rad;
    int side = 2*rad + 1;
    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++){
            int dx = abs(x - j);
            int dy = abs(y - i);
            if(dx*dx + dy*dy <= rad * rad) (*filledCircle)[i*side + j] = 1;
        }
    }
}


void AddCircle(Window* win, Simulator* sim, int* circle, int rad, int x, int y){
    // SDL_SetRenderDrawColor(sim->
    if(sim == NULL || circle == NULL) return;
    int startx = x - rad - 1;
    int starty = y - rad - 1;
    int endx = x + rad;
    int endy = y + rad;
    printf("%d\n", endx - startx);
    printf("%d\n", endy - starty);

    for(int i = starty; i < endy; i++){
        for(int j = startx; j < endx; j++){
            int pos = (i - starty)*(2*rad + 1) + (j - startx);
            if(circle[pos]){
                SDL_Rect rect;
                rect.x = j;
                rect.y = i;
                rect.w = sim->pSide;
                rect.h = sim->pSide;
                SDL_RenderDrawRect(win->renderer, &rect);
            }
            if(!circle[pos + 1]) break;
        }
    }
}

void WallBox(Simulator* sim){
    Color c;
    ChangeColor(&c, GRAY);

    for(int j = 0; j < sim->cols; j++){
        CreateParticle(sim, j, 0, &c, WALL, WALL_DENSITY);
    }
    for(int j = 0; j < sim->cols; j++){
        CreateParticle(sim, j, sim->rows - 1, &c, WALL, WALL_DENSITY);
    }
    for(int i = 1; i < sim->rows - 1; i++){
        CreateParticle(sim, 0, i, &c, WALL, WALL_DENSITY);
        CreateParticle(sim, sim->cols - 1, i, &c, WALL, WALL_DENSITY);
    }

}
