#include "particle_load.h"
#include "custom_parser.h"

PartData default_data(){
    PartData default_data = (PartData){
        .name = "default particle",
        .buttonColor = 0xFFFFFFFF,
        .colorCount = 1,
        .color[0].rgba = 0xFFFFFFFF,
        .flags = 0x80000000,
        .pflags = 0x00,
        .heatTransfer = 0,
        .density = 100,
        .durability = 1000,
        .func = NULL
    };

    return default_data;
}


PartData ReadParticleData(int typeIndex, const char* filename){
    // const char* json_data = myjson_load_json(filename);
    // if(json_data == NULL){
    //     printf("%s load failed\n", filename);
    //     return default_data();
    // }
    // JsonValue* root = myjson_read_value(json_data);
    
    PartData part_data;
    // part_data.name = myjson_get_value(root, "name");
    // part_data.buttonColor.rgba = (uint32_t)STR_HEX_TO_NUMBER(myjson_get_value(root, "button_color"));
    // part_data.colorCount = (int)myjson_get_value(root, "color_count");
    // // part_data.color = myjson_get_value(root, "name");
    // part_data.flags = (uint32_t)STR_HEX_TO_NUMBER(myjson_get_value(root, "flags"));
    // part_data.pflags = (uint8_t)STR_HEX_TO_NUMBER(myjson_get_value(root, "p_flags"));
    // part_data.heatTransfer = (int32_t)myjson_get_value(root, "heat_transfer");
    // part_data.density = (int)myjson_get_value(root, "density");
    // part_data.durability = (uint32_t)myjson_get_value(root, "durability");

    // myjson_free_value(root);
    // free(json_data);
    return part_data;
}


void PartLoadData(int typeIndex, const char* filename){
    PartData pd = ReadParticleData(typeIndex, filename);

    // AddType(pd.name, typeIndex, );
}


// Load all behaviour functions
bool LoadFunctions(){

}


// Register behaviour functions
bool RegisterFunction(char* funcName, bool (*func)(int x, int y)){
    if(GetFunc(funcName) != NULL){
        printf("Function with the same name already exists: %s\n", funcName);
        return FALSE;
    }
    PartFunc pf = {.name = funcName, .func = func};
    partFuncList[countParticleFuncs++] = pf;

    return TRUE;
}




void InitParticles(){
    Color c;
    ChangeColor(&c, YELLOW);
    AddType("sand", SAND, c, c, 700, SAND_FLAGS, 0x00, SandBehave, SAND_HT, 100, 0, 0);
    ChangeColor(&c, WATER_COLORS);
    AddType("water", WATER, c, c, 200, WATER_FLAGS, 0x00, WaterBehave, WATER_HT, 100, 0, 0);
    ChangeColor(&c, LIGHT_BLUE);
    AddType("steam", STEAM, c, c, 40, STEAM_FLAGS, 0x00, SteamBehave, STEAM_HT, 100, 1000, 0);
    ChangeColor(&c, GREEN);
    AddType("acid", ACID, c, c, 190, ACID_FLAGS, 0x00, AcidBehave, ACID_HT, 100, 0, 300);
    ChangeColor(&c, GRAY);
    AddType("wall", WALL, c, c, 1000000, WALL_FLAGS, 0x00, NULL, WALL_HT, 1000000, 0, 0);
    ChangeColor(&c, BROWN);
    AddType("wood", WOOD, c, c, 1000000, WOOD_FLAGS, 0x00, WoodBehave, WOOD_HT, 100, 1500, 500);
    ChangeColor(&c, RED);
    AddType("fire", FIRE, c, c, 80, FIRE_FLAGS, 0x80, FireBehave, FIRE_HT, 100, 50, 0);
    ChangeColor(&c, RED);
    AddType("fire smoke", FIRE_SMOKE, c, c, 20, FIRE_SMOKE_FLAGS, 0x80, FireSmokeBehave, FIRE_SMOKE_HT, 100, 100, 0);
    ChangeColor(&c, RED);
    AddType("fire liquid", FIRE_LIQUID, c, c, 60, FIRE_LIQUID_FLAGS, 0x80, FireLiquidBehave, FIRE_SMOKE_HT, 2000, 800, 200);
    ChangeColor(&c, BLACK);
    AddType("smoke", SMOKE, c, c, 45, SMOKE_FLAGS, 0x00, SmokeBehave, SMOKE_HT, 100, 1000, 0);
    ChangeColor(&c, BLACK);
    AddType("coal", COAL, c, c, 600, COAL_FLAGS, 0x00, CoalBehave, COAL_HT, 100, 2500, 700);
    ChangeColor(&c, GRAY);  
    AddType("powder", POWDER, c, c, 600, POWDER_FLAGS, 0x00, PowderBehave, POWDER_HT, 100, 0, 0);
    ChangeColor(&c, OIL_BROWN);
    AddType("oil", OIL, c, c, 180, OIL_FLAGS, 0x00, OilBehave, OIL_HT, 100, 2000, 450);
    ChangeColor(&c, ORANGE);
    AddType("lava", LAVA, c, c, 400, LAVA_FLAGS, 0x80, LavaBehave, LAVA_HT, 100, 0, 0);
    ChangeColor(&c, PINK);
    AddType("fungus", FUNGUS, c, c, 250, FUNGUS_FLAGS, 0x00, FungusBehave, FUNGUS_HT, 100, 400, 200);
    ChangeColor(&c, OBSIDIAN_GRAY);
    AddType("obsidian", OBSIDIAN, c, c, 1000, OBSIDIAN_FLAGS, 0x00, ObsidianBehave, OBSIDIAN_HT, 100, 0, 0);
    ChangeColor(&c, WHITE);
    AddType("phantom", PHANTOM, c, c, 0, PAHNTOM_FLAGS, 0x00, PhantomBehave, OBSIDIAN_HT, 1000000, 1500, 0);
    ChangeColor(&c, YELLOW);
    AddType("source", SOURCE, c, c, 1000000, SOURCE_FLAGS, 0x00, SourceBehave, OBSIDIAN_HT, 1000000, 0, 0);
    ChangeColor(&c, WHITE);
    AddType("air", AIR, c, c, 50, AIR_FLAGS, 0x00, NULL, AIR_HT, 100, 0, 0);
}



void AddType(
    char* name, 
    PartType type, 
    Color color, 
    Color buttonColor, 
    int dens, 
    uint32_t flags, 
    uint8_t iflags, 
    bool (*func)(int x, int y),
    int32_t ht,
    uint32_t durability,
    uint32_t life_t,
    uint32_t effect_t
)
{
    typeNameList[type] = name;
    typeFuncList[type] = func;
    typeButtonColorList[type] = buttonColor;
    typeColorList[type][0] = color;
    typeDensityList[type] = dens;
    typeFlagsList[type] = flags;
    typeIFlagsList[type] = iflags;
    typeDurabilityList[type] = durability;
    typeLifetList[type] = life_t;
    typeEffecttList[type] = effect_t;

    SetParticleTypeCount(GetParticleTypeCount()+1);
}