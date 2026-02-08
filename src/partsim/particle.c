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


#define SAND_DISTRIBUTION 84
#define WATER_DISTRIBUTION 40
#define STEAM_DISTRIBUTION 100

bool BasicDistributiveFalling(int x, int y, int prob){
    TYPE_DT p_type = GET_PART_TYPE(x, y);
    TYPE_DT d_type = GET_PART_TYPE(x, y+1);
    TYPE_DT ld_type = GET_PART_TYPE(x-1, y+1);
    TYPE_DT rd_type = GET_PART_TYPE(x+1, y+1);

    int val = rand()%100;
    if(val > 100-prob){
        if((typeDensityList[d_type] < typeDensityList[p_type])
            && !CHECK_FLAG(typeFlagsList[d_type], IS_DUST)){
            SWAP_PARTS(x, y, x, y+1);
            return TRUE;
        }
    }
    else if(val > (100-prob)/2){
        if((typeDensityList[ld_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
            SWAP_PARTS(x, y, x-1, y+1);
            return TRUE;
        }
    }
    else{
        if((typeDensityList[rd_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
            SWAP_PARTS(x, y, x+1, y+1);
            return TRUE;
        }
    }

    return FALSE;
}


bool BasicDistributiveFlying(int x, int y, int prob){
    TYPE_DT p_type = GET_PART_TYPE(x, y);
    TYPE_DT u_type = GET_PART_TYPE(x, y-1);
    TYPE_DT lu_type = GET_PART_TYPE(x-1, y-1);
    TYPE_DT ru_type = GET_PART_TYPE(x+1, y-1);

    int val = rand()%100;
    if(val > 100-prob){
        if((typeDensityList[u_type] > typeDensityList[p_type])
            && !CHECK_FLAG(typeFlagsList[u_type], IS_SOLID)){
            SWAP_PARTS(x, y, x, y-1);
            return TRUE;
        }
    }
    else if(val > (100-prob)/2){
        if((typeDensityList[lu_type] > typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[lu_type], IS_SOLID)){
            SWAP_PARTS(x, y, x-1, y-1);
            return TRUE;
        }
    }
    else{
        if((typeDensityList[ru_type] > typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[ru_type], IS_SOLID)){ 
            SWAP_PARTS(x, y, x+1, y-1);
            return TRUE;
        }
    }

    return FALSE;
}

bool BasicFallingBehave(int x, int y){
    TYPE_DT p_type = GET_PART_TYPE(x, y);
    TYPE_DT d_type = GET_PART_TYPE(x, y+1);

    if(typeDensityList[d_type] < typeDensityList[p_type]){
        SWAP_PARTS(x, y, x, y + 1);
        return TRUE;
    }
    return FALSE;
}

bool BasicDustBehave(int x, int y){

    TYPE_DT p_type = GET_PART_TYPE(x, y);
    TYPE_DT d_type = GET_PART_TYPE(x, y+1);

    if((typeDensityList[d_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[d_type], IS_DUST)){
        // SWAP_PARTS(x, y, x, y+1);
        BasicDistributiveFalling(x, y, SAND_DISTRIBUTION);
        return TRUE;
    }

    TYPE_DT ld_type = GET_PART_TYPE(x-1, y+1);
    if((typeDensityList[ld_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
        SWAP_PARTS(x, y, x-1, y+1);
        return TRUE;
    }

    TYPE_DT rd_type = GET_PART_TYPE(x+1, y+1);
    if((typeDensityList[rd_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
        SWAP_PARTS(x, y, x+1, y+1);
        return TRUE;
    }

    return FALSE;
}

bool BasicLiquidBehave(int x, int y){

    TYPE_DT p_type = GET_PART_TYPE(x, y);
    TYPE_DT d_type = GET_PART_TYPE(x, y+1);

    XVEL_DT* p_xvel = &GET_PART_XVEL(x, y);

    if(typeDensityList[d_type] < typeDensityList[p_type]){
        if(!CHECK_FLAG(typeFlagsList[d_type], IS_SOLID)){
            // SWAP_PARTS(x, y, x, y+1);
            BasicDistributiveFalling(x, y, WATER_DISTRIBUTION);
            return TRUE;
        }
    }

    TYPE_DT ld_type = GET_PART_TYPE(x-1, y+1);
    if(typeDensityList[ld_type] < typeDensityList[p_type]){
        if(!CHECK_FLAG(typeFlagsList[ld_type], IS_SOLID)){
            SWAP_PARTS(x, y, x-1, y+1);
            return TRUE;
        }
    }
    TYPE_DT rd_type = GET_PART_TYPE(x+1, y+1);
    if(typeDensityList[rd_type] < typeDensityList[p_type]){
        if(!CHECK_FLAG(typeFlagsList[rd_type], IS_SOLID)){
            SWAP_PARTS(x, y, x+1, y+1);
            return TRUE;
        }
    }

    
    // Particle* r = GET_PARTICLE_AT(WORLD, x + 1, y);
    // if(typeDensityList[r->type] < typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[r->type], IS_SOLID)){ 
    //     SwapParticles(WORLD, x, y, x + 1, y);
    //     return TRUE;
    // }

    // Particle* l = GET_PARTICLE_AT(WORLD, x - 1, y);
    // if(typeDensityList[l->type] < typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[l->type], IS_SOLID)){ 
    //     SwapParticles(WORLD, x, y, x - 1, y);
    //     return TRUE;
    // }

    TYPE_DT l_type = GET_PART_TYPE(x-1, y);
    TYPE_DT r_type = GET_PART_TYPE(x+1, y);

    if(*p_xvel > 1){
        if(typeDensityList[l_type] < typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
                SWAP_PARTS(x, y, x-1, y);
                return TRUE;
            }
        }
        if(typeDensityList[r_type] < typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
                SWAP_PARTS(x, y, x+1, y);
                if(*p_xvel == 1) *p_xvel -= 2; 
                *p_xvel -= 1; 
                return TRUE;
            }
        }
    } else {
        if(typeDensityList[r_type] < typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
                SWAP_PARTS(x, y, x+1, y);
                return TRUE;
            }
        }
        if(typeDensityList[l_type] < typeDensityList[p_type]){ 
            if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
                SWAP_PARTS(x, y, x-1, y);
                if(*p_xvel == 1) *p_xvel += 2; 
                *p_xvel += 1; 
                return TRUE;
            }
        }
    }

    return FALSE;
}

bool BasicGasBehave(int x, int y){

    TYPE_DT p_type = GET_PART_TYPE(x, y);
    XVEL_DT *p_xvel = &GET_PART_XVEL(x, y);
    // Particle* u = GET_PARTICLE_AT(w, x, y - 1);
    // if(typeDensityList[u->type] > typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[u->type], IS_SOLID)){ 
    //     (*y) -= 1; 
    //     return TRUE;
    // }

    // Particle* ru = GET_PARTICLE_AT(w, x + 1, y - 1);  
    // if(typeDensityList[ru->type] > typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[ru->type], IS_SOLID)){ 
    //     (*y) -= 1, (*x) += 1; 
    //     return TRUE;
    // }

    // Particle* lu = GET_PARTICLE_AT(map, *\x - 1, *y - 1);
    // if(typeDensityList[lu->type] > typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[lu->type], IS_SOLID)){ 
    //     (*y) -= 1, (*x) -= 1; 
    //     return TRUE;
    // }

    if(BasicDistributiveFlying(x, y, STEAM_DISTRIBUTION) == TRUE) return TRUE;

    // TYPE_DT lu_type = GET_PART_TYPE(x-1, y-1);
    // TYPE_DT ru_type = GET_PART_TYPE(x+1, y-1);
    // if(*p_xvel > 1){
    //     if(typeDensityList[lu_type] > typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[lu_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x - 1, y - 1);
    //             // if(p->xvel == 1)p->xvel -= 2; 
    //             *p_xvel -= 1; 
    //             return TRUE;
    //         }
    //     }
    //     if(typeDensityList[ru_type] > typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[ru_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x + 1, y - 1);
    //             // if(p->xvel == 1)p->xvel -= 2; 
    //             *p_xvel -= 1; 
    //             return TRUE;
    //         }
    //     }
    // } else {
    //     if(typeDensityList[ru_type] > typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[ru_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x + 1, y - 1);
    //             // if(p->xvel == 1)p->xvel += 2; 
    //             *p_xvel += 1; 
    //             return TRUE;
    //         }
    //     }
    //     if(typeDensityList[lu_type] > typeDensityList[p_type]){ 
    //         if(!CHECK_FLAG(typeFlagsList[lu_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x - 1, y - 1);
    //             // if(p->xvel == 1)p->xvel += 2; 
    //             *p_xvel += 1; 
    //             return TRUE;
    //         }
    //     }
    // }


    // Particle* r = GET_PARTICLE_AT(w, x + 1, y);
    // if(typeDensityList[r->type] > typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[r->type], IS_SOLID)){ 
    //     (*x) += 1; 
    //     return TRUE;
    // }

    // Particle* l = GET_PARTICLE_AT(w, x - 1, y);
    // if(typeDensityList[l->type] > typeDensityList[p->type] && 
    // !CHECK_FLAG(typeFlagsList[l->type], IS_SOLID)){ 
    //     (*x) -= 1; 
    //     return TRUE;
    // }

    TYPE_DT l_type = GET_PART_TYPE(x-1, y);
    TYPE_DT r_type = GET_PART_TYPE(x+1, y);
    if(*p_xvel > 1){
        if(typeDensityList[l_type] > typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
                SWAP_PARTS(x, y, x - 1, y);
                return TRUE;
            }
        }
        if(typeDensityList[r_type] > typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
                SWAP_PARTS(x, y, x + 1, y);
                // if(p->xvel == 1)p->xvel -= 2; 
                *p_xvel -= 1; 
                return TRUE;
            }
        }
    } else {
        if(typeDensityList[r_type] > typeDensityList[p_type]){
            if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
                SWAP_PARTS(x, y, x + 1, y);
                return TRUE;
            }
        }
        if(typeDensityList[l_type] > typeDensityList[p_type]){ 
            if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
                SWAP_PARTS(x, y, x - 1, y);
                // if(p->xvel == 1)p->xvel += 2; 
                *p_xvel += 1; 
                return TRUE;
            }
        }
    }

    return FALSE;
}

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

bool BasicAcidicBehave(int x, int y){
    
    TYPE_DT p_type = GET_PART_TYPE(x, y);
    EFFECTT_DT *p_efft = &GET_PART_EFFECT_T(x, y);

    TYPE_DT d_type = GET_PART_TYPE(x, y+1);
    if(CHECK_FLAG(typeFlagsList[d_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x, y+1);
        }
        return TRUE;
    }

    TYPE_DT l_type = GET_PART_TYPE(x-1, y);
    if(CHECK_FLAG(typeFlagsList[l_type], ACID_HAS_AN_EFFECT)){
        if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x-1, y);
        }
        return TRUE;
    }

    TYPE_DT r_type = GET_PART_TYPE(x+1, y);
    if(CHECK_FLAG(typeFlagsList[r_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x+1, y);
        }
        return TRUE;
    }

    TYPE_DT u_type = GET_PART_TYPE(x, y-1);
    if(CHECK_FLAG(typeFlagsList[u_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft < ACID_EFFECT_TIME) *p_efft++;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x, y-1);
        }
        return TRUE;
    }

    return FALSE;
}

