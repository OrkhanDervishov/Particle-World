#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

//Third party
#include <SDL2/SDL.h>


#define SCR_WIDTH 1080
#define SCR_HEIGHT 720

#define WIN_TITLE "SDL Playground"
#define MAX_TITLE_LEN 64
#define ICON_PATH "./images/sand.bmp"

#define NUMBERS_OF_ELEMENTS 11
#define MAX_NUMBER_OF_TYPES 256

// GUI macros
#define MAX_BUTTONS 100
#define ELEMENT_BUTTON_SIZE 16
#define ELEMENT_BUTTONS_MARGIN 4
#define ELEMENT_BUTTON_MARGIN_H 16
#define ELEMENT_BUTTON_MARGIN_W 30

#define M_SAND_BUTTON_COLOR 255, 255, 0, 255
#define M_COAL_BUTTON_COLOR 0, 0, 0, 255
#define M_WATER_BUTTON_COLOR 0, 0, 255, 255
#define M_FUNGUS_BUTTON_COLOR 252, 70, 170, 255
#define M_ACID_BUTTON_COLOR 0, 255, 0, 255
#define M_WALL_BUTTON_COLOR 100, 100, 100, 255
#define M_STEAM_BUTTON_COLOR 150, 150, 255, 255
#define M_FIRE_BUTTON_COLOR 255, 0, 0, 255
#define M_SMOKE_BUTTON_COLOR 40, 40, 40, 255
#define M_OIL_BUTTON_COLOR 131, 66, 0, 255
#define M_LAVA_BUTTON_COLOR 255, 69, 0, 255

// Simulator macros
#define PART_SIDE 4
#define DELAY 8
#define RADIUS 6
#define STEAM_LIFE_TIME 350
#define SMOKE_LIFE_TIME 300
#define ACID_EFFECT_TIME 50
#define FIRE_LIFE_TIME 100
#define FIRE_EFFECT_TIME 20

#define FIRE_HEAT_RELEASE_TEMP 40
#define WATER_TO_STEAM_TEMP 400
#define COAL_TO_FIRE_TEMP 1000
#define OIL_TO_FIRE_TEMP 40

#define WATER_HEAT_STEAL 200


#define CSIDE 4


#define MWHITE 255, 255, 255, 255
#define MBLACK 0, 0, 0, 255
#define MRED 255, 0, 0, 255
#define MGREEN 0, 255, 0, 255
#define MBLUE 0, 0, 255, 255
#define MGOOD_GRAY 58, 58, 58, 255


#define PARTICLE_GENERATION_BUTTON (event.button.button == SDL_BUTTON_LEFT)
#define DELETE_PARTICLE_BUTTON (event.button.button == SDL_BUTTON_RIGHT)
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
    OIL,
    FIRE,
    WALL,
    SMOKE,
    COAL,
    LAVA
} PartType;

typedef enum{
    AIR_DENSITY = 50,
    STEAM_DENSITY = 40,
    SMOKE_DENSITY = 45,
    WATER_DENSITY = 200,
    ACID_DENSITY = 190,
    OIL_DENSITY = 180,
    SAND_DENSITY = 500,
    COAL_DENSITY = 350,
    FUNGUS_DENSITY = 250,
    FIRE_DENSITY = 80,
    LAVA_DENSITY = 400,
    WALL_DENSITY = 1000000
} PartDensity;


#define CHECK_FLAG(p, f) ((p)->flags & (f) ? 1 : 0)
#define CHECK_FLAG(p, f) ((p)->flags & (f) ? 1 : 0)


#define IS_DUST 0x80000000
#define IS_LIQUID 0x40000000
#define IS_GAS 0x20000000
#define IS_SOLID 0x10000000

#define ACID_HAS_AN_EFFECT 0x08000000
#define FIRE_HAS_AN_EFFECT 0x04000000
#define HEAT_STEALER 0x02000000
#define HEAT_RELEASER 0x01000000


typedef enum{
    F_AIR = (uint32_t)0x20000000,
    F_STEAM = (uint32_t)0x20000000,
    F_SAND = (uint32_t)0x88000000,
    F_FUNGUS = (uint32_t)0x8C000000,
    F_COAL = (uint32_t)0x8C000000,
    F_WATER = (uint32_t)0x42000000,
    F_ACID = (uint32_t)0x40000000,
    F_OIL = (uint32_t)0x44000000,
    F_FIRE = (uint32_t)0x11000000,
    F_WALL = (uint32_t)0x10000000,
    F_SMOKE = (uint32_t)0x20000000,
    F_LAVA = (uint32_t)0x41000000
} PartFlags;


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
    ORANGE,
    GRAY,
    GOOD_GRAY,
    PURPLE,
    BROWN,
    OIL_BROWN,
    SAND_COLORS,
    WATER_COLORS,
    WALL_COLORS,
    ACID_COLORS,
    FIRE_COLORS,
    COAL_COLORS,
} Colors;


typedef struct
{
    int x, y;
} Pos;


typedef union{
    struct{
        uint8_t r, g, b, a;
    };
    uint32_t rgba;
} Color;

/* 
Flags
from left to right
0 - dust, 1 - liquid, 2 - gas, 3 - solid
4 - acid has an effect, 5 - flamable
*/
typedef struct{
    Pos p;
    int8_t id;
    Color c;
    PartType t;
    int type;
    uint16_t dens;
    uint8_t xvel, yvel;
    uint16_t effect_t;
    uint16_t life_t;
    uint16_t heat;
    uint32_t flags;
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
    int partCount;
    Particle** pMap;
} Simulator;


