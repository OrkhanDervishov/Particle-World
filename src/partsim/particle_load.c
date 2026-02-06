#include "particle_load.h"


char *jsonFormat =  " { \"name\" : \" %127[^\"]\" , "
                    "\"button_color\" : \" %16[^\"]\" , "
                    "\"color_count\" : \" %d , "
                    "\"color\" : \" %16[^\"]\" , "
                    "\"flags\" : \" %16[^\"]\" , "
                    "\"p_flags\" : \" %16[^\"]\" , "
                    "\"heat_transfer\" : \" %d , "
                    "\"density\" : %d , "
                    "\"durability\" : %d } ";

char* jsonFormatPart1 = " { \"name\" : \" %127[^\"]\" , "
                        "\"button_color\" : \" %16[^\"]\" , "
                        "\"color_count\" : \" %d , ";

char* jsonFormatPart2 = "\"flags\" : \" %16[^\"]\" , "
                        "\"p_flags\" : \" %16[^\"]\" , "
                        "\"heat_transfer\" : \" %d , "
                        "\"density\" : %d , "
                        "\"durability\" : %d } ";

char *xmlFormat =   " <particle> "
                    " <name> %127[^<] </name> "
                    " <button_color> %16[^<] </button_color> "
                    " <color> %16[^<] </color> "
                    " <flags> %16[^<] </flags> "
                    " <p_flags> %16[^<] </p_flags> "
                    " <heat_transfer> %d </heat_transfer> "
                    " <density> %d </density> "
                    " <durability> %d </durability>"
                    " </particle> ";


PartData ReadParticleData(int typeIndex, const char* filename){
    int len = strlen(filename);
    char *format;
    if(!strcmp(&filename[len-4], "json")) format = jsonFormat;
    else if(!strcmp(&filename[len-3], "xml")) format = xmlFormat;
    

    PartData pd;
    FILE *file = fopen(filename, "r");
    
    char flagsStr[16];
    char pflagsStr[16];
    char buttonColorStr[16];
    char colorStr[32][16];
    char funcNameStr[256];

    fscanf(file, jsonFormatPart1,
        pd.name, buttonColorStr, pd.colorCount
    );

    if(pd.colorCount >= MAX_COLOR_COUNT) pd.colorCount = MAX_COLOR_COUNT-1;
    if(pd.colorCount <= 0) pd.colorCount = 1;

    fscanf(file, " \"color\" : [ ");
    for(int i = 0; i < pd.colorCount; i++){
        fscanf(file, " %s , ", colorStr[i]);
        pd.color[i].rgba = strtol(colorStr[i], NULL, 16);
    }
    fscanf(file, " ] , ");

    fscanf(file, jsonFormatPart2,
        flagsStr, pflagsStr, pd.heatTransfer, pd.density, pd.durability, funcNameStr
    );

    pd.flags = strtol(flagsStr, NULL, 16);
    pd.pflags = strtol(pflagsStr, NULL, 16);
    pd.buttonColor.rgba = strtol(buttonColorStr, NULL, 16);

    pd.func = GetFunc(funcNameStr);

    fclose(file);
    return pd;
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
    AddType("sand", SAND, c, c, 700, SAND_FLAGS, 0x00, SandBehave, SAND_HT, 100);
    ChangeColor(&c, BLUE);
    AddType("water", WATER, c, c, 200, WATER_FLAGS, 0x00, WaterBehave, WATER_HT, 100);
    ChangeColor(&c, LIGHT_BLUE);
    AddType("steam", STEAM, c, c, 40, STEAM_FLAGS, 0x00, SteamBehave, STEAM_HT, 100);
    ChangeColor(&c, GREEN);
    AddType("acid", ACID, c, c, 190, ACID_FLAGS, 0x00, AcidBehave, ACID_HT, 100);
    ChangeColor(&c, GRAY);
    AddType("wall", WALL, c, c, 1000000, WALL_FLAGS, 0x00, NULL, WALL_HT, 1000000);
    ChangeColor(&c, BROWN);
    AddType("wood", WOOD, c, c, 1000000, WOOD_FLAGS, 0x00, WoodBehave, WOOD_HT, 100);
    ChangeColor(&c, RED);
    AddType("fire", FIRE, c, c, 80, FIRE_FLAGS, 0x80, FireBehave, FIRE_HT, 100);
    ChangeColor(&c, RED);
    AddType("fire smoke", FIRE_SMOKE, c, c, 20, FIRE_SMOKE_FLAGS, 0x80, FireSmokeBehave, FIRE_SMOKE_HT, 100);
    ChangeColor(&c, BLACK);
    AddType("smoke", SMOKE, c, c, 45, SMOKE_FLAGS, 0x00, SmokeBehave, SMOKE_HT, 100);
    ChangeColor(&c, BLACK);
    AddType("coal", COAL, c, c, 600, COAL_FLAGS, 0x00, CoalBehave, COAL_HT, 100);
    ChangeColor(&c, GRAY);  
    AddType("powder", POWDER, c, c, 600, POWDER_FLAGS, 0x00, PowderBehave, POWDER_HT, 100);
    ChangeColor(&c, OIL_BROWN);
    AddType("oil", OIL, c, c, 180, OIL_FLAGS, 0x00, OilBehave, OIL_HT, 100);
    ChangeColor(&c, ORANGE);
    AddType("lava", LAVA, c, c, 400, LAVA_FLAGS, 0x80, LavaBehave, LAVA_HT, 100);
    ChangeColor(&c, PINK);
    AddType("fungus", FUNGUS, c, c, 250, FUNGUS_FLAGS, 0x00, FungusBehave, FUNGUS_HT, 100);
    ChangeColor(&c, OBSIDIAN_GRAY);
    AddType("obsidian", OBSIDIAN, c, c, 1000, OBSIDIAN_FLAGS, 0x00, ObsidianBehave, OBSIDIAN_HT, 100);
    ChangeColor(&c, WHITE);
    AddType("phantom", PHANTOM, c, c, 0, PAHNTOM_FLAGS, 0x00, PhantomBehave, OBSIDIAN_HT, 1000000);
    ChangeColor(&c, YELLOW);
    AddType("source", SOURCE, c, c, 1000000, SOURCE_FLAGS, 0x00, SourceBehave, OBSIDIAN_HT, 1000000);
    // ChangeColor(&c, WHITE);
    AddType("air", AIR, c, c, 50, AIR_FLAGS, 0x00, NULL, AIR_HT, 100);
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
    uint32_t durability
)
{
    typeNameList[type] = name;
    typeFuncList[type] = func;
    typeButtonColorList[type] = buttonColor;
    // printf("works\n");
    typeColorList[type][0] = color;
    // printf("works1\n");
    typeDensityList[type] = dens;
    typeFlagsList[type] = flags;
    typeIFlagsList[type] = iflags;
    typeDurabilityList[type] = durability;

    countParticleTypes++;
}