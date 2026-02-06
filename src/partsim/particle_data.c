#include "particle_data.h"


int countParticleTypes = 0;
int countParticleFuncs = 0;

bool (*typeFuncList[MAX_NUMBER_OF_TYPES])(int x, int y);
Color typeButtonColorList[MAX_NUMBER_OF_TYPES];
Color typeColorList[MAX_NUMBER_OF_TYPES][MAX_COLOR_COUNT];
int typeDensityList[MAX_NUMBER_OF_TYPES];
uint32_t typeFlagsList[MAX_NUMBER_OF_TYPES];
uint8_t typeIFlagsList[MAX_NUMBER_OF_TYPES];
char* typeNameList[MAX_NUMBER_OF_TYPES];
int32_t typeHeatTransferList[MAX_NUMBER_OF_TYPES];
uint32_t typeDurabilityList[MAX_NUMBER_OF_TYPES];


int LIQUID_OPACITY = 100;


PartFunc partFuncList[MAX_NUMBER_OF_FUNCS];
bool (*GetFunc(const char* funcName))(int x, int y){
    for(int i = 0; i < MAX_NUMBER_OF_FUNCS; i++){
        
    }
}


int CreateParticleMap(ParticleMap* pmap, int rows, int cols){
    pmap->rows = rows;
    pmap->cols = cols;
    pmap->partCount = 0;
    pmap->map = Malloc2D(rows, cols, sizeof(Particle));
}


int DeleteParticleMap(ParticleMap* pmap){
    Free2D(pmap->map, pmap->rows);
}


void ChangeColor(Color *c, Colors cs){

    int r = rand() % 4;
    switch (cs)
    {
    case WHITE:             c->rgba = 0xFFFFFFFF; break;
    case BLACK:             c->rgba = 0xFF000000; break;
    case RED:               c->rgba = 0xFF0000FF; break;
    case PINK:              c->rgba = 0xFFAA46FC; break;
    case GREEN:             c->rgba = 0xFF00FF00; break;
    case BLUE:              c->rgba = 0xFFFF0000; break;
    case LIGHT_BLUE:        c->rgba = 0xFFFF9696; break;
    case YELLOW:            c->rgba = 0xFF00FFFF; break;
    case LYELLOW:           c->rgba = 0xFF64FFFF; break;
    case ORANGE:            c->rgba = 0xFF0045FF; break;
    case GRAY:              c->rgba = 0xFF878787; break;
    case GOOD_GRAY:         c->rgba = 0xFF3A3A3A; break;
    case OBSIDIAN_GRAY:     c->rgba = 0xFF141414; break;
    case PURPLE:            c->rgba = 0xFFFF00FF; break;
    case BROWN:             c->rgba = 0xFF004B96; break;
    case OIL_BROWN:         c->rgba = 0xFF004283; c->a = LIQUID_OPACITY; break;

    case SAND_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFF00509B; break;
            case 1:         c->rgba = 0xFF004191; break;
            case 2:         c->rgba = 0xFF0046A0; break;
            case 3:         c->rgba = 0xFF005096; break;
        }
        break;
    case WATER_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFFFA0000; break;
            case 1:         c->rgba = 0xFFF00000; break;
            case 2:         c->rgba = 0xFFE60000; break;
            case 3:         c->rgba = 0xFFD80000; break;
        }
        c->a = LIQUID_OPACITY;
        break;
    case WALL_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFF878787; break;
            case 1:         c->rgba = 0xFF919191; break;
            case 2:         c->rgba = 0xFF9B9B9B; break;
            case 3:         c->rgba = 0xFF7D7D7D; break;
        }
        break;
    case ACID_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFF00FA00; break;
            case 1:         c->rgba = 0xFF00F000; break;
            case 2:         c->rgba = 0xFF00E600; break;
            case 3:         c->rgba = 0xFF00DC00; break;
        }
        c->a = LIQUID_OPACITY;
        break;
    case FIRE_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFF0000FF; break;
            case 1:         c->rgba = 0xFF005AFF; break;
            case 2:         c->rgba = 0xFF009AFF; break;
            case 3:         c->rgba = 0xFF00CEFF; break;
        }
        break;
    case COAL_COLORS:
        switch (r) {
            case 0:         c->rgba = 0xFF000000; break;
            case 1:         c->rgba = 0xFF0A0A0A; break;
            case 2:         c->rgba = 0xFF0F0F0F; break;
            case 3:         c->rgba = 0xFF050505; break;
        }
        break;
    }
}