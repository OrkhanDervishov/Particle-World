#include "particle.h"

int gravity = 1;

// Particle params
int STEAM_LIFE_TIME = 350;
int SMOKE_LIFE_TIME = 1000;
int FIRE_LIFE_TIME = 100;
int PHANTOM_LIFE_TIME = 20;

int ACID_EFFECT_TIME = 50;
int FIRE_EFFECT_TIME = 1000;
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

#define GRAVITY_Y gravity
#define SAND_DISTRIBUTION 84
#define WATER_FALLING_DISTRIBUTION 40
#define WATER_FLOWING_DISTRIBUTION 50
#define STEAM_DISTRIBUTION 20

bool BasicDistributiveFalling(int x, int y, int prob){
    part_type_t p_type = GET_PART_TYPE(x, y);
    part_type_t d_type = GET_PART_TYPE(x, y+GRAVITY_Y);
    part_type_t ld_type = GET_PART_TYPE(x-1, y+GRAVITY_Y);
    part_type_t rd_type = GET_PART_TYPE(x+1, y+GRAVITY_Y);

    int val = rand()%100;
    if(val >= 100-prob){
        if((typeDensityList[d_type] < typeDensityList[p_type])
            && !CHECK_FLAG(typeFlagsList[d_type], IS_DUST)){
            SWAP_PARTS(x, y, x, y+GRAVITY_Y);
            return TRUE;
        }
    }
    if(val >= (100-prob)/2){
        if((typeDensityList[ld_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
            SWAP_PARTS(x, y, x-1, y+GRAVITY_Y);
            return TRUE;
        }
    }
    if(val >= 0){
        if((typeDensityList[rd_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
            SWAP_PARTS(x, y, x+1, y+GRAVITY_Y);
            return TRUE;
        }
    }

    return FALSE;
}

bool BasicDistributiveLiquid(int x, int y, int prob){
    part_type_t p_type = GET_PART_TYPE(x, y);
    // part_type_t d_type = GET_PART_TYPE(x, y+GRAVITY_Y);
    part_type_t ld_type = GET_PART_TYPE(x-1, y+GRAVITY_Y);
    part_type_t rd_type = GET_PART_TYPE(x+1, y+GRAVITY_Y);
    part_type_t l_type = GET_PART_TYPE(x-1, y);
    part_type_t r_type = GET_PART_TYPE(x+1, y);
    part_xvel_t* p_xvel = &GET_PART_XVEL(x, y);

    // printf("(100-prob): %d\n", 100-prob);
    // printf("(100-2*prob): %d\n", (100-2*prob));
    // printf("(100-2*prob)/2: %d\n", (100-2*prob)/2);
    // printf("0: %d\n\n", 0);

    int val = rand()%100;
    if(val <= (0+prob)){
        if((typeDensityList[ld_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
            SWAP_PARTS(x, y, x-1, y+GRAVITY_Y);
            return TRUE;
        }
    }
    if(val > (100-prob)){
        if((typeDensityList[rd_type] < typeDensityList[p_type])
        && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
            SWAP_PARTS(x, y, x+1, y+GRAVITY_Y);
            return TRUE;
        }
    }
    if(val > (50+prob)){
        // if((typeDensityList[r_type] < typeDensityList[p_type])
        // && !CHECK_FLAG(typeFlagsList[r_type], IS_DUST)){ 
        //     SWAP_PARTS(x, y, x+1, y);
        //     return TRUE;
        // }
    }
    if(val <= 50-prob){
        // if((typeDensityList[r_type] < typeDensityList[p_type])
        // && !CHECK_FLAG(typeFlagsList[r_type], IS_DUST)){ 
            //     SWAP_PARTS(x, y, x+1, y);
            //     return TRUE;
            // }
        // if((typeDensityList[l_type] < typeDensityList[p_type])
        // && !CHECK_FLAG(typeFlagsList[l_type], IS_DUST)){
        //     SWAP_PARTS(x, y, x-1, y);
        //     return TRUE;
        // }
    }
        
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


bool BasicDistributiveFlying(int x, int y, int prob){
    part_type_t p_type = GET_PART_TYPE(x, y);
    part_type_t u_type = GET_PART_TYPE(x, y-GRAVITY_Y);
    part_type_t lu_type = GET_PART_TYPE(x-1, y-GRAVITY_Y);
    part_type_t ru_type = GET_PART_TYPE(x+1, y-GRAVITY_Y);
    part_lifet_t* p_lifet = &GET_PART_LIFE_T(x, y);

    int val = rand()%100;
    if(val > 100-prob){
        if((typeDensityList[u_type] > typeDensityList[p_type])
            && !CHECK_FLAG(typeFlagsList[u_type], IS_SOLID)){
            (*p_lifet)--;
            SWAP_PARTS(x, y, x, y-GRAVITY_Y);
            return TRUE;
        }
    }
    if(val > (100-prob)/2 + 5){
        if((typeDensityList[lu_type] > typeDensityList[p_type])
        && CHECK_FLAG(typeFlagsList[lu_type], IS_GAS)){
            SWAP_PARTS(x, y, x-1, y-GRAVITY_Y);
            return TRUE;
        }
    }
    else{
        if((typeDensityList[ru_type] > typeDensityList[p_type])
        && CHECK_FLAG(typeFlagsList[ru_type], IS_GAS)){ 
            SWAP_PARTS(x, y, x+1, y-GRAVITY_Y);
            return TRUE;
        }
    }

    return FALSE;
}

bool BasicFallingBehave(int x, int y){
    part_type_t p_type = GET_PART_TYPE(x, y);
    part_type_t d_type = GET_PART_TYPE(x, y+GRAVITY_Y);

    if(typeDensityList[d_type] < typeDensityList[p_type]){
        BasicDistributiveFalling(x, y, SAND_DISTRIBUTION);
        return TRUE;
    }
    return FALSE;
}

bool BasicDustBehave(int x, int y){

    part_type_t p_type = GET_PART_TYPE(x, y);
    part_type_t d_type = GET_PART_TYPE(x, y+GRAVITY_Y);

    if((typeDensityList[d_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[d_type], IS_DUST)){
        // SWAP_PARTS(x, y, x, y+1);
        BasicDistributiveFalling(x, y, SAND_DISTRIBUTION);
        return TRUE;
    }

    part_type_t ld_type = GET_PART_TYPE(x-1, y+GRAVITY_Y);
    if((typeDensityList[ld_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[ld_type], IS_DUST)){
        SWAP_PARTS(x, y, x-1, y+GRAVITY_Y);
        return TRUE;
    }

    part_type_t rd_type = GET_PART_TYPE(x+1, y+GRAVITY_Y);
    if((typeDensityList[rd_type] < typeDensityList[p_type])
     && !CHECK_FLAG(typeFlagsList[rd_type], IS_DUST)){ 
        SWAP_PARTS(x, y, x+1, y+GRAVITY_Y);
        return TRUE;
    }

    return FALSE;
}

bool BasicLiquidBehave(int x, int y){

    part_type_t p_type = GET_PART_TYPE(x, y);
    part_type_t d_type = GET_PART_TYPE(x, y+GRAVITY_Y);

    part_xvel_t* p_xvel = &GET_PART_XVEL(x, y);

    
    if(BasicDistributiveFalling(x, y, WATER_FALLING_DISTRIBUTION)) return TRUE;
    if(BasicDistributiveLiquid(x, y, WATER_FLOWING_DISTRIBUTION)) return TRUE;

    // if(typeDensityList[d_type] < typeDensityList[p_type]){
    //     if(!CHECK_FLAG(typeFlagsList[d_type], IS_SOLID)){
    //         // SWAP_PARTS(x, y, x, y+1);
    //         return TRUE;
    //     }
    // }
    part_type_t l_type = GET_PART_TYPE(x-1, y);
    part_type_t ld_type = GET_PART_TYPE(x-1, y+GRAVITY_Y);
    part_type_t r_type = GET_PART_TYPE(x+1, y);
    part_type_t rd_type = GET_PART_TYPE(x+1, y+GRAVITY_Y);

    // if(typeDensityList[ld_type] < typeDensityList[p_type] && typeDensityList[rd_type] < typeDensityList[p_type]){
    //     int chance = rand() % 100;
    //     if(chance >= 50){
    //         if(!CHECK_FLAG(typeFlagsList[ld_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x-1, y+GRAVITY_Y);
    //             return TRUE;
    //         }
    //     }
    //     else {
    //         if(!CHECK_FLAG(typeFlagsList[rd_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x+1, y+GRAVITY_Y);
    //             return TRUE;
    //         }
    //     }
    // }
    // if(typeDensityList[l_type] < typeDensityList[p_type] && typeDensityList[r_type] < typeDensityList[p_type]){
    //     int chance = rand() % 100;
    //     if(chance >= 50){
    //         if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x-1, y);
    //             return TRUE;
    //         }
    //     }
    //     else {
    //         if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x+1, y);
    //             return TRUE;
    //         }
    //     }
    // }

    // if(typeDensityList[ld_type] < typeDensityList[p_type]){
    //     // if(typeDensityList[l_type] < typeDensityList[p_type]){
    //     //     if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
    //     //         SWAP_PARTS(x, y, x-1, y);
    //     //         return TRUE;
    //     //     }
    //     // }
    //     if(!CHECK_FLAG(typeFlagsList[ld_type], IS_SOLID)){
    //         SWAP_PARTS(x, y, x-1, y+GRAVITY_Y);
    //         return TRUE;
    //     }
    // }
    // if(typeDensityList[rd_type] < typeDensityList[p_type]){
    //     // if(typeDensityList[r_type] < typeDensityList[p_type]){
    //     //     if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
    //     //         SWAP_PARTS(x, y, x+1, y);
    //     //         return TRUE;
    //     //     }
    //     // }
    //     if(!CHECK_FLAG(typeFlagsList[rd_type], IS_SOLID)){
    //         SWAP_PARTS(x, y, x+1, y+GRAVITY_Y);
    //         return TRUE;
    //     }
    // }


    // if(*p_xvel > 1){
    //     if(typeDensityList[l_type] < typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x-1, y);
    //             return TRUE;
    //         }
    //     }
    //     if(typeDensityList[r_type] < typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x+1, y);
    //             if(*p_xvel == 1) *p_xvel -= 2; 
    //             *p_xvel -= 1; 
    //             return TRUE;
    //         }
    //     }
    // } else {
    //     if(typeDensityList[r_type] < typeDensityList[p_type]){
    //         if(!CHECK_FLAG(typeFlagsList[r_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x+1, y);
    //             return TRUE;
    //         }
    //     }
    //     if(typeDensityList[l_type] < typeDensityList[p_type]){ 
    //         if(!CHECK_FLAG(typeFlagsList[l_type], IS_SOLID)){
    //             SWAP_PARTS(x, y, x-1, y);
    //             if(*p_xvel == 1) *p_xvel += 2; 
    //             *p_xvel += 1; 
    //             return TRUE;
    //         }
    //     }
    // }

    return FALSE;
}

bool BasicGasBehave(int x, int y){

    part_type_t p_type = GET_PART_TYPE(x, y);
    part_xvel_t *p_xvel = &GET_PART_XVEL(x, y);

    if(BasicDistributiveFlying(x, y, STEAM_DISTRIBUTION) == TRUE) return TRUE;

    part_type_t l_type = GET_PART_TYPE(x-1, y);
    part_type_t r_type = GET_PART_TYPE(x+1, y);
    if(*p_xvel > 1){
        if(typeDensityList[l_type] > typeDensityList[p_type]){
            if(CHECK_FLAG(typeFlagsList[l_type], IS_GAS)){
                SWAP_PARTS(x, y, x - 1, y);
                return TRUE;
            }
        }
        if(typeDensityList[r_type] > typeDensityList[p_type]){
            if(CHECK_FLAG(typeFlagsList[r_type], IS_GAS)){
                SWAP_PARTS(x, y, x + 1, y);
                // if(p->xvel == 1)p->xvel -= 2; 
                *p_xvel -= 1; 
                return TRUE;
            }
        }
    } else {
        if(typeDensityList[r_type] > typeDensityList[p_type]){
            if(CHECK_FLAG(typeFlagsList[r_type], IS_GAS)){
                SWAP_PARTS(x, y, x + 1, y);
                return TRUE;
            }
        }
        if(typeDensityList[l_type] > typeDensityList[p_type]){ 
            if(CHECK_FLAG(typeFlagsList[l_type], IS_GAS)){
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
    
    part_type_t p_type = GET_PART_TYPE(x, y);
    part_effectt_t *p_efft = &GET_PART_EFFECT_T(x, y);

    part_type_t d_type = GET_PART_TYPE(x, y+1);
    if(CHECK_FLAG(typeFlagsList[d_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft > 0) (*p_efft)--;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x, y+1);
        }
        return TRUE;
    }

    part_type_t l_type = GET_PART_TYPE(x-1, y);
    if(CHECK_FLAG(typeFlagsList[l_type], ACID_HAS_AN_EFFECT)){
        if(*p_efft > 0) (*p_efft)--;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x-1, y);
        }
        return TRUE;
    }

    part_type_t r_type = GET_PART_TYPE(x+1, y);
    if(CHECK_FLAG(typeFlagsList[r_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft > 0) (*p_efft)--;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x+1, y);
        }
        return TRUE;
    }

    part_type_t u_type = GET_PART_TYPE(x, y-1);
    if(CHECK_FLAG(typeFlagsList[u_type], ACID_HAS_AN_EFFECT)){ 
        if(*p_efft > 0) (*p_efft)--;
        else{
            DELETE_PART(x, y);
            DELETE_PART(x, y-1);
        }
        return TRUE;
    }

    return FALSE;
}

bool BasicBurningBehave(int x, int y, PartType type){
    part_type_t p_type = GET_PART_TYPE(x, y);
    part_lifet_t *p_lifet = &GET_PART_LIFE_T(x, y);
    part_heat_t p_heat = GET_PART_HEAT(x, y);
    part_color_t *p_color = &GET_PART_COLOR(x, y);
    part_pflags_t *p_pf = &GET_PART_PFLAGS(x, y);

    if(p_heat > 1000){
        // printf("works\n");
        (*p_lifet)--;
        if(*p_lifet > BURNING_COAL_LIFE_TIME){
            REPLACE_PART(x, y, SMOKE);
            return TRUE;
        }
        ChangeColor(p_color, FIRE_COLORS);
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
    changed = (changed || BasicDustBehave(x, y)) ? 1 : 0;
    changed = (changed || BasicBurningBehave(x, y, COAL)) ? 1 : 0;
    return changed;
}

bool WaterBehave(int x, int y)
{
    part_heat_t p_heat = GET_PART_HEAT(x, y);
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
    part_lifet_t *p_lifet = &GET_PART_LIFE_T(x, y);
    (*p_lifet)--;
    if(*p_lifet == 0){DELETE_PART(x, y);}

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

    part_lifet_t *p_lifet = &GET_PART_LIFE_T(x, y);
    part_effectt_t *p_effectt = &GET_PART_EFFECT_T(x, y);
    part_heat_t p_heat = GET_PART_HEAT(x, y);
    
    (*p_lifet)--;
    (*p_effectt)--;

    if((*p_effectt) == 0){
        if(rand() % 10 > 5){
            REPLACE_PART(x, y, SMOKE);
        } else {
            DELETE_PART(x, y);
        }
        return TRUE;
    }

    if((*p_lifet) == 0){
        if(rand() % 10 > 5){
            REPLACE_PART(x, y, SMOKE);
        } else {
            DELETE_PART(x, y);
        }
        return TRUE;
    }

    // if(p_heat <= 300){
    //     if(rand() % 10 > 5){
    //         REPLACE_PART(x, y, SMOKE);
    //     } else {
    //         DELETE_PART(x, y);
    //     }
    //     return TRUE;
    // }


    return TRUE;
}

bool SmokeBehave(int x, int y){

    part_lifet_t *p_lifet = &GET_PART_LIFE_T(x, y);
    (*p_lifet)--;
    if(*p_lifet == 0){DELETE_PART(x, y);}

    BasicGasBehave(x, y);

    return TRUE;
}

bool LavaBehave(int x, int y){
    
    part_heat_t p_heat = GET_PART_HEAT(x, y);
    bool changed = FALSE;

    if(p_heat < 500){
        // printf("%d\n", p_heat);
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
    part_heat_t p_heat = GET_PART_HEAT(x, y);
    part_color_t* p_color = &GET_PART_COLOR(x, y);

    if(p_heat > 1500){
        ChangeColor(p_color, ORANGE);
        REPLACE_PART(x, y, LAVA);
        changed = TRUE;
    }

    return changed;
}

bool PhantomBehave(int x, int y){
    
    part_lifet_t *p_lifet = &GET_PART_LIFE_T(x, y);
    (*p_lifet)--;
    if(*p_lifet == 0){DELETE_PART(x, y);}

    return TRUE;
}

bool SourceBehave(int x, int y){
    
    PartType genType = ACID;

    part_type_t d_type = GET_PART_TYPE(x, y+1);
    part_type_t u_type = GET_PART_TYPE(x, y-1);
    part_type_t l_type = GET_PART_TYPE(x-1, y);
    part_type_t r_type = GET_PART_TYPE(x+1, y);

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