// Main objects
Simulator* sim;
Window* win;


//Pragram params
int mode = 1;
int drawlines = 0;
int Delay = 8;
int RAD = 6;
int fps = 0;

//Color

//Particles
PartType PartSeq[] = {SAND, WATER, FUNGUS, ACID, WALL, STEAM, FIRE, SMOKE, COAL, OIL, LAVA};
int currentPart = 0;
int partSeqSize = NUMBERS_OF_ELEMENTS;
int bound;
PartType genType = SAND;

// Declarations
int* shuffledIndexesX;
int* shuffledIndexesY;
int mov = 0;

// Input
int delmode = 0;
int genmode = 0;

SDL_Rect buttons[MAX_BUTTONS];
int buttonCount = 0;


// Particle type params
// They store common data of particle types
void (*typeFuncList[MAX_NUMBER_OF_TYPES])(Simulator* sim, int* x, int* y);
Color typeButtonColorList[MAX_NUMBER_OF_TYPES];
Colors typeColorList[MAX_NUMBER_OF_TYPES];
int typeDensityList[MAX_NUMBER_OF_TYPES];
uint32_t typeFlagsList[MAX_NUMBER_OF_TYPES];
char* typeNameList[MAX_NUMBER_OF_TYPES];
int countTypes = 0;
int selectedType = 0;



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
void DrawCircle(Simulator* sim, int mx, int my, int rad);
void ClearMap(Simulator* sim);

// Other
clock_t CalcDelta(clock_t old);
void ProcessInput(SDL_Event event, Window* win, Simulator* sim, Color* color);

// Color
void ChangeColor(Color* c, Colors cs);
void CopyColor(Color* dest, Color* src);
int CompColors(Color* c1, Color* c2);

// Simulation
void SandBehave(Simulator* sim, int* x, int* y);
void WaterBehave(Simulator* sim, int* x, int* y);
void OilBehave(Simulator* sim, int* x, int* y);
void AcidBehave(Simulator* sim, int* x, int* y);
void SteamBehave(Simulator* sim, int* x, int* y);
void FungusBehave(Simulator* sim, int* x, int* y);
void FireBehave(Simulator* sim, int* x, int* y);
void CoalBehave(Simulator* sim, int* x, int* y);
void LavaBehave(Simulator* sim, int* x, int* y);
void Simulate(Simulator* sim);

// Particle
void InitTypes();
void AddType(char* name, Colors color, Color buttonColor, int dens, uint32_t flags, void (*func)(Simulator* sim, int* x, int* y));
int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens, PartFlags flags);
void CreateReplaceParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens, PartFlags flags);
void CreateManyParticles(Simulator* sim, int px, int py, int rad, Colors c, PartType type, int dens, PartFlags flags);
void DeleteParticle(Simulator* sim, int px, int py);
void DeleteManyParticles(Simulator* sim, int px, int py, int rad);
void SwapParticles(Simulator* sim, Particle* p1, Particle* p2);
void WallBox(Simulator* sim);
void ElementChange();

void CreateShuffleIndexes(int rows, int cols);
void DeleteSuffledIndexes();

void CreateGui(Window* win, SDL_Rect* buttons, int* buttonCount, int elementCount);
void DrawGui(Window* win, SDL_Rect* buttons, int buttonCount);
void CheckGuiButtons(Window* win, int mx, int my);

int main(int argc, char* argv[]){

    // Inits
    if(InitWindow(&win, SCR_WIDTH, SCR_HEIGHT, WIN_TITLE)) return 1;
    if(InitSimulator(&sim, SCR_WIDTH, SCR_HEIGHT, PART_SIDE)) return 1;
    WallBox(sim);
    Color color;
    ChangeColor(&color, BLACK);
    srand(time(NULL));
    CreateGui(win, buttons, &buttonCount, partSeqSize);

    SDL_Surface* icon = SDL_LoadBMP(ICON_PATH);
    if(icon == NULL) printf("fail\n");
    SDL_SetWindowIcon(win->window, icon);


    SDL_RendererInfo info;
    SDL_GetRendererInfo(win->renderer, &info);
    printf("%s\n", info.name);

    clock_t start = 0, end = 1, delta = 0;
    // Loop
    SDL_Event event;
    while(win->isrunning){
        delta += (end - start);

        if(delta > 400){
            fps = 1000 / (end - start);
            delta = 0;
        }
        // printf("time: %d\n", end - start);
        // printf("elapced: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);

        start = clock();

        sprintf(win->title, "Particles: %d FPS: %d", sim->partCount, fps);
        SDL_SetWindowTitle(win->window, win->title);

        // Input handling
        ProcessInput(event, win, sim, &color);

        // Render
        if(drawlines){
            DrawCage(win, sim, MBLACK);
        }

        DrawScene(win, sim);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        CheckGuiButtons(win, mx, my);
        DrawCircle(sim, mx, my, RADIUS);
        DrawGui(win, buttons, buttonCount);

        // Calculations
        if(mode == 1){
            Simulate(sim);
        }
        
        // End Frame
        SDL_RenderPresent(win->renderer);
        SDL_Delay(DELAY);

        end = clock();
    }


    // Destroy
    SDL_FreeSurface(icon);
    DestroySimulator(&sim);
    DestroyWindow(&win);
    return 0;
}



