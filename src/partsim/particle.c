#include "particle.h"



// Particle params
int STEAM_LIFE_TIME = 350;
int SMOKE_LIFE_TIME = 300;
int FIRE_LIFE_TIME = 100;
int PHANTOM_LIFE_TIME = 20;

int ACID_EFFECT_TIME = 50;
int FIRE_EFFECT_TIME = 20;
int BURNING_COAL_LIFE_TIME = 800;
int BURNING_OIL_LIFE_TIME = 200;
int BURNING_WOOD_LIFE_TIME = 500;
int BURNING_FUNGUS_LIFE_TIME = 200;

int FIRE_HEAT_RELEASE_TEMP = 50;
int WATER_TO_STEAM_TEMP = 500;
int COAL_TO_FIRE_TEMP = 2000;
int OIL_TO_FIRE_TEMP = 250;
int WOOD_TO_FIRE_TEMP = 1500;
int FUNGUS_TO_FIRE_TEMP = 400;
int WATER_HEAT_ABSORB = 500;

// Implementations

// #define GET_PARTICLE_AT(m, x, y) CHUNK_GET_PARTICLE_REF(m, x, y)
// #define GET_PARTICLE_AT(m, x, y) CS_

ChunkSpace* currentCS = NULL;

void SetChunkSpace(ChunkSpace* cs){
    if(cs == NULL){
        return;
    }
    currentCS = cs;
}

#define SWAP_PARTS(x0, y0, x1, y1)  SwapParticlesCS(currentCS, (x0), (y0), (x1), (y1))
#define DELETE_PART(x, y)           DeleteParticleCS(currentCS, (x), (y))
#define CREATE_PART(x, y, type)     CreateParticleCS(currentCS, (x), (y), (type))
#define REPLACE_PART(x, y, type)    ReplaceParticleCS(currentCS, (x), (y), (type))

#define GET_PART_TYPE(x, y)     CS_GET_TYPE(currentCS, (x), (y))
#define GET_PART_XVEL(x, y)     CS_GET_XVEL(currentCS, (x), (y))
#define GET_PART_YVEL(x, y)     CS_GET_YVEL(currentCS, (x), (y))
#define GET_PART_COLOR(x, y)    CS_GET_COLOR(currentCS, (x), (y))
#define GET_PART_EFFECT_T(x, y) CS_GET_EFFECT_T(currentCS, (x), (y))
#define GET_PART_LIFE_T(x, y)   CS_GET_LIFE_T(currentCS, (x), (y))
#define GET_PART_HEAT(x, y)     CS_GET_HEAT(currentCS, (x), (y))
#define GET_PART_PFLAGS(x, y)   CS_GET_PFLAGS(currentCS, (x), (y))
#define GET_PART_DURAB(x, y)    CS_GET_DURAB(currentCS, (x), (y))
#define GET_PART_CUSTOM(x, y)   CS_GET_CUSTOM(currentCS, (x), (y))




