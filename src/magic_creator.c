#ifndef MAGIC_CREATOR_C_
#define MAGIC_CREATOR_C_

#include "game.h"


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
    vec3f pos;
    SpellElementType type;
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
    float start_angle;    
    float end_angle;
} MagicSector;

typedef struct{
    MagicSector sector;
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
    vec3f direction;
    vec3f position;
    float collect_scale;
    float convergence;
    float force_mag;
    float density;
    float spread;
    bool is_active;
    MagicPullSectors sectors;
} Spell;


typedef struct{
    vec3f* items;
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



#define add_type(elem_type, arr)\
do{\
    SpellElement elem = {\
        (vec3f){\
            .x = game->is.mouse.x -40,\
            .y = game->is.mouse.y -40\
        },\
        .type = (elem_type)\
    };\
    da_append(arr, elem);\
}while(0)


static Spell get_result_spell(SpellElements elems, MagicRing ring){
    Spell res = {0};
    res.is_active = FALSE;
    res.ring = ring;
    res.position = ring.center;
    res.magic = UNKNOWN_MAGIC;
    res.spread = 1.0f;
    res.sectors = (MagicPullSectors){0};

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

            case COLUMN_SIGN:
                {
                    vec3f current_force_vec3 = vec3_scale(res.direction, res.force_mag);
                    // column scalar is defined by the length of column sign
                    vec3f new_force_vec3 = vec3_scale(vec3_normalize(vec3_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f);
                    vec3f force_vec3 = vec3_sum(current_force_vec3, new_force_vec3);
                    res.force_mag = vec3_length(force_vec3);
                    res.direction = vec3_normalize(force_vec3);
                } break;
            case LEVITATION_SIGN:
                {
                    // levitation scalar is defined by the length of levitation sign
                    vec3f levitation_vec3 = vec3_scale(vec3_normalize(vec3_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f);
                    res.position = vec3_sum(res.position, levitation_vec3);
                } break;
            case CONVERGENCE_SIGN:
                {
                    res.density += 20.0f;
                    res.convergence += 10.0f;
                } break;
            case PULL_SIGN:
                {
                    // pull force - (pull direction, pull magnitude)
                    // pull spread
                    vec2f origin_vec2 = vec2_normalize(MAGIC_RING_ORIGIN_VEC2(ring));
                    vec2f elem_pos2 = {elem.pos.x, elem.pos.y};
                    vec2f center2 = {res.ring.center.x, res.ring.center.y};
                    vec2f elem_vec = vec2_normalize(vec2_sub(elem_pos2, center2));
                    float angle = vec2_get_angle_360(origin_vec2, elem_vec);

                    float offset = PI_CONST/4.0f;
                    float start_angle   = angle - offset;
                    float end_angle     = angle + offset;
                    MagicSector sector = {start_angle, end_angle};

                    MagicPullSector pull_sector = {
                        .sector = sector,
                        .pull_vector = {0.5f, 0.0f, 0.0f}
                    };
                    da_append(res.sectors, pull_sector);
                } break;
            case DISPERSION_SIGN:
                {

                } break;
            case REGION_SIGN:
                {

                } break;
            case COLLECTION_SIGN:
                {
                    res.collect_scale += 1.0f;
                } break;
            
        }
    }
    
    // printf("\n\n");

    float angle_sum = 0;
    int column_count = 0;
    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == COLUMN_SIGN){
            float angle = vec3_get_angle(res.direction, vec3_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f}));
            // if(angle < 0.0f) angle = -angle;
            angle_sum += angle;
            column_count++;
            // printf("angle: %f\n", vec3_get_angle(res.direction, vec3_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})));
        }
    }
    if(column_count != 0) res.spread = 0.3f*angle_sum/(float)column_count;

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
        vec3f part = (vec3f){
            (randf(spell.position.x - gen_radius, spell.position.x + gen_radius)),
            (randf(spell.position.y - gen_radius, spell.position.y + gen_radius)),
            (randf(spell.position.z  - gen_radius, spell.position.z + gen_radius))
        };
        // vec3f part = (vec3f){
        //     0.0f, 0.0f, 0.0f
        // };
        da_append(magic.parts, part);
    }

    return magic;
}

#endif