// Implementations

void Inittypes(){
    Color c;
    ChangeColor(&c, YELLOW);
    AddType("sand", SAND_COLORS, c, 500, 0x88000000, SandBehave);
    ChangeColor(&c, BLUE);
    AddType("water", WATER_COLORS, c, 200, 0x42000000, WaterBehave);
    ChangeColor(&c, LIGHT_BLUE);
    AddType("steam", LIGHT_BLUE, c, 40, 0x20000000, SteamBehave);
    ChangeColor(&c, GREEN);
    AddType("acid", ACID_COLORS, c, 190, 0x40000000, AcidBehave);
    ChangeColor(&c, GRAY);
    AddType("wall", WALL_COLORS, c, 1000000, 0x10000000, NULL);
    ChangeColor(&c, WHITE);
    AddType("air", WHITE, c, 50, 0x20000000, NULL);
    ChangeColor(&c, RED);
    AddType("fire", FIRE_COLORS, c, 80, 0x11000000, FireBehave);
    ChangeColor(&c, BLACK);
    AddType("smoke", BLACK, c, 45, 0x20000000, SteamBehave);
    ChangeColor(&c, BLACK);
    AddType("coal", COAL_COLORS, c, 350, 0x8C000000, CoalBehave);
    ChangeColor(&c, OIL_BROWN);
    AddType("oil", OIL_BROWN, c, 180, 0x44000000, OilBehave);
    ChangeColor(&c, ORANGE);
    AddType("lava", ORANGE, c, 400, 0x41000000, WaterBehave);
    ChangeColor(&c, PINK);
    AddType("fungus", PINK, c, 250, 0x8C000000, FungusBehave);
}

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
    (*sim)->partCount = 0;
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
            sim->pMap[i][j].flags = F_AIR;
            sim->pMap[i][j].heat = 0;
            sim->partCount = 0;
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

    int r = rand() % 4;
    switch (cs)
    {
    case WHITE:      c->rgba = 0xFFFFFFFF; break;
    case BLACK:      c->rgba = 0xFF000000; break;
    case RED:        c->rgba = 0xFF0000FF; break;
    case PINK:       c->rgba = 0xFFAA46FC; break;
    case GREEN:      c->rgba = 0xFF00FF00; break;
    case BLUE:       c->rgba = 0xFFFF0000; break;
    case LIGHT_BLUE: c->rgba = 0xFFFF9696; break;
    case YELLOW:     c->rgba = 0xFF00FFFF; break;
    case LYELLOW:    c->rgba = 0xFF64FFFF; break;
    case ORANGE:     c->rgba = 0xFF0045FF; break;
    case GRAY:       c->rgba = 0xFF878787; break;
    case GOOD_GRAY:  c->rgba = 0xFF3A3A3A; break;
    case PURPLE:     c->rgba = 0xFFFF00FF; break;
    case BROWN:      c->rgba = 0xFF4B9696; break;
    case OIL_BROWN:  c->rgba = 0xFF004283; break;

    case SAND_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFF00509B; break;
            case 1: c->rgba = 0xFF004191; break;
            case 2: c->rgba = 0xFF0046A0; break;
            case 3: c->rgba = 0xFF005096; break;
        }
        break;
    case WATER_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFFFA0000; break;
            case 1: c->rgba = 0xFFF00000; break;
            case 2: c->rgba = 0xFFE60000; break;
            case 3: c->rgba = 0xFFD80000; break;
        }
        break;
    case WALL_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFF878787; break;
            case 1: c->rgba = 0xFF919191; break;
            case 2: c->rgba = 0xFF9B9B9B; break;
            case 3: c->rgba = 0xFF7D7D7D; break;
        }
        break;
    case ACID_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFF00FA00; break;
            case 1: c->rgba = 0xFF00F000; break;
            case 2: c->rgba = 0xFF00E600; break;
            case 3: c->rgba = 0xFF00DC00; break;
        }
        break;
    case FIRE_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFF0000FF; break;
            case 1: c->rgba = 0xFF005AFF; break;
            case 2: c->rgba = 0xFF009AFF; break;
            case 3: c->rgba = 0xFF00CEFF; break;
        }
        break;
    case COAL_COLORS:
        switch (r) {
            case 0: c->rgba = 0xFF000000; break;
            case 1: c->rgba = 0xFF0A0A0A; break;
            case 2: c->rgba = 0xFF0F0F0F; break;
            case 3: c->rgba = 0xFF050505; break;
        }
        break;
    
    default:
        break;
    }
}

void CopyColor(Color* dest, Color* src){
    dest->rgba = src->rgba;
}

int CompColors(Color* c1, Color* c2){
    if(c1->rgba != c2->rgba) return 1;
    return 0;
}


void SandBehave(Simulator* sim, int* x, int* y){
    Particle* p = &sim->pMap[*y][*x];
    Particle* d = &sim->pMap[*y + 1][*x];
    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    Particle* rd = &sim->pMap[*y + 1][*x + 1];

    if((d->dens < p->dens) && !CHECK_FLAG(d, IS_DUST)){
        // Particle* l = &sim->pMap[*y][*x - 1];
        // if(l->id < 0) 
        (*y) += 1;
    }
    else if((ld->dens < p->dens) && !CHECK_FLAG(ld, IS_DUST)){ (*y) += 1, (*x) -= 1; }
    else if((rd->dens < p->dens) && !CHECK_FLAG(rd, IS_DUST)){ (*y) += 1, (*x) += 1;}
}

