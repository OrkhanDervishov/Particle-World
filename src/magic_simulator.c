#ifndef MAGIC_SIMULATOR_H_
#define MAGIC_SIMULATOR_H_

#include "magic_creator.c"

// TODO: Optimize this function
static bool inside_sector(MagicRing ring, MagicSector sector, vec3f pos){

    vec2f center2 = {ring.center.x, ring.center.y};
    vec2f pos2 = {pos.x, pos.y};
    vec2f part_vec = vec2_sub(pos2, center2);

    float dist = vec2_length(part_vec);
    if(dist > ring.radius) return FALSE;

    vec2f origin_vec = MAGIC_RING_ORIGIN_VEC2(ring);
    float angle = vec2_get_angle_360(origin_vec, part_vec);

    if(sector.start_angle > sector.end_angle){
        if(angle <= 2*PI_CONST && angle >= sector.start_angle) return TRUE;
        if(angle >= 0 && angle <= sector.end_angle) return TRUE;
    }
    else{
        if(angle >= sector.start_angle && angle <= sector.end_angle) return TRUE;
    }

    return FALSE;
}

static vec3f sum_pull_vectors(Spell spell, vec3f part){
    vec3f sum = {0.0f, 0.0f, 0.0f};
    for(size_t i = 0; i < spell.sectors.count; i++){
        if(inside_sector(spell.ring, spell.sectors.items[i].sector, part)){
            sum = vec3_sum(sum, spell.sectors.items[i].pull_vector);
        }
    }
    return sum;
}

static vec3f random_move(vec3f pos, float scale){
    return (vec3f){
        pos.x += randf(-1.0f*scale, 1.0f*scale),
        pos.y += randf(-1.0f*scale, 1.0f*scale),
        pos.z += randf(-1.0f*scale, 1.0f*scale)
    };
}

static vec3f move_to_center(vec3f v, vec3f c, float scale){
    vec3f diff = vec3_sub(c, v);
    // float dist = vec3_length(diff);
    float final_scale = scale * 0.000005f;
    vec3f move = vec3_scale(diff, final_scale);
    return vec3_sum(v, move);
}

static vec3f spread_vec3(vec3f part, vec3f direction, vec3f pos, float spread)
{
    vec3f diff = vec3_normalize(vec3_sub(part, pos));
    // if(vec3_dot(diff, direction) < 0.0f) diff = vec3_inv(diff);

    float t = spread;
    if(t > 1.0f) t = 1.0f;
    if(t < 0.0f) t = 0.0f;

    vec3f result =
        vec3_sum(
            vec3_scale(direction, 1.0f - t),
            vec3_scale(diff, t)
        );

    return vec3_normalize(result);
}


static float convergence_scale(vec3f v, vec3f c, float convergence, float scale){
    vec3f diff = vec3_sub(c, v);
    float dist = vec3_length(diff);
    if(dist <= 0.0f) return 1.0f;
    return 10000.0f/(dist*dist*convergence);
}

static void magic_simulate(ParticleGame* game, Spell spell, Magic magic){
    if(!spell.is_active) return;

    spell.density = spell.density == 0 ? 1.0f : spell.density;
    float converge_scale = 1.0f;
    float avg_speed = 0.0f;
    float avg_cs = 0.0f;
    vec3f* part;
    da_foreach(part, magic.parts){
        if(spell.convergence != 0.0f){
            converge_scale = convergence_scale(*part, magic.pos, spell.convergence, 1.0f);
        }
        float speed = converge_scale*spell.force_mag;
        avg_speed += speed;
        avg_cs += converge_scale;

        vec3f move_dir = spread_vec3(*part, spell.direction, magic.pos, spell.spread);
        *part = vec3_sum(*part, vec3_scale(move_dir, speed*get_global_delta()));
        *part = random_move(*part, (converge_scale)*spell.spread*spell.density/20.0f);
        *part = move_to_center(*part, magic.pos, spell.collect_scale);
        
        *part = vec3_sum(*part, sum_pull_vectors(spell, *part));
        //*part = vec3_sum(*part, vec3_scale(spell.direction, speed*get_global_delta()));
    }
    // render_parameter(game, "avg_speed", avg_speed/(float)magic.parts.count, (Color){.rgba=0xFFFFFFFF}, 10, 250);
    // render_parameter(game, "avg_convergence_scale", avg_cs/(float)magic.parts.count, (Color){.rgba=0xFFFFFFFF}, 10, 270);
}


#endif