bool BasicBurningBehave(int x, int y, PartType type){
    TYPE_DT p_type = GET_PART_TYPE(x, y);
    LIFET_DT *p_lifet = &GET_PART_LIFE_T(x, y);
    HEAT_DT p_heat = GET_PART_HEAT(x, y);
    COLOR_DT *p_color = &GET_PART_COLOR(x, y);
    PFLAGS_DT *p_pf = &GET_PART_PFLAGS(x, y);

    if(p_heat > COAL_TO_FIRE_TEMP){
        *p_lifet++;
        if(*p_lifet > BURNING_COAL_LIFE_TIME){
            REPLACE_PART(x, y, SMOKE);
            return TRUE;
        }
        // ChangeColor(&p_color, FIRE_COLORS);
        SET_FLAG(*p_pf, BURNING);
        return TRUE;
    } else {
        if(CHECK_FLAG(*p_pf, BURNING)){
            // ChangeColor(&p_color, COAL_COLORS);
        }
        CLEAR_FLAG(*p_pf, BURNING);
    }
    return FALSE;
}

bool SandBehave(int x, int y){
    return BasicDustBehave(x, y);
}

bool CoalBehave(int x, int y){
    
    bool changed = FALSE;
    changed = (changed || BasicBurningBehave(x, y, COAL)) ? 1 : 0;
    changed = (changed || BasicDustBehave(x, y)) ? 1 : 0;
    return changed;
}