void CoalBehave(Simulator* sim, int* x, int* y){
    Particle* p = &sim->pMap[*y][*x];
    Particle* d = &sim->pMap[*y + 1][*x];
    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    Particle* rd = &sim->pMap[*y + 1][*x + 1];

    if(p->heat > COAL_TO_FIRE_TEMP){
        Color c;
        ChangeColor(&c, FIRE_COLORS);
        CreateReplaceParticle(sim, *x, *y, &c, FIRE, FIRE_DENSITY, F_FIRE);
    }

    if((d->dens < p->dens) && !CHECK_FLAG(d, IS_DUST)){
        // Particle* l = &sim->pMap[*y][*x - 1];
        // if(l->id < 0)
        (*y) += 1;
    }
    else if((ld->dens < p->dens) && !CHECK_FLAG(ld, IS_DUST)){ (*y) += 1, (*x) -= 1; }
    else if((rd->dens < p->dens) && !CHECK_FLAG(rd, IS_DUST)){ (*y) += 1, (*x) += 1;}
}

void WaterBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    if(p->heat > WATER_TO_STEAM_TEMP){
        Color c;
        ChangeColor(&c, LIGHT_BLUE);
        CreateReplaceParticle(sim, *x, *y, &c, STEAM, STEAM_DENSITY, F_STEAM);
    }

    Particle* d = &sim->pMap[*y + 1][*x];
    Particle* l = &sim->pMap[*y][*x - 1];
    Particle* r = &sim->pMap[*y][*x + 1];

    if(d->heat > p->heat && !CHECK_FLAG(d, HEAT_STEALER)){
        d->heat -= WATER_HEAT_STEAL;
        p->heat += WATER_HEAT_STEAL;
    }
    if(l->heat > p->heat && !CHECK_FLAG(l, HEAT_STEALER)){
        l->heat -= WATER_HEAT_STEAL;
        p->heat += WATER_HEAT_STEAL;
    }
    if(r->heat > p->heat && !CHECK_FLAG(r, HEAT_STEALER)){
        r->heat -= WATER_HEAT_STEAL;
        p->heat += WATER_HEAT_STEAL;
    }




    if(d->dens < p->dens){
        if(!CHECK_FLAG(d, IS_DUST) && !CHECK_FLAG(d, IS_SOLID)){
            (*y) += 1;
            return;
        }
    }

    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    if(ld->dens < p->dens){
        if(!CHECK_FLAG(ld, IS_SOLID)){
            (*y) += 1, (*x) -= 1; 
            return;
        }
    }
    Particle* rd = &sim->pMap[*y + 1][*x + 1];
    if(rd->dens < p->dens){
        if(!CHECK_FLAG(rd, IS_SOLID)){
            (*y) += 1, (*x) += 1; 
            return;
        }
    }

    if(p->xvel > 1){
        if(l->dens < p->dens){
            if(!CHECK_FLAG(l, IS_SOLID)){
                (*x) -= 1; 
                return;
            }
        }
        if(r->dens < p->dens){
            if(!CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                if(p->xvel == 1)p->xvel -= 2; 
                p->xvel -= 1; 
                return;
            }
        }
    } else {
        if(r->dens < p->dens){
            if(!CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                return;
            }
        }
        if(l->dens < p->dens){ 
            if(!CHECK_FLAG(l, IS_SOLID)){
                (*x) -= 1; 
                if(p->xvel == 1)p->xvel += 2; 
                p->xvel += 1; 
                return;
            }
        }
    }
}

void OilBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    if(p->heat > OIL_TO_FIRE_TEMP){
        Color c;
        ChangeColor(&c, FIRE_COLORS);
        CreateReplaceParticle(sim, *x, *y, &c, FIRE, FIRE_DENSITY, F_FIRE);
    }

    Particle* d = &sim->pMap[*y + 1][*x];
    if(d->dens < p->dens){
        if(!CHECK_FLAG(d, IS_DUST) && !CHECK_FLAG(d, IS_SOLID)){
            (*y) += 1;
            return;
        }
    }

    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    if(ld->dens < p->dens){
        if(!CHECK_FLAG(ld, IS_SOLID)){
            (*y) += 1, (*x) -= 1; 
            return;
        }
    }
    Particle* rd = &sim->pMap[*y + 1][*x + 1];
    if(rd->dens < p->dens){
        if(!CHECK_FLAG(rd, IS_SOLID)){
            (*y) += 1, (*x) += 1; 
            return;
        }
    }

    if(p->xvel > 1){
        Particle* l = &sim->pMap[*y][*x - 1];
        if(l->dens < p->dens){
        if(!CHECK_FLAG(l, IS_SOLID)){
            (*x) -= 1; 
            return;
        }
    }
        Particle* r = &sim->pMap[*y][*x + 1];
        if(r->dens < p->dens){
            if(!CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                if(p->xvel == 1)p->xvel -= 2; 
                p->xvel -= 1; 
                return;
            }
        }
    } else {
        Particle* r = &sim->pMap[*y][*x + 1];
        if(r->dens < p->dens){
            if(!CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                return;
            }
        }
        Particle* l = &sim->pMap[*y][*x - 1];
        if(l->dens < p->dens){ 
            if(!CHECK_FLAG(l, IS_SOLID)){
                (*x) -= 1; 
                if(p->xvel == 1)p->xvel += 2; 
                p->xvel += 1; 
                return;
            }
        }
    }
}

void AcidBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];

    char del = 0;

    Particle* d = &sim->pMap[*y + 1][*x];
    if(d->dens < p->dens || (del = CHECK_FLAG(d, ACID_HAS_AN_EFFECT))){ 
        if(del){
            if(p->effect_t < ACID_EFFECT_TIME) p->effect_t++;
            else{
                DeleteParticle(sim, *x, *y);
                DeleteParticle(sim, *x, *y + 1);
            }
            return;
        }
        if(!CHECK_FLAG(d, IS_DUST) && !CHECK_FLAG(d, IS_SOLID)){
            (*y) += 1;
            return;
        }
    }
    Particle* u = &sim->pMap[*y - 1][*x];
    if(CHECK_FLAG(u, ACID_HAS_AN_EFFECT)){ 
        if(p->effect_t < ACID_EFFECT_TIME) p->effect_t++;
        else{
            DeleteParticle(sim, *x, *y);
            DeleteParticle(sim, *x, *y - 1);
        }
        return;
    }

    Particle* ld = &sim->pMap[*y + 1][*x - 1];
    if(ld->dens < p->dens){
        if(!CHECK_FLAG(ld, IS_DUST) && !CHECK_FLAG(ld, IS_SOLID)){
            (*y) += 1, (*x) -= 1; 
            return;
        }
    }
    Particle* rd = &sim->pMap[*y + 1][*x + 1];
    if(rd->dens < p->dens){
        if(!CHECK_FLAG(rd, IS_DUST) && !CHECK_FLAG(rd, IS_SOLID)){
            (*y) += 1, (*x) += 1; 
            return;
        }
    }

    if(p->xvel > 1){
        Particle* l = &sim->pMap[*y][*x - 1];
        if(l->dens < p->dens || (del = CHECK_FLAG(l, ACID_HAS_AN_EFFECT))){
            if(del){
                if(p->effect_t < ACID_EFFECT_TIME) p->effect_t++;
                else{
                    DeleteParticle(sim, *x, *y);
                    DeleteParticle(sim, *x - 1, *y);
                }
                return;
            }
            if(!CHECK_FLAG(l, IS_DUST) && !CHECK_FLAG(l, IS_SOLID)){
                (*x) -= 1; 
                return;
            }
        }
        Particle* r = &sim->pMap[*y][*x + 1];
        if(r->dens < p->dens|| (del = CHECK_FLAG(r, ACID_HAS_AN_EFFECT))){ 
            if(del){
                if(p->effect_t < ACID_EFFECT_TIME) p->effect_t++;
                else{
                    DeleteParticle(sim, *x, *y);
                    DeleteParticle(sim, *x + 1, *y);
                }
                return;
            }
            if(!CHECK_FLAG(r, IS_DUST) && !CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                if(p->xvel == 1)p->xvel -= 2; 
                p->xvel -= 1; 
                return;
            }
        }
    } else {
        Particle* r = &sim->pMap[*y][*x + 1];
        if(r->dens < p->dens || (del = CHECK_FLAG(r, ACID_HAS_AN_EFFECT))){
            if(del){
                if(p->effect_t < ACID_EFFECT_TIME)p->effect_t++;
                else{
                    DeleteParticle(sim, *x, *y);
                    DeleteParticle(sim, *x + 1, *y);
                }
                return;
            }
            if(!CHECK_FLAG(r, IS_DUST) && !CHECK_FLAG(r, IS_SOLID)){
                (*x) += 1; 
                return;
            }
        }
        Particle* l = &sim->pMap[*y][*x - 1];
        if(l->dens < p->dens|| (del = CHECK_FLAG(l, ACID_HAS_AN_EFFECT))){ 
            if(del){
                if(p->effect_t < ACID_EFFECT_TIME)p->effect_t++;
                else{
                    DeleteParticle(sim, *x, *y);
                    DeleteParticle(sim, *x - 1, *y);
                }
                return;
            }
            if(!CHECK_FLAG(l, IS_DUST) && !CHECK_FLAG(l, IS_SOLID)){
                (*x) -= 1; 
                if(p->xvel == 1)p->xvel -= 2; 
                p->xvel -= 1; 
                return;
            }
        }
    }
}

void SteamBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    p->life_t++;
    if(p->life_t > STEAM_LIFE_TIME){DeleteParticle(sim, *x, *y); return;}

    Particle* u = &sim->pMap[*y - 1][*x];
    if(u->dens > p->dens){ (*y) -= 1; return;}

    Particle* ru = &sim->pMap[*y - 1][*x + 1];  
    if(ru->dens > p->dens){ (*y) -= 1, (*x) += 1; return;}

    Particle* lu = &sim->pMap[*y - 1][*x - 1];
    if(lu->dens > p->dens){ (*y) -= 1, (*x) -= 1; return;}

    Particle* r = &sim->pMap[*y][*x + 1];
    if(r->dens > p->dens){ (*x) += 1; return;}

    Particle* l = &sim->pMap[*y][*x - 1];
    if(l->dens > p->dens){ (*x) -= 1; return;}
}

