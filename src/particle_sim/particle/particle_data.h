#ifndef PARTICLE_DATA_H
#define PARTICLE_DATA_H

#include "engine_lib.h"


#define STATE_DT        byte
#define TYPE_DT         int32_t
#define XVEL_DT         uint8_t
#define YVEL_DT         uint8_t
#define COLOR_DT        Color
#define EFFECTT_DT      uint16_t
#define LIFET_DT        uint16_t
#define HEAT_DT         int32_t
#define PFLAGS_DT       uint8_t
#define DURABILITY_DT   uint32_t
#define CUSTOM_DT       void*

typedef byte            part_state_t;
typedef int32_t         part_type_t;
typedef uint8_t         part_xvel_t;
typedef uint8_t         part_yvel_t;
typedef Color           part_color_t;
typedef uint16_t        part_effectt_t;
typedef uint16_t        part_lifet_t;
typedef int32_t         part_heat_t;
typedef uint8_t         part_pflags_t;
typedef uint32_t        part_durab_t;
typedef void*           part_custom_t;




typedef enum{
    EMPTY = -1,
    SAND = 0,
    WATER,
    STEAM,
    ACID,
    WOOD,
    WALL,
    FIRE,
    FIRE_SMOKE,
    SMOKE,
    COAL,
    POWDER,
    OIL,
    LAVA,
    FUNGUS,
    OBSIDIAN,
    PHANTOM,
    SOURCE,
    AIR
} PartType;

// Flags
typedef enum{
    IS_DUST = 0x80000000,
    IS_LIQUID = 0x40000000,
    IS_GAS = 0x20000000,
    IS_SOLID = 0x10000000,

    ACID_HAS_AN_EFFECT = 0x08000000,
    FIRE_HAS_AN_EFFECT = 0x04000000,
    HEAT_STEALER = 0x02000000,
    HEAT_RELEASER = 0x01000000,

    HEAT_KEEPER = 0x00800000,
    EXPLOSIVE = 0x00400000
} Flags;

typedef enum{
    BURNING = 0x80
} iFlags;

typedef enum{
    SAND_FLAGS = 0x88000000,
    WATER_FLAGS = 0x42000000,
    STEAM_FLAGS = 0x20800000,
    ACID_FLAGS = 0x40000000,
    WOOD_FLAGS = 0x1C000000,
    WALL_FLAGS = 0x10000000,
    FIRE_FLAGS = 0x1000000,
    FIRE_SMOKE_FLAGS = 0x2000000,
    SMOKE_FLAGS = 0x20000000,
    COAL_FLAGS = 0x8C000000,
    POWDER_FLAGS = 0x8C000000,
    OIL_FLAGS = 0x44000000,
    LAVA_FLAGS = 0x41000000,
    FUNGUS_FLAGS = 0x8C000000,
    OBSIDIAN_FLAGS = 0x10000000,
    PAHNTOM_FLAGS = 0x00400000,
    SOURCE_FLAGS = 0x10000000,
    AIR_FLAGS = 0x20000000
} PartFlags;

typedef enum{
    SAND_HT = 100,
    WATER_HT = 300,
    STEAM_HT = 50,
    ACID_HT = 200,
    WOOD_HT = 100,
    WALL_HT = 100,
    FIRE_HT = 20,
    FIRE_SMOKE_HT = 50,
    SMOKE_HT = 50,
    COAL_HT = 100,
    POWDER_HT = 100,
    OIL_HT = 200,
    LAVA_HT = 20,
    FUNGUS_HT = 150,
    OBSIDIAN_HT = 40,
    AIR_HT = 10
} PartHeatTransfer;



/* 
Flags
from left to right
0 - dust, 1 - liquid, 2 - gas, 3 - solid
4 - acid has an effect, 5 - flamable

Flags are used to give some common 
information about class particles
*/

/*
iFlags
from left to right
0 - burning

iFlags are used to give some information
about individual particle
*/

typedef enum{
    P_IGNORED = -1,
    P_USED = 0,
    P_FRESH = 1
} PartStates;

#define MAX_COLOR_COUNT 32
typedef struct{
    char* name;
    Color buttonColor;
    int colorCount;
    Color color[MAX_COLOR_COUNT];
    uint32_t flags;
    uint8_t pflags;
    int32_t heatTransfer;
    int density;
    uint32_t durability;
    bool (*func)(int x, int y);
} PartData;

typedef struct{
    Pos p;
    int8_t id;
    Color c;
    int type;
    uint8_t xvel, yvel;
    uint16_t effect_t;
    uint16_t life_t;
    int32_t heat;
    uint8_t iflags;
    uint32_t durability;
} Particle;

typedef struct{
    int rows;
    int cols;
    int partCount;
    Particle** map;
} ParticleMap;

typedef struct{
    int width;
    int height;
    part_type_t* types;
    part_color_t* colors;
} ParticleImage;

typedef struct{
    const char* name;
    bool (*func)(int x, int y);
} PartFunc;


typedef uint16_t ParticleType;


#define CHECK_FLAG(p, f) ((p) & (f) ? 1 : 0)
#define SET_FLAG(p, f) ((p) |= (f))
#define CLEAR_FLAG(p, f) ((p) &= (f))
#define TOGGLE_FLAG(p, f) ((p) ^= (f))

// Particle type params
// They store common data of particle types

#define MAX_NUMBER_OF_TYPES 256
#define MAX_NUMBER_OF_FUNCS MAX_NUMBER_OF_TYPES

extern bool (*typeFuncList[MAX_NUMBER_OF_TYPES])(int x, int y);
extern Color typeButtonColorList[MAX_NUMBER_OF_TYPES];
extern Color typeColorList[MAX_NUMBER_OF_TYPES][MAX_COLOR_COUNT];
extern int typeDensityList[MAX_NUMBER_OF_TYPES];
extern uint32_t typeFlagsList[MAX_NUMBER_OF_TYPES];
extern uint8_t typeIFlagsList[MAX_NUMBER_OF_TYPES];
extern char* typeNameList[MAX_NUMBER_OF_TYPES];
extern int32_t typeHeatTransferList[MAX_NUMBER_OF_TYPES];
extern uint32_t typeDurabilityList[MAX_NUMBER_OF_TYPES];

extern int countParticleFuncs;
bool (*GetFunc(const char* funcName))(int x, int y);
extern PartFunc partFuncList[MAX_NUMBER_OF_TYPES];


extern int LIQUID_OPACITY;


int GetParticleTypeCount();
int GetParticleFuncCount();
void SetParticleTypeCount(int count);
void SetParticleFuncCount(int count);

// Particle map
int CreateParticleMap(ParticleMap* pmap, int rows, int cols);
int DeleteParticleMap(ParticleMap* pmap);

// Color
void ChangeColor(Color* c, Colors cs);


#endif