bool WaterBehave(int x, int y)
{
    HEAT_DT p_heat = GET_PART_HEAT(x, y);
    bool changed = FALSE;
    if(p_heat > WATER_TO_STEAM_TEMP){
        REPLACE_PART(x, y, STEAM);
        changed = TRUE;
    }

    changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;
    return changed;
}

bool OilBehave(int x, int y)
{
    bool changed = FALSE;
    changed = (changed || BasicBurningBehave(x, y, OIL)) ? 1 : 0;
    changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;

    return changed;
}

bool AcidBehave(int x, int y)
{
    bool changed = FALSE;
    changed = (changed || BasicAcidicBehave(x, y)) ? 1 : 0;
    changed = (changed || BasicLiquidBehave(x, y)) ? 1 : 0;

    return changed;
}

bool SteamBehave(int x, int y)
{
    LIFET_DT *p_lifet = &GET_PART_LIFE_T(x, y);
    (*p_lifet)++;
    if(*p_lifet > STEAM_LIFE_TIME){DELETE_PART(x, y);}

    BasicGasBehave(x, y);

    return TRUE;
}

bool FungusBehave(int x, int y)
{
    bool changed = FALSE;
    changed = (changed || BasicFallingBehave(x, y)) ? 1 : 0;
    changed = (changed || BasicBurningBehave(x, y, FUNGUS)) ? 1 : 0;

    return changed;
}

