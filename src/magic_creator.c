#ifndef MAGIC_CREATOR_C_
#define MAGIC_CREATOR_C_

#include "game.h"
#include "la.h"


typedef enum{
    UNKNOWN_MAGIC = 0,
    FIRE_MAGIC,
    WATER_MAGIC,
    WIND_MAGIC,
    EARTH_MAGIC,
    LIGHT_MAGIC
} MagicType;

typedef enum{
    FIRE_SIGIL = 0,
    WATER_SIGIL,
    WIND_SIGIL,
    EARTH_SIGIL,
    LIGHT_SIGIL,
    COLUMN_SIGN,
    LEVITATION_SIGN,
    CONVERGENCE_SIGN,
    // Unimplemented
    PULL_SIGN,
    DISPERSION_SIGN,
    REGION_SIGN,
    COLLECTION_SIGN,
    // Advanced
    BILLOW_SIGN,
    REPETITION_SIGN,
    WEAVE_SIGN,
    FLOAT_SIGN,
    COOL_SIGN,
} SpellElementType;


typedef struct{
    float start_angle;    
    float end_angle;
} Sector;

typedef struct{
    Sector *items;
    size_t count;
    size_t capacity;
} Sectors;

typedef struct{
    float *items;
    size_t count;
    size_t capacity;    
} Floats;

typedef struct{
    vec2f *items;
    size_t count;
    size_t capacity;    
} Vectors2f;

typedef struct{
    vec3f *items;
    size_t count;
    size_t capacity;    
} Vectors3f;

static int compare_float(const void *a, const void *b){
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

static Sectors points_to_sectors(float* point_angles, size_t count){
    Sectors sectors = {0};
    if(count <= 0) return sectors;
    if(point_angles == NULL) return sectors;

    float *sorted_point_angles = (float*)malloc(sizeof(float) * count);
    for(size_t i = 0; i < count; i++){
        sorted_point_angles[i] = point_angles[i];
    }
    qsort(sorted_point_angles, count, sizeof(sorted_point_angles[0]), compare_float);

    float *midpoint_angles = (float*)malloc(sizeof(float) * count);

    if(!midpoint_angles){
        free(midpoint_angles);
        return sectors;
    }

    // Calc midpoint angles -> (n + (n+1))/2
    for(size_t i = 0; i < count-1; i++){
        midpoint_angles[i] = (sorted_point_angles[i] + sorted_point_angles[i+1])/2.0f;
    }
    midpoint_angles[count-1] = fmodf((sorted_point_angles[count-1] + 2*PI_CONST + sorted_point_angles[0])/2.0f, 2*PI_CONST);


    // Calc sectors
    Sector sector = (Sector){midpoint_angles[count-1], midpoint_angles[0]};
    da_append(sectors, sector);
    for(size_t i = 1; i < count; i++){
        Sector sector = (Sector){midpoint_angles[i-1], midpoint_angles[i]};
        da_append(sectors, sector);
    }
    

    Sectors sectors_unsorted = {0};
    da_reserve(sectors_unsorted, sectors.count);
    sectors_unsorted.count = sectors.count;

    for(size_t i = 0; i < count; i++){
        for(size_t j = 0; j < count; j++){
            if(point_angles[i] == sorted_point_angles[j]){
                sectors_unsorted.items[i] = sectors.items[j];
            }
        }
    }

    free(midpoint_angles);
    free(sorted_point_angles);
    free(sectors.items);
    return sectors_unsorted;
}




typedef struct{
    SpellElementType type;
    vec3f pos;
    vec2f direction;
    float size;
} SpellElement;

typedef struct{
    SpellElement* items;
    size_t count;
    size_t capacity;
} SpellElements;

#define MAGIC_RING_ORIGIN_VEC2(ring) (vec2f){(ring).radius, 0.0f}
typedef struct{
    vec3f center;
    float radius;
} MagicRing;

typedef struct{
    Sector sector;
    vec3f pull_vector;
} MagicPullSector;

typedef struct{
    MagicPullSector* items;
    size_t count;
    size_t capacity;
} MagicPullSectors;


typedef struct{
    MagicType magic;
    MagicRing ring;
    vec3f position;

    float collect_scale;
    float convergence;

    vec3f direction;
    float force_mag;

    MagicPullSectors pull_sectors;
    vec3f pull_direction;
    float pull_mag;

    float density;
    float spread;

    bool is_active;
} Spell;

typedef struct{
    vec3f pos;
    vec3f velocity;
} MagicParticle;


typedef struct{
    MagicParticle* items;
    size_t count;
    size_t capacity;
} MagicParticles;


typedef struct{
    MagicType type;
    vec3f pos;
    MagicParticles parts;
    MagicPullSectors sectors;
    float power;
    float duration;
} Magic;

#define add_type(Elem_type, Pos, Direct, Len, Arr)\
do{\
    SpellElement elem = {\
        .type = (Elem_type),\
        .pos = (vec3f){\
            .x = (Pos).x,\
            .y = (Pos).y\
        },\
        .direction = (Direct),\
        .size = (Len)\
    };\
    da_append((Arr), elem);\
}while(0)
 






static void handle_column_signs(Spell* spell, SpellElements elems){
 
    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == COLUMN_SIGN){
            vec3f current_force_vec3 = vec3_scale(spell->direction, spell->force_mag);
            // column scalar is defined by the length of column sign
            vec3f new_force_vec3 = vec3_scale(vec3_normalize((vec3f){elem.direction.x, elem.direction.y, 50.f}), 50.0f);
            vec3f force_vec3 = vec3_sum(current_force_vec3, new_force_vec3);
            spell->force_mag = vec3_length(force_vec3);
            spell->direction = vec3_normalize(force_vec3);
        }
    }
    
    // Spread
    float angle_sum = 0;
    int column_count = 0;
    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == COLUMN_SIGN){
            float angle = vec3_get_angle(spell->direction, vec3_sub(spell->ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f}));
            // if(angle < 0.0f) angle = -angle;
            angle_sum += angle;
            column_count++;
            // printf("angle: %f\n", vec3_get_angle(spell->direction, vec3_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})));
        }
    }
    if(column_count != 0) spell->spread = 0.3f*angle_sum/(float)column_count;
}