void FungusBehave(Simulator* sim, int* x, int* y)
{
    Particle* p = &sim->pMap[*y][*x];
    Particle* d = &sim->pMap[*y + 1][*x];

    if(d->dens < p->dens){ (*y) += 1; }
}

void FireBehave(Simulator* sim, int* x, int* y){

    Particle* p = &sim->pMap[*y][*x];
    // printf("life:%d effect:%d\n",p->life_t, p->effect_t);
    // sim->pMap[*y][*x]
    
    p->life_t++;
    if(p->effect_t < FIRE_EFFECT_TIME){
        p->effect_t++;
        return;
    }

    if(p->heat <= 0){
        Color c;
        ChangeColor(&c, BLACK);
        CreateReplaceParticle(sim, *x, *y, &c, SMOKE, SMOKE_DENSITY, F_SMOKE);
        return;
    }

    if(p->life_t > FIRE_LIFE_TIME){
        Color c;
        ChangeColor(&c, BLACK);
        CreateReplaceParticle(sim, *x, *y, &c, SMOKE, SMOKE_DENSITY, F_SMOKE);
        return;
    }

    int fire;
    Particle* d = &sim->pMap[*y + 1][*x];
    if(!CHECK_FLAG(d, HEAT_RELEASER)){
        d->heat += FIRE_HEAT_RELEASE_TEMP;
    }
    Particle* u = &sim->pMap[*y - 1][*x];
    if(!CHECK_FLAG(u, HEAT_RELEASER)){
        u->heat += FIRE_HEAT_RELEASE_TEMP;
    }
    Particle* l = &sim->pMap[*y][*x - 1];
    if(!CHECK_FLAG(l, HEAT_RELEASER)){
        l->heat += FIRE_HEAT_RELEASE_TEMP;
    }
    Particle* r = &sim->pMap[*y][*x + 1];
    if(!CHECK_FLAG(r, HEAT_RELEASER)){
        r->heat += FIRE_HEAT_RELEASE_TEMP;
    }

    // Particle* u = &sim->pMap[*y - 1][*x];
    // if(fire = (CHECK_FLAG(u, FIRE_HAS_AN_EFFECT))){
    //     u->heat += FIRE_HEAT_RELEASE_TEMP;
    // }

    // Particle* l = &sim->pMap[*y][*x - 1];
    // if(fire = (CHECK_FLAG(l, FIRE_HAS_AN_EFFECT))){
    //     Color c;
    //     ChangeColor(&c, FIRE_COLORS);
    //     CreateReplaceParticle(sim, *x - 1, *y, &c, FIRE, FIRE_DENSITY, F_FIRE);
    //     return;
    // }

    // Particle* r = &sim->pMap[*y][*x + 1];
    // if(fire = (CHECK_FLAG(r, FIRE_HAS_AN_EFFECT))){
    //     Color c;
    //     ChangeColor(&c, FIRE_COLORS);
    //     CreateReplaceParticle(sim, *x + 1, *y, &c, FIRE, FIRE_DENSITY, F_FIRE);
    //     return;
    // }
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
                    WaterBehave(sim, &x, &y);
                    break;
                case ACID:
                    AcidBehave(sim, &x, &y);
                    break;
                case STEAM:
                    SteamBehave(sim, &x, &y);
                    break;
                case FIRE:
                    FireBehave(sim, &x, &y);
                    break;
                case SMOKE:
                    SteamBehave(sim, &x, &y);
                    break;
                case COAL:
                    CoalBehave(sim, &x, &y);
                    break;
                case OIL:
                    OilBehave(sim, &x, &y);
                    break;
                case LAVA:
                    OilBehave(sim, &x, &y);
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


clock_t CalcDelta(clock_t old){
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

void DrawCage(Window* win, Simulator* sim,int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(win->renderer, r, g, b, a);

    for(int i = 0; i < win->w; i += sim->pSide){
        SDL_RenderDrawLine(win->renderer, i, 0, i, win->h);
    }
    
    for(int i = 0; i < win->h; i += sim->pSide){
        SDL_RenderDrawLine(win->renderer, 0, i, win->w, i);
    }
}

void DrawCircle(Simulator* sim, int px, int py, int rad){
    int x = rad - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (rad << 1);

    SDL_SetRenderDrawColor(win->renderer, MWHITE);
    while(x >= y){
        int cx = x * CSIDE;
        int cy = y * CSIDE;
        SDL_Rect r0 = {px + cx, py + cy, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r0);
        SDL_Rect r1 = {px + cy, py + cx, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r1);
        SDL_Rect r2 = {px - cy, py + cx, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r2);
        SDL_Rect r3 = {px - cx, py + cy, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r3);
        SDL_Rect r4 = {px - cx, py - cy, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r4);
        SDL_Rect r5 = {px - cy, py - cx, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r5);
        SDL_Rect r6 = {px + cy, py - cx, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r6);
        SDL_Rect r7 = {px + cx, py - cy, CSIDE, CSIDE};
        SDL_RenderFillRect(win->renderer, &r7);

        if(err <= 0){
            y++;
            err += dy;
            dy += 2;  
        }
        if(err > 0){
            x--;
            dx += 2;
            err += dx - (rad << 1);
        }
    }
}

void DrawGui(Window* win, SDL_Rect* buttons, int buttonCount){
    // printf("x:%d y:%d\n", buttons[0].x, buttons[0].y);
    SDL_SetRenderDrawColor(win->renderer, M_SAND_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[0]);
    SDL_SetRenderDrawColor(win->renderer, M_WATER_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[1]);
    SDL_SetRenderDrawColor(win->renderer, M_FUNGUS_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[2]);
    SDL_SetRenderDrawColor(win->renderer, M_ACID_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[3]);
    SDL_SetRenderDrawColor(win->renderer, M_WALL_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[4]);
    SDL_SetRenderDrawColor(win->renderer, M_STEAM_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[5]);
    SDL_SetRenderDrawColor(win->renderer, M_FIRE_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[6]);
    SDL_SetRenderDrawColor(win->renderer, M_SMOKE_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[7]);
    SDL_SetRenderDrawColor(win->renderer, M_COAL_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[8]);
    SDL_SetRenderDrawColor(win->renderer, M_OIL_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[9]);
    SDL_SetRenderDrawColor(win->renderer, M_LAVA_BUTTON_COLOR);
    SDL_RenderFillRect(win->renderer, &buttons[10]);

    SDL_SetRenderDrawColor(win->renderer, MWHITE);
    SDL_RenderDrawRect(win->renderer, &buttons[currentPart]);
}


void ProcessInput(SDL_Event event, Window* win, Simulator* sim,Color* color)
{
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);

    // Particle generation
    if((state & SDL_BUTTON(SDL_BUTTON_LEFT))){
        // printf("works\n");
        int created = 0;
        int px = ((mx) / sim->pSide);
        int py = ((my) / sim->pSide);
        if(mode == 0)
            CreateParticle(sim, px, py, color, genType, WALL_DENSITY, F_WALL);
        if(mode == 1){

            switch (genType)
            {
            case SAND:
                CreateManyParticles(sim, px, py, RADIUS, SAND_COLORS, genType, SAND_DENSITY, F_SAND);
                break;
            case FUNGUS:
                CreateManyParticles(sim, px, py, RADIUS, PINK, genType, FUNGUS_DENSITY, F_FUNGUS);
                break;
            case WATER:
                CreateManyParticles(sim, px, py, RADIUS, WATER_COLORS, genType, WATER_DENSITY, F_WATER);
                break;
            case ACID:
                CreateManyParticles(sim, px, py, RADIUS, ACID_COLORS, genType, ACID_DENSITY, F_ACID);
                break;
            case WALL:
                CreateManyParticles(sim, px, py, RADIUS, WALL_COLORS, genType, WALL_DENSITY, F_WALL);
                break;
            case STEAM:
                CreateManyParticles(sim, px, py, RADIUS, LIGHT_BLUE, genType, STEAM_DENSITY, F_STEAM);
                break;
            case FIRE:
                CreateManyParticles(sim, px, py, RADIUS, FIRE_COLORS, genType, FIRE_DENSITY, F_FIRE);
                break;
            case SMOKE:
                CreateManyParticles(sim, px, py, RADIUS, BLACK, genType, SMOKE_DENSITY, F_SMOKE);
                break;
            case COAL:
                CreateManyParticles(sim, px, py, RADIUS, COAL_COLORS, genType, COAL_DENSITY, F_COAL);
                break;
            case OIL:
                CreateManyParticles(sim, px, py, RADIUS, OIL_BROWN, genType, OIL_DENSITY, F_OIL);
                break;
            case LAVA:
                CreateManyParticles(sim, px, py, RADIUS, ORANGE, genType, OIL_DENSITY, F_OIL);
                break;
            }
        }
    }

    if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        int px = ((mx) / sim->pSide);
        int py = ((my) / sim->pSide);
        DeleteManyParticles(sim, px, py, RADIUS);
    }

    // Other events
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) win->isrunning = 0;

        if(event.type == SDL_KEYDOWN){
            // printf("works\n");
            if(QUIT_BUTTON) win->isrunning = 0;
            if(SCREEN_CLEAR_BUTTON){
                ClearMap(sim);
                WallBox(sim);
            }
            if(PARTICLE_CHANGE_BUTTON){
                ElementChange();
            }
        }
    }
}