bool BasicDustBehave(int x, int y){

    if(currentCS == NULL){
        printf("null\n");
    }

    // CONSOLE("works16\n");
    int p_type = GET_PART_TYPE(x, y);
    int d_type = GET_PART_TYPE(x, y+1);
    // printf("x:%d y:%d\n", x, y);
    // printf("pp:%d\n", p_type);
    // printf("dp:%d\n", d_type);
    // CONSOLE("works17\n");
    // CONSOLE("works18\n");

    // if(p_type = SAND){
    //     printf("p:%d ", p_type);
    //     printf("d:%d\n", d_type);
    // }

    if((typeDensityList[d_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[d_type], IS_DUST)){
        // printf("works100\n");
        SWAP_PARTS(x, y, x, y+1);
        // printf("works101\n");
        return TRUE;
    }

    int ld_type = GET_PART_TYPE(x-1, y+1);
    if((typeDensityList[ld_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
        SWAP_PARTS(x, y, x-1, y+1);
        return TRUE;
    }

    int rd_type = GET_PART_TYPE(x+1, y+1);
    if((typeDensityList[rd_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
        SWAP_PARTS(x, y, x+1, y+1);
        return TRUE;
    }

    return FALSE;
}

// bool BasicLiquidBehave(int x, int y){

//     int p_type = GET_PART_TYPE(x, y);
//     int d_type = GET_PART_TYPE(x, y+1);

//     int* p_xvel = &GET_PART_XVEL(x, y);

//     if(typeDensityList[d_type] < typeDensityList[p_type]){
//         if(!CHECK_FLAG(typeFlagsList[d_type], IS_SOLID)){
//             SWAP_PARTS(x, y, x, y+1);
//             return TRUE;
//         }
//     }

//     int ld_type = GET_PART_TYPE(x-1, y+1);
//     if(typeDensityList[ld_type] < typeDensityList[p_type]){
//         if(!CHECK_FLAG(typeFlagsList[ld_type], IS_SOLID)){
//             SWAP_PARTS(x, y, x-1, y+1);
//             return TRUE;
//         }
//     }
//     int rd_type = GET_PART_TYPE(x+1, y+1);
//     if(typeDensityList[rd_type] < typeDensityList[p_type]){
//         if(!CHECK_FLAG(typeFlagsList[rd_type], IS_SOLID)){
//             SWAP_PARTS(x, y, x+1, y+1);
//             return TRUE;
//         }
//     }

    
//     // Particle* r = GET_PARTICLE_AT(WORLD, x + 1, y);
//     // if(typeDensityList[r->type] < typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[r->type], IS_SOLID)){ 
//     //     SwapParticles(WORLD, x, y, x + 1, y);
//     //     return TRUE;
//     // }

//     // Particle* l = GET_PARTICLE_AT(WORLD, x - 1, y);
//     // if(typeDensityList[l->type] < typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[l->type], IS_SOLID)){ 
//     //     SwapParticles(WORLD, x, y, x - 1, y);
//     //     return TRUE;
//     // }

//     int l_type = GET_PART_TYPE(x-1, y);
//     int r_type = GET_PART_TYPE(x+1, y);

//     if(*p_xvel > 1){
//         if(typeDensityList[l_type] < typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
//                 SWAP_PARTS(x, y, x-1, y);
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[r_type] < typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
//                 SWAP_PARTS(x, y, x+1, y);
//                 if(*p_xvel == 1) *p_xvel -= 2; 
//                 *p_xvel -= 1; 
//                 return TRUE;
//             }
//         }
//     } else {
//         if(typeDensityList[r_type] < typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
//                 SWAP_PARTS(x, y, x+1, y);
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[l_type] < typeDensityList[p_type]){ 
//             if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
//                 SWAP_PARTS(x, y, x-1, y);
//                 if(*p_xvel == 1) *p_xvel += 2; 
//                 *p_xvel += 1; 
//                 return TRUE;
//             }
//         }
//     }

//     return FALSE;
// }

// bool BasicGasBehave(int x, int y){

//     int p_type = GET_PART_TYPE(x, y);
//     int *p_xvel = &GET_PART_XVEL(x, y);
//     // Particle* u = GET_PARTICLE_AT(w, x, y - 1);
//     // if(typeDensityList[u->type] > typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[u->type], IS_SOLID)){ 
//     //     (*y) -= 1; 
//     //     return TRUE;
//     // }

//     // Particle* ru = GET_PARTICLE_AT(w, x + 1, y - 1);  
//     // if(typeDensityList[ru->type] > typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[ru->type], IS_SOLID)){ 
//     //     (*y) -= 1, (*x) += 1; 
//     //     return TRUE;
//     // }

//     // Particle* lu = GET_PARTICLE_AT(map, *\x - 1, *y - 1);
//     // if(typeDensityList[lu->type] > typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[lu->type], IS_SOLID)){ 
//     //     (*y) -= 1, (*x) -= 1; 
//     //     return TRUE;
//     // }


//     int lu_type = GET_PART_TYPE(x-1, y-1);
//     int ru_type = GET_PART_TYPE(x+1, y-1);
//     if(*p_xvel > 1){
//         if(typeDensityList[lu_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[lu_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x - 1, y - 1);
//                 // if(p->xvel == 1)p->xvel -= 2; 
//                 *p_xvel -= 1; 
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[ru_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[ru_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x + 1, y - 1);
//                 // if(p->xvel == 1)p->xvel -= 2; 
//                 *p_xvel -= 1; 
//                 return TRUE;
//             }
//         }
//     } else {
//         if(typeDensityList[ru_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[ru_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x + 1, y - 1);
//                 // if(p->xvel == 1)p->xvel += 2; 
//                 *p_xvel += 1; 
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[lu_type] > typeDensityList[p_type]){ 
//             if(!CHECK_FLAG(typeFlagsList[lu_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x - 1, y - 1);
//                 // if(p->xvel == 1)p->xvel += 2; 
//                 *p_xvel += 1; 
//                 return TRUE;
//             }
//         }
//     }



//     // Particle* r = GET_PARTICLE_AT(w, x + 1, y);
//     // if(typeDensityList[r->type] > typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[r->type], IS_SOLID)){ 
//     //     (*x) += 1; 
//     //     return TRUE;
//     // }

//     // Particle* l = GET_PARTICLE_AT(w, x - 1, y);
//     // if(typeDensityList[l->type] > typeDensityList[p->type] && 
//     // !CHECK_FLAG(typeFlagsList[l->type], IS_SOLID)){ 
//     //     (*x) -= 1; 
//     //     return TRUE;
//     // }

//     int l_type = GET_PART_TYPE(x-1, y);
//     int r_type = GET_PART_TYPE(x+1, y);
//     if(*p_xvel > 1){
//         if(typeDensityList[l_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x - 1, y);
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[r_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x + 1, y);
//                 // if(p->xvel == 1)p->xvel -= 2; 
//                 *p_xvel -= 1; 
//                 return TRUE;
//             }
//         }
//     } else {
//         if(typeDensityList[r_type] > typeDensityList[p_type]){
//             if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x + 1, y);
//                 return TRUE;
//             }
//         }
//         if(typeDensityList[l_type] > typeDensityList[p_type]){ 
//             if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
//                 SwapParticles(WORLD, x, y, x - 1, y);
//                 // if(p->xvel == 1)p->xvel += 2; 
//                 *p_xvel += 1; 
//                 return TRUE;
//             }
//         }
//     }

//     return FALSE;
// }

// bool BasicHeatAbsorberBehave(int x, int y){

//     // int absorb = WATER_HEAT_ABSORB * deltaTime / 100;

//     // Particle* d = GET_PARTICLE_AT(w, x, y + 1);
//     // if(CHECK_FLAG(d->iflags, BURNING)){
//     //     d->heat -= absorb;
//     // }
//     // Particle* u = GET_PARTICLE_AT(w, x, y - 1);
//     // if(CHECK_FLAG(u->iflags, BURNING)){
//     //     u->heat -= absorb;
//     // }
//     // Particle* l = GET_PARTICLE_AT(w, x - 1, y);
//     // if(CHECK_FLAG(l->iflags, BURNING)){
//     //     l->heat -= absorb;
//     // }
//     // Particle* r = GET_PARTICLE_AT(w, x + 1, y);
//     // if(CHECK_FLAG(r->iflags, BURNING)){
//     //     r->heat -= absorb;
//     // }

//     return FALSE;
// }

// bool BasicHeatReleaserBehave(int x, int y){

//     // Particle** map = pmap->map;
//     // sim->pMap[*y][*x].heat += FIRE_HEAT_RELEASE_TEMP;
//     // int release = FIRE_HEAT_RELEASE_TEMP * deltaTime / 100;

//     // Particle* d = GET_PARTICLE_AT(w, x, y + 1);
//     // if(!CHECK_FLAG(d->iflags, BURNING)){
//     //     d->heat += release;
//     // }
//     // Particle* u = GET_PARTICLE_AT(w, x, y - 1);
//     // if(!CHECK_FLAG(u->iflags, BURNING)){
//     //     u->heat += release;
//     // }
//     // Particle* l = GET_PARTICLE_AT(w, x - 1, y);
//     // if(!CHECK_FLAG(l->iflags, BURNING)){
//     //     l->heat += release;
//     // }
//     // Particle* r = GET_PARTICLE_AT(w, x + 1, y);
//     // if(!CHECK_FLAG(r->iflags, BURNING)){
//     //     r->heat += release;
//     // }
//     return FALSE;
// }

// bool BasicAcidicBehave(int x, int y){
    
//     int p_type = GET_PART_TYPE(x, y);
//     int *p_efft = &GET_PART_EFFECT_T(x, y);

//     int d_type = GET_PART_TYPE(x, y+1);
//     if(CHECK_FLAG(typeFlagsList[d_type], ACID_HAS_AN_EFFECT)){ 
//         if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
//         else{
//             DELETE_PART(x, y);
//             DELETE_PART(x, y+1);
//         }
//         return TRUE;
//     }

//     int l_type = GET_PART_TYPE(x-1, y);
//     if(CHECK_FLAG(typeFlagsList[l_type], ACID_HAS_AN_EFFECT)){
//         if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
//         else{
//             DELETE_PART(x, y);
//             DELETE_PART(x-1, y);
//         }
//         return TRUE;
//     }

//     int r_type = GET_PART_TYPE(x+1, y);
//     if(CHECK_FLAG(typeFlagsList[r_type], ACID_HAS_AN_EFFECT)){ 
//         if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
//         else{
//             DELETE_PART(x, y);
//             DELETE_PART(x+1, y);
//         }
//         return TRUE;
//     }

//     int u_type = GET_PART_TYPE(x, y-1);
//     if(CHECK_FLAG(typeFlagsList[u_type], ACID_HAS_AN_EFFECT)){ 
//         if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
//         else{
//             DELETE_PART(x, y);
//             DELETE_PART(x, y-1);
//         }
//         return TRUE;
//     }

//     return FALSE;
// }

// bool BasicBurningBehave(int x, int y, PartType type){
//     int p_type = GET_PART_TYPE(x, y);
//     int *p_lifet = &GET_PART_LIFE_T(x, y);
//     int *p_heat = &GET_PART_HEAT(x, y);
//     Color *p_color = &GET_PART_COLOR(x, y);
//     int *p_pf = &GET_PART_PFLAGS(x, y);

//     if(*p_heat > COAL_TO_FIRE_TEMP){
//         *p_lifet++;
//         if(*p_lifet > BURNING_COAL_LIFE_TIME){
//             CreateReplaceParticle(WORLD, SMOKE, x, y);
//             return TRUE;
//         }
//         ChangeColor(&p_color, FIRE_COLORS);
//         SET_FLAG(*p_pf, BURNING);
//         return TRUE;
//     } else {
//         if(CHECK_FLAG(*p_pf, BURNING)){
//             ChangeColor(&p_color, COAL_COLORS);
//         }
//         CLEAR_FLAG(*p_pf, BURNING);
//     }
//     return FALSE;
// }

bool SandBehave(int x, int y){
    return BasicDustBehave(x, y);
}

// bool CoalBehave(int x, int y){
    
//     // Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;

//     changed = (changed || BasicBurningBehave(x, y, COAL)) ? 1 : 0;
//     changed = (changed || BasicDustBehave(x, y)) ? 1 : 0;
//     return changed;
// }

// bool WaterBehave(int x, int y)
// {
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;
//     if(p->heat > WATER_TO_STEAM_TEMP){
//         CreateReplaceParticle(WORLD, STEAM, x, y);
//         changed = TRUE;
//     }

//     // changed = BasicHeatAbsorberBehave(x, y);
//     changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;
//     return changed;
// }

// bool OilBehave(int x, int y)
// {
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;

//     changed = (changed || BasicBurningBehave(x, y, OIL)) ? 1 : 0;
//     changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;

//     return changed;
// }

// bool AcidBehave(int x, int y)
// {
//     bool changed = FALSE;
//     changed = (changed || BasicAcidicBehave(x, y)) ? 1 : 0;
//     changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;

//     return changed;
// }

// bool SteamBehave(int x, int y)
// {
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     p->life_t++;
//     if(p->life_t > STEAM_LIFE_TIME){DeleteParticle(WORLD, x, y);}

//     BasicGasBehave(x, y);

//     return TRUE;
// }

// bool FungusBehave(int x, int y)
// {
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;

//     Particle* d = GET_PARTICLE_AT(WORLD, x, y + 1);   
//     if(typeDensityList[d->type] < typeDensityList[p->type]){
//         SwapParticles(WORLD, x, y, x, y + 1);
//         changed = TRUE;
//     }

//     changed = (changed || BasicBurningBehave(x, y, FUNGUS)) ? 1 : 0;

//     return changed;
// }

// bool FireBehave(int x, int y){

//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     // printf("life:%d effect:%d\n",p->life_t, p->effect_t);
//     // sim->pMap[*y][*x]
    
//     p->life_t++;
//     // printf("life:%d\n", p->life_t);
//     if(p->effect_t < FIRE_EFFECT_TIME){
//         p->effect_t++;
//         return TRUE;
//     }

//     if(p->heat <= 300){
//         if(rand() % 10 > 5){
//             CreateReplaceParticle(WORLD, SMOKE, x, y);
//         } else {
//             DeleteParticle(WORLD, x, y);
//         }
//         return TRUE;
//     }

//     if(p->life_t > FIRE_LIFE_TIME){
//         if(rand() % 10 > 5){
//             CreateReplaceParticle(WORLD, SMOKE, x, y);
//         } else {
//             DeleteParticle(WORLD, x, y);
//         }
//         return TRUE;
//     }

//     BasicHeatReleaserBehave(x, y);

//     return TRUE;
// }

// bool SmokeBehave(int x, int y){

//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     p->life_t++;
//     if(p->life_t > STEAM_LIFE_TIME){DeleteParticle(WORLD, x, y);}

//     BasicGasBehave(x, y);

//     return TRUE;
// }

// bool LavaBehave(int x, int y){
    
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;

//     if(p->heat < 500){
//         CreateReplaceParticle(WORLD, OBSIDIAN, x, y);
//         changed = TRUE;
//     } else {
//         // BasicHeatReleaserBehave(sim, x, y);
//     }
//     changed = BasicLiquidBehave(x, y);

//     return changed;
// }

// bool WoodBehave(int x, int y){
    
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;

//     changed = changed = (changed || BasicBurningBehave(x, y, WOOD)) ? 1 : 0;

//     return changed;
// }

// bool FireSmokeBehave(int x, int y){
//     // printf("works");
//     FireBehave(x, y);
//     BasicGasBehave(x, y);

//     return TRUE;
// }

// bool PowderBehave(int x, int y){

//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;
//     // if(p->durability <= 0){
//     //     Explosion(sim, *x, *y, 10, 100, FIRE_SMOKE);
//     // }
//     // if(p->heat > 200){
//     //     Explosion(sim, *x, *y, 10, 100, PHANTOM);
//     //     AddDirtyRect(sim, *x, *y, 10);
//     //     changed = TRUE;
//     // }
//     changed = BasicDustBehave(x, y);

//     return changed;
// }

// bool ObsidianBehave(int x, int y){
    
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     bool changed = FALSE;
//     int heat = p->heat;
//     if(p->heat > 1500){
//         Color c;
//         ChangeColor(&c, ORANGE);
//         CreateReplaceParticle(WORLD, LAVA, x, y);
//         p->heat = heat;
//         changed = TRUE;
//     }

//     return changed;
// }

// bool PhantomBehave(int x, int y){
    
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     p->life_t++;
//     if(p->life_t > PHANTOM_LIFE_TIME){DeleteParticle(WORLD, x, y);}

//     return TRUE;
// }

// bool SourceBehave(int x, int y){
    
//     Particle* p = GET_PARTICLE_AT(WORLD, x, y);
//     PartType genType = ACID;

//     Particle* d = GET_PARTICLE_AT(WORLD, x, y + 1);
//     Particle* u = GET_PARTICLE_AT(WORLD, x, y - 1);
//     Particle* l = GET_PARTICLE_AT(WORLD, x - 1, y);
//     Particle* r = GET_PARTICLE_AT(WORLD, x + 1, y);

//     if(d->type == AIR){
//         CreateReplaceParticle(WORLD, genType, x, y + 1);
//     }
//     if(u->type == AIR){
//         CreateReplaceParticle(WORLD, genType, x, y - 1);
//     }
//     if(l->type == AIR){
//         CreateReplaceParticle(WORLD, genType, x - 1, y);
//     }
//     if(r->type == AIR){
//         CreateReplaceParticle(WORLD, genType, x + 1, y);
//     }

//     return TRUE;
// }


// void (*CreateParticle)(Chunk* chunk, ParticleType type, int px, int py) = NULL;
// void (*CreateReplaceParticle)(Chunk* chunk, ParticleType type, int px, int py) = NULL;
// void (*ReplaceParticle)(Chunk* chunk, int sx, int sy, int dx, int dy) = NULL;
// void (*DeleteParticle)(Chunk* chunk, int px, int py) = NULL;

// void SwapParticles2(Particle* p1, Particle* p2){
//     Particle tp;
//     int t;

//     SWAP(*p1, *p2, tp);
//     SWAP(p1->p.x, p2->p.x, t);
//     SWAP(p1->p.y, p2->p.y, t);
// }

// void SwapParticles(Chunk* chunk, int x1, int y1, int x2, int y2){
//     Particle tp;
//     Particle* p1 = GET_PARTICLE_AT(chunk, x1, y1);
//     Particle* p2 = GET_PARTICLE_AT(chunk, x2, y2);
//     int t;

//     SWAP(*p1, *p2, tp);
//     SWAP(p1->p.x, p2->p.x, t);
//     SWAP(p1->p.y, p2->p.y, t);
// }