static void handle_levitation_signs(Spell* spell, SpellElements elems){
    
    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == LEVITATION_SIGN){
            // levitation scalar is defined by the length of levitation sign
            vec3f levitation_vec3 = vec3_scale(vec3_normalize((vec3f){elem.direction.x, elem.direction.y, 50.f}), 50.0f);
            spell->position = vec3_sum(spell->position, levitation_vec3);
        }
    }
}

static void handle_convergence_signs(Spell* spell, SpellElements elems){

    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == CONVERGENCE_SIGN){
            spell->density += 20.0f;
            spell->convergence += 10.0f;
        }
    }
}

static void handle_collection_signs(Spell* spell, SpellElements elems){

}

static void handle_dispersion_signs(Spell* spell, SpellElements elems){

}

static void handle_region_signs(Spell* spell, SpellElements elems){

}


/*
Explanation of pull logic:
1.  It computes angular position around the ring center of all pull signs, 
    and stores them, so later the ring could be partitioned into sectors.
    It also calculates pull direction(works the same was as column).

2.  It computes the cosine between the pull direction and the vector
    pointing toward the ring center. It gives us how slanted pull sign is.
    If cosine is 1 or -1 then pull sign directs to the ring center, and global pull is max or -max and
    pull sign's twist force is 0;
    if cosine is 0 then pull sign is perpendicular, and global pull is 0 and
    pull sign's twist force is max.

spell->pull_direction is pull/push. spell->pull_sectors is twist vectors.

After it creates sectors from pull signs.

If particle inside the pull sector, that sector's twist vector is added to particle's velocity.
*/
static void handle_pull_signs(Spell* spell, SpellElements elems){

    Floats pull_point_angles = {0};
    Vectors3f pull_vecs = {0};

    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == PULL_SIGN){
            // pull force - (pull direction, pull magnitude)
            // pull spread
            vec2f origin_vec2 = vec2_normalize(MAGIC_RING_ORIGIN_VEC2(spell->ring));
            vec2f elem_pos2 = {elem.pos.x, elem.pos.y};
            vec2f center2 = {spell->ring.center.x, spell->ring.center.y};
            vec2f elem_vec = vec2_normalize(vec2_sub(elem_pos2, center2));
            float angle = vec2_get_angle_360(origin_vec2, elem_vec);
            da_append(pull_point_angles, angle);

            // Pull sign position and angle
            vec3f new_pull_vec3 = vec3_scale(vec3_normalize((vec3f){elem.direction.x, elem.direction.y, 50.0f}), elem.size);
            vec3f to_center = vec3_sub(spell->ring.center, elem.pos);
            float slanted = vec3_get_cos(new_pull_vec3, to_center);

            // Calc sector and global pull vectors
            float abs_slanted = slanted < 0.0f ? -slanted : slanted;
            vec3f sector_pull_vec3 = vec3_scale(new_pull_vec3, 1.0f - abs_slanted);
            sector_pull_vec3.z = 0.0f; // pull sign itself does not affect to z coordinate
            da_append(pull_vecs, sector_pull_vec3);

            spell->pull_direction = vec3_sum(spell->pull_direction, vec3_scale(new_pull_vec3, slanted));
        }
    }

    Sectors sectors = points_to_sectors(pull_point_angles.items, pull_point_angles.count);

    for(size_t i = 0; i < sectors.count; i++){
        MagicPullSector pull_sector = {
            .sector = sectors.items[i],
            .pull_vector = vec3_inv(pull_vecs.items[i])
        };  
        da_append(spell->pull_sectors, pull_sector);
    }
    
    free(sectors.items);
    free(pull_vecs.items);
    free(pull_point_angles.items);
}