void ElementChange(){
    currentPart = (currentPart + 1) % partSeqSize;
    genType = PartSeq[currentPart];
}


void AddType(char* name, Colors color, Color buttonColor, int dens, uint32_t flags, void (*func)(Simulator* sim, int* x, int* y))
{
    typeNameList[countTypes] = name;
    typeFuncList[countTypes] = func;
    typeButtonColorList[countTypes] = buttonColor;
    typeColorList[countTypes] = color;
    typeDensityList[countTypes] = dens;
    typeFlagsList[countTypes] = flags;

    countTypes++;
}

int CreateParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens, PartFlags flags){
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
        part.xvel = 2;
        part.yvel = 1;
        part.dens = dens;
        part.life_t = rand() % 30;
        part.effect_t = rand() % 30;
        part.flags = flags;
        part.heat = 0;
        

        sim->pMap[py][px] = part;
        sim->partCount++;
        return 1;
    }
    return 0;
}

void CreateReplaceParticle(Simulator* sim, int px, int py, Color* color, PartType type, int dens, PartFlags flags){
    if(sim->pMap[py][px].id < 0){
        sim->partCount++;
    }

    Particle part;
    part.p.x = px;
    part.p.y = py;
    part.id = 1;
    CopyColor(&part.c, color);
    part.t = type;
    part.xvel = 2;
    part.yvel = 1;
    part.dens = dens;
    part.life_t = rand() % 30;
    part.effect_t = rand() % 30;
    part.flags = flags;
    part.heat = (type == FIRE ? 400 : 0);
    // printf("%d\n", (flags & IS_SOLID) ? 1 : 0);

    sim->pMap[py][px] = part;
}