bool FireBehave(int x, int y){

    LIFET_DT *p_lifet = &GET_PART_LIFE_T(x, y);
    EFFECTT_DT *p_effectt = &GET_PART_EFFECT_T(x, y);
    HEAT_DT p_heat = GET_PART_HEAT(x, y);
    
    (*p_lifet)++;
    if((*p_effectt) < FIRE_EFFECT_TIME){
        (*p_effectt)++;
        return TRUE;
    }

    if(p_heat <= 300){
        if(rand() % 10 > 5){
            REPLACE_PART(x, y, SMOKE);
        } else {
            DELETE_PART(x, y);
        }
        return TRUE;
    }

    if((*p_lifet) > FIRE_LIFE_TIME){
        if(rand() % 10 > 5){
            REPLACE_PART(x, y, SMOKE);
        } else {
            DELETE_PART(x, y);
        }
        return TRUE;
    }

    return TRUE;
}

bool SmokeBehave(int x, int y){

    LIFET_DT *p_lifet = &GET_PART_LIFE_T(x, y);
    (p_lifet)++;
    if(*p_lifet > STEAM_LIFE_TIME){DELETE_PART(x, y);}

    BasicGasBehave(x, y);

    return TRUE;
}

bool LavaBehave(int x, int y){
    
    HEAT_DT p_heat = GET_PART_HEAT(x, y);
    bool changed = FALSE;

    if(p_heat < 500){
        REPLACE_PART(x, y, OBSIDIAN);
        changed = TRUE;
    } else {
        // BasicHeatReleaserBehave(sim, x, y);
    }
    changed = BasicLiquidBehave(x, y);

    return changed;
}

bool WoodBehave(int x, int y){
    
    bool changed = FALSE;
    changed = changed = (changed || BasicBurningBehave(x, y, WOOD)) ? 1 : 0;

    return changed;
}

bool FireSmokeBehave(int x, int y){

    FireBehave(x, y);
    BasicGasBehave(x, y);

    return TRUE;
}

bool PowderBehave(int x, int y){
    
    bool changed = FALSE;
    // Particle* p = GET_PARTICLE_AT(WORLD, x, y);
    // if(p->durability <= 0){
    //     Explosion(sim, *x, *y, 10, 100, FIRE_SMOKE);
    // }
    // if(p->heat > 200){
    //     Explosion(sim, *x, *y, 10, 100, PHANTOM);
    //     AddDirtyRect(sim, *x, *y, 10);
    //     changed = TRUE;
    // }
    changed = BasicDustBehave(x, y);

    return changed;
}

bool ObsidianBehave(int x, int y){
    
    bool changed = FALSE;
    HEAT_DT p_heat = GET_PART_HEAT(x, y);
    COLOR_DT* p_color = &GET_PART_COLOR(x, y);

    if(p_heat > 1500){
        ChangeColor(p_color, ORANGE);
        REPLACE_PART(x, y, LAVA);
        changed = TRUE;
    }

    return changed;
}

bool PhantomBehave(int x, int y){
    
    LIFET_DT *p_lifet = &GET_PART_LIFE_T(x, y);
    (*p_lifet)++;
    if(*p_lifet > PHANTOM_LIFE_TIME){DELETE_PART(x, y);}

    return TRUE;
}

bool SourceBehave(int x, int y){
    
    PartType genType = ACID;

    TYPE_DT d_type = GET_PART_TYPE(x, y+1);
    TYPE_DT u_type = GET_PART_TYPE(x, y-1);
    TYPE_DT l_type = GET_PART_TYPE(x-1, y);
    TYPE_DT r_type = GET_PART_TYPE(x+1, y);

    if(d_type == AIR){
        REPLACE_PART(x, y+1, genType);
    }
    if(u_type == AIR){
        REPLACE_PART(x, y-1, genType);
    }
    if(l_type == AIR){
        REPLACE_PART(x-1, y, genType);
    }
    if(r_type == AIR){
        REPLACE_PART(x+1, y, genType);
    }

    return TRUE;
}


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