static Spell get_result_spell(SpellElements elems, MagicRing ring){
    Spell res = {0};
    res.is_active = FALSE;
    res.ring = ring;
    res.position = ring.center;
    res.magic = UNKNOWN_MAGIC;
    res.spread = 1.0f;
    res.pull_sectors = (MagicPullSectors){0};

    float *pull_point_angles;
    int pull_point_count = 0;

    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(vec3_length(vec3_sub(ring.center, elem.pos)) >= ring.radius) continue;

        switch(elem.type){
            case FIRE_SIGIL:
                {
                    res.magic = FIRE_MAGIC;
                } break;
            case WATER_SIGIL:
                {
                    res.magic = WATER_MAGIC;
                } break;
            case WIND_SIGIL:
                {
                    res.magic = WIND_MAGIC;
                } break;
            case EARTH_SIGIL:
                {
                    res.magic = EARTH_MAGIC;
                } break;
            case LIGHT_SIGIL:
                {
                    res.magic = LIGHT_MAGIC;
                } break;
        }
    }
    handle_column_signs(&res, elems);
    handle_levitation_signs(&res, elems);
    handle_convergence_signs(&res, elems);
    handle_pull_signs(&res, elems);

    return res;
}


#define MAGIC_GEN_OFFSET 50.0f
#define MAGIC_GEN_COUNT 10000
static Magic create_magic(Spell spell){

    Magic magic = {0};
    magic.type = spell.magic;
    magic.pos = spell.position;
    magic.duration = 200.0f;
    magic.power = 100.0f;
    float gen_radius = spell.ring.radius == 0.0f ? MAGIC_GEN_OFFSET : spell.ring.radius/2.0f;
    for(int i = 0; i < MAGIC_GEN_COUNT; i++){
        vec3f part_pos = (vec3f){
            (randf(spell.position.x - gen_radius, spell.position.x + gen_radius)),
            (randf(spell.position.y - gen_radius, spell.position.y + gen_radius)),
            (randf(spell.position.z  - gen_radius, spell.position.z + gen_radius))
        };

        MagicParticle part = {
            .pos = part_pos,
            .velocity = {0.0f, 0.0f, 0.0f}
        };
        da_append(magic.parts, part);
    }

    return magic;
}

#endif