void CreateManyParticles(Simulator* sim, int px, int py, int rad, Colors c, PartType type, int dens, PartFlags flags){
    int begy = py - rad;
    int begx = px - rad;
    int endy = py + rad;
    int endx = px + rad;
    int srad = rad * rad;

    Color color;
    for(int i = begy; i < endy; i++){
        int di = abs(py - i);
        for(int j = begx; j < endx; j++){
            int dj = abs(px - j);
            if(di*di + dj*dj < srad)
                if(j >= 0 && i >= 0 && j < sim->cols && i < sim->rows)
                    if(sim->pMap[i][j].id < 0){
                        ChangeColor(&color, c);
                        CreateReplaceParticle(sim, j, i, &color, type, dens, flags);
                    }
        }
    }
}

void DeleteParticle(Simulator* sim, int px, int py){
    if(sim->pMap[py][px].id > 0){
        sim->pMap[py][px].id = -1;
        sim->pMap[py][px].t = AIR;
        sim->pMap[py][px].dens = AIR_DENSITY;
        sim->pMap[py][px].flags = F_AIR;
        sim->pMap[py][px].heat = 0;

        sim->partCount--;
    }
}

void DeleteManyParticles(Simulator* sim, int px, int py, int rad){
    int begy = py - rad;
    int begx = px - rad;
    int endy = py + rad;
    int endx = px + rad;

    for(int i = begy; i < endy; i++){
        if(i <= 0 || i >= sim->rows - 1) continue;
        int di = abs(py - i);
        for(int j = begx; j < endx; j++){
            if(j <= 0 || j >= sim->cols - 1) continue;
            int dj = abs(px - j);
            if(di*di + dj*dj < rad*rad)
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

void WallBox(Simulator* sim){
    Color c;
    ChangeColor(&c, GRAY);
    
    for(int j = 0; j < sim->cols; j++){
        CreateParticle(sim, j, 0, &c, WALL, WALL_DENSITY, F_WALL);
    }
    for(int j = 0; j < sim->cols; j++){
        CreateParticle(sim, j, sim->rows - 1, &c, WALL, WALL_DENSITY, F_WALL);
    }
    for(int i = 1; i < sim->rows - 1; i++){
        CreateParticle(sim, 0, i, &c, WALL, WALL_DENSITY, F_WALL);
        CreateParticle(sim, sim->cols - 1, i, &c, WALL, WALL_DENSITY, F_WALL);
    }

}

void CheckGuiButtons(Window* win, int mx, int my){
    if(mx > win->w - ELEMENT_BUTTON_MARGIN_W && mx <= win->w - ELEMENT_BUTTON_MARGIN_W + ELEMENT_BUTTON_SIZE){

        int bd = ELEMENT_BUTTON_SIZE + ELEMENT_BUTTONS_MARGIN;
        int bt = ELEMENT_BUTTON_MARGIN_H + ELEMENT_BUTTON_SIZE;

        if(my > ELEMENT_BUTTON_MARGIN_H && my <= bt){
            genType = SAND;
            currentPart = 0;
            return;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + bd && my <= (bt + bd)){
            genType = WATER;
            currentPart = 1;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 2*bd && my <= (bt + 2*bd)){
            genType = FUNGUS;
            currentPart = 2;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 3*bd && my <= (bt + 3*bd)){
            genType = ACID;
            currentPart = 3;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 4*bd && my <= (bt + 4*bd)){
            genType = WALL;
            currentPart = 4;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 5*bd && my <= (bt + 5*bd)){
            genType = STEAM;
            currentPart = 5;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 6*bd && my <= (bt + 6*bd)){
            genType = FIRE;
            currentPart = 6;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 7*bd && my <= (bt + 7*bd)){
            genType = SMOKE;
            currentPart = 7;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 8*bd && my <= (bt + 8*bd)){
            genType = COAL;
            currentPart = 8;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 9*bd && my <= (bt + 9*bd)){
            genType = OIL;
            currentPart = 9;
        }
        if(my > ELEMENT_BUTTON_MARGIN_H + 10*bd && my <= (bt + 10*bd)){
            genType = LAVA;
            currentPart = 10;
        }
    }
}

void CreateGui(Window* win, SDL_Rect* buttons, int* buttonCount, int elementCount){
    SDL_Rect button = {
        win->w - ELEMENT_BUTTON_MARGIN_W,
        ELEMENT_BUTTON_MARGIN_H,
        ELEMENT_BUTTON_SIZE,
        ELEMENT_BUTTON_SIZE
    };
    
    for(int i = 0; i < elementCount; i++){
        button.y = ELEMENT_BUTTON_MARGIN_H + i * (ELEMENT_BUTTON_SIZE + ELEMENT_BUTTONS_MARGIN);
        buttons[i] = button; 
    }
    *buttonCount = elementCount;
}