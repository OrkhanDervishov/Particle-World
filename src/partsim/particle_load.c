#include "particle_load.h"



void InitParticles(){
    Color c;
    ChangeColor(&c, YELLOW);
    AddType("sand", SAND, SAND_COLORS, c, 700, SAND_FLAGS, 0x00, SandBehave, SAND_HT, 100);
    ChangeColor(&c, BLUE);
    AddType("water", WATER, WATER_COLORS, c, 200, WATER_FLAGS, 0x00, WaterBehave, WATER_HT, 100);
    ChangeColor(&c, LIGHT_BLUE);
    AddType("steam", STEAM, LIGHT_BLUE, c, 40, STEAM_FLAGS, 0x00, SteamBehave, STEAM_HT, 100);
    ChangeColor(&c, GREEN);
    AddType("acid", ACID, ACID_COLORS, c, 190, ACID_FLAGS, 0x00, AcidBehave, ACID_HT, 100);
    ChangeColor(&c, GRAY);
    AddType("wall", WALL, WALL_COLORS, c, 1000000, WALL_FLAGS, 0x00, NULL, WALL_HT, 1000000);
    ChangeColor(&c, BROWN);
    AddType("wood", WOOD, BROWN, c, 1000000, WOOD_FLAGS, 0x00, WoodBehave, WOOD_HT, 100);
    ChangeColor(&c, RED);
    AddType("fire", FIRE, FIRE_COLORS, c, 80, FIRE_FLAGS, 0x80, FireBehave, FIRE_HT, 100);
    ChangeColor(&c, RED);
    AddType("fire smoke", FIRE_SMOKE, FIRE_COLORS, c, 20, FIRE_SMOKE_FLAGS, 0x80, FireSmokeBehave, FIRE_SMOKE_HT, 100);
    ChangeColor(&c, BLACK);
    AddType("smoke", SMOKE, BLACK, c, 45, SMOKE_FLAGS, 0x00, SmokeBehave, SMOKE_HT, 100);
    ChangeColor(&c, BLACK);
    AddType("coal", COAL, COAL_COLORS, c, 600, COAL_FLAGS, 0x00, CoalBehave, COAL_HT, 100);
    ChangeColor(&c, GRAY);  
    AddType("powder", POWDER, GRAY, c, 600, POWDER_FLAGS, 0x00, PowderBehave, POWDER_HT, 100);
    ChangeColor(&c, OIL_BROWN);
    AddType("oil", OIL, OIL_BROWN, c, 180, OIL_FLAGS, 0x00, OilBehave, OIL_HT, 100);
    ChangeColor(&c, ORANGE);
    AddType("lava", LAVA, ORANGE, c, 400, LAVA_FLAGS, 0x80, LavaBehave, LAVA_HT, 100);
    ChangeColor(&c, PINK);
    AddType("fungus", FUNGUS, PINK, c, 250, FUNGUS_FLAGS, 0x00, FungusBehave, FUNGUS_HT, 100);
    ChangeColor(&c, OBSIDIAN_GRAY);
    AddType("obsidian", OBSIDIAN, OBSIDIAN_GRAY, c, 1000, OBSIDIAN_FLAGS, 0x00, ObsidianBehave, OBSIDIAN_HT, 100);
    ChangeColor(&c, WHITE);
    AddType("phantom", PHANTOM, FIRE_COLORS, c, 0, PAHNTOM_FLAGS, 0x00, PhantomBehave, OBSIDIAN_HT, 1000000);
    ChangeColor(&c, YELLOW);
    AddType("source", SOURCE, YELLOW, c, 1000000, SOURCE_FLAGS, 0x00, SourceBehave, OBSIDIAN_HT, 1000000);
    // ChangeColor(&c, WHITE);
    AddType("air", AIR, WHITE, c, 50, AIR_FLAGS, 0x00, NULL, AIR_HT, 100);
}



void AddType(
    char* name, 
    PartType type, 
    Colors color, 
    Color buttonColor, 
    int dens, 
    uint32_t flags, 
    uint8_t iflags, 
    bool (*func)(int x, int y),
    int32_t ht,
    uint32_t durability
)
{
    typeNameList[type] = name;
    typeFuncList[type] = func;
    typeButtonColorList[type] = buttonColor;
    typeColorList[type] = color;
    typeDensityList[type] = dens;
    typeFlagsList[type] = flags;
    typeIFlagsList[type] = iflags;
    typeDurabilityList[type] = durability;

    countParticleTypes++;
}