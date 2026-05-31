#include "particle_game.h"
#include "particle_generation.h"
#include "basic_text_renderer.h"
#include "gui_renderer_sw.h"
#include "gui_handler.h"
#include "input_system.h"
#include "mouse.h"
#include "renderer_sw.h"
#include "chunk_lighting.h"
#include "image_manipulation.h"
#include "entity.h"
#include "custom_parser.h"


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
    CONVERGENCE_SIGN
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

typedef struct{
    vec3f center;
    float radius;
} MagicRing;

typedef struct{
    MagicType magic;
    MagicRing ring;
    vec3f direction;
    vec3f position;
    float convergence;
    float force_mag;
    float density;
    bool is_active;
} Spell;


typedef struct{
    vec3f* items;
    size_t count;
    size_t capacity;
} MagicParticles;

typedef struct{
    vec3f pos;
    MagicParticles parts;
    float power;
    float duration;
} Magic;

Image fire_sigil_image;
Image water_sigil_image;
Image wind_sigil_image;
Image earth_sigil_image;
Image light_sigil_image;
Image column_sign_image;
Image levitation_sign_image;
Image convergence_sign_image;

void draw_element(ParticleGame* game, SpellElement elem){
    switch(elem.type){
        case FIRE_SIGIL:
            draw_image_on_fimage_scaled(game->win->context, fire_sigil_image, (int)elem.pos.x, (int)elem.pos.y, 4, 4);
            break;
        case WATER_SIGIL:
            draw_image_on_fimage_scaled(game->win->context, water_sigil_image, (int)elem.pos.x, (int)elem.pos.y, 4, 4);
            break;
        case WIND_SIGIL:
            draw_image_on_fimage_scaled(game->win->context, wind_sigil_image, (int)elem.pos.x, (int)elem.pos.y, 4, 4);
            break;
        case EARTH_SIGIL:
            draw_image_on_fimage_scaled(game->win->context, earth_sigil_image, (int)elem.pos.x, (int)elem.pos.y, 4, 4);
            break;
        case LIGHT_SIGIL:
            draw_image_on_fimage_scaled(game->win->context, light_sigil_image, (int)elem.pos.x, (int)elem.pos.y, 4, 4);
            break;
        case COLUMN_SIGN:
            draw_image_on_fimage_scaled(game->win->context, column_sign_image, (int)elem.pos.x, (int)elem.pos.y, 2, 2);
            break;
        case LEVITATION_SIGN:
            draw_image_on_fimage_scaled(game->win->context, levitation_sign_image, (int)elem.pos.x, (int)elem.pos.y, 2, 2);
            break;
        case CONVERGENCE_SIGN:
            draw_image_on_fimage_scaled(game->win->context, convergence_sign_image, (int)elem.pos.x, (int)elem.pos.y, 2, 2);
            break;
    }
}

void draw_all_elements(ParticleGame* game, SpellElements elems){
    for(size_t i = 0; i < elems.count; i++){
        draw_element(game, elems.items[i]);
    }
}

vec3f vector_sum(vec3f a, vec3f b){
    return (vec3f){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

vec3f vector_sub(vec3f a, vec3f b){
    return (vec3f){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

vec3f vector_scale(vec3f v, float scale){
    return (vec3f){
        v.x * scale,
        v.y * scale,
        v.z * scale
    };
}

float distance3f(vec3f v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}


vec3f vector_normalize(vec3f v){
    float len = distance3f(v);
    return (vec3f){
        v.x / len,
        v.y / len,
        v.z / len
    };
}


Spell get_result_spell(SpellElements elems, MagicRing ring){
    Spell res = {0};
    res.is_active = FALSE;
    res.ring = ring;
    res.position = ring.center;
    res.magic = UNKNOWN_MAGIC;

    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(distance3f(vector_sub(ring.center, elem.pos)) >= ring.radius) continue;
        
        if(elem.type == FIRE_SIGIL)         res.magic = FIRE_MAGIC;
        else if(elem.type == WATER_SIGIL)   res.magic = WATER_MAGIC;
        else if(elem.type == WIND_SIGIL)    res.magic = WIND_MAGIC;
        else if(elem.type == EARTH_SIGIL)   res.magic = EARTH_MAGIC;
        else if(elem.type == LIGHT_SIGIL)   res.magic = LIGHT_MAGIC;

        else if(elem.type == COLUMN_SIGN){
            vec3f force_vector = vector_sum(vector_scale(res.direction, res.force_mag), vector_scale(vector_normalize(vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f));
            res.force_mag = 100.0f + distance3f(force_vector);
            res.direction = vector_normalize(force_vector);
        }
        else if(elem.type == LEVITATION_SIGN){
            vec3f levitation_vector = vector_scale(vector_normalize(vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f);
            res.position = vector_sum(res.position, levitation_vector);
        }
        else if(elem.type == CONVERGENCE_SIGN){
            res.density += 5.0f;
            res.convergence += 1.0f;
        }
    }

    return res;
}

void render_spell_params(ParticleGame* game, Spell spell, Color color, int x, int y){
    char text[256];
    sprintf(text, "Spell:\ntype:%s\ndirection:[ x:%.2f y:%.2f z:%.2f ]\nposition:[ x:%.2f y:%.2f z:%.2f ]\nforce_mag:%.2f\ndensity:%.2f\nconvergence:%.2f", 
        spell.magic == 5 ? "LIGHT" : spell.magic == 4 ? "EARTH" : spell.magic == 3 ? "WIND" : spell.magic == 2 ? "WATER" : spell.magic == 1 ? "FIRE" : "UNKNOWN", 
        spell.direction.x, spell.direction.y, spell.direction.z, 
        spell.position.x, spell.position.y, spell.position.z,
        spell.force_mag, spell.density, spell.convergence);
    BasicTextRender(game->win, text, x, y, 2, color);
}

void render_mouse_pos(ParticleGame* game, Color color, int x, int y){
    char text[64];
    sprintf(text, "mouse: x:%.2f y:%.2f", game->is.mouse.x, game->is.mouse.y);
    BasicTextRender(game->win, text, x, y, 2, color);
}

float randf(float min, float max){
    return min + (max-min) * ((float)rand() / (float)RAND_MAX);
}

#define MAGIC_GEN_OFFSET 40.0f
#define MAGIC_GEN_COUNT 200
Magic spell_active(Spell spell){

    Magic magic = {0};
    magic.pos = spell.position;
    // magic.parts = (MagicParticles){0};
    for(int i = 0; i < MAGIC_GEN_COUNT; i++){
        vec3f part = (vec3f){
            (randf(spell.position.x - MAGIC_GEN_OFFSET, spell.position.x + MAGIC_GEN_OFFSET)),
            (randf(spell.position.y - MAGIC_GEN_OFFSET, spell.position.y + MAGIC_GEN_OFFSET)),
            0.0f
        };
        da_append(magic.parts, part);
    }

    return magic;
}

vec3f random_move(vec3f pos, float scale){
    return (vec3f){
        pos.x += randf(-1.0f*scale, 1.0f*scale),
        pos.y += randf(-1.0f*scale, 1.0f*scale),
        pos.z += randf(-1.0f*scale, 1.0f*scale)
    };
}

vec3f converge_to_center(vec3f v, vec3f c, float convergence, float scale){
    vec3f diff = vector_sub(c, v);
    float dist = distance3f(diff);
    float final_scale = scale * dist * convergence / 1000.0f;
    vec3f move = vector_scale(diff, final_scale);
    return vector_sum(v, move);
}

void magic_simulate(Spell spell, Magic magic){
    if(!spell.is_active) return;

    spell.density = spell.density == 0 ? 1.0f : spell.density;
    vec3f* part;
    da_foreach(part, magic.parts){
        *part = vector_sum(*part, vector_scale(vector_scale(spell.direction, spell.force_mag), get_global_delta()));
        *part = random_move(*part, 20.0f/spell.density);
        *part = converge_to_center(*part, magic.pos, spell.convergence, 0.01f);
    }
}


Color ring_color;
Color fire_color;
Color water_color;
Color wind_color;
Color earth_color;
Color light_color;
Color sign_color;
void magic_draw(ParticleGame* game, Spell spell, Magic magic){
    Color magic_color = (Color){.rgba = 0xFF000000};
    switch(spell.magic){
        case FIRE_MAGIC:
            magic_color = fire_color;
            break;
        case WATER_MAGIC:
            magic_color = water_color;
            break;
        case WIND_MAGIC:
            magic_color = wind_color;
            break;
        case EARTH_MAGIC:
            magic_color = earth_color;
            break;
        case LIGHT_MAGIC:
            magic_color = light_color;
            break;
    }

    for(size_t i = 0; i < magic.parts.count; i++){
        draw_filled_circle_f(game->win->context, (int)magic.parts.items[i].x, (int)magic.parts.items[i].y, 3, magic_color);
    }
}

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

void prepare_images();

int RunSpellGame(ParticleGame* game){
    
    SpellElements elems = {0};
    prepare_images();

    
    ring_color = (Color){.rgba = 0xFFFFFFFF};
    fire_color = (Color){.rgba = 0xFF0000FF};
    water_color = (Color){.rgba = 0xFFFF0000};
    wind_color = (Color){.rgba = 0xFFAAAAAA};
    earth_color = (Color){.rgba = 0xFF008888};
    light_color = (Color){.rgba = 0xFF00FFFF};
    sign_color = (Color){.rgba = 0xFFFFFFFF};


    Color result_color = (Color){.rgba = 0xFFFF0000};
    Color text_color = (Color){.rgba = 0xFFFFFFFF};

    action_t act_exit = 0;
    action_t act_create_ring = 1;
    action_t act_create_fire_sigil = 2;
    action_t act_create_water_sigil = 3;
    action_t act_create_wind_sigil = 4;
    action_t act_create_earth_sigil = 5;
    action_t act_create_light_sigil = 6;
    action_t act_create_column_sign = 7;
    action_t act_create_levitation_sign = 8;
    action_t act_create_convergence_sign = 9;
    action_t act_spell_activate = 10;
    add_binding(&game->is, BUTTON_ESCAPE, act_exit);
    add_binding(&game->is, BUTTON_MOUSE_LEFT, act_create_ring);
    add_binding(&game->is, BUTTON_Q, act_create_fire_sigil);
    add_binding(&game->is, BUTTON_W, act_create_water_sigil);
    add_binding(&game->is, BUTTON_E, act_create_wind_sigil);
    add_binding(&game->is, BUTTON_R, act_create_earth_sigil);
    add_binding(&game->is, BUTTON_A, act_create_light_sigil);
    add_binding(&game->is, BUTTON_S, act_create_column_sign);
    add_binding(&game->is, BUTTON_D, act_create_levitation_sign);
    add_binding(&game->is, BUTTON_F, act_create_convergence_sign);
    add_binding(&game->is, BUTTON_SPACE, act_spell_activate);
    

    Spell spell;
    Magic magic = {0};
    // MagicParticles magic_parts = {0};
    MagicRing ring = {0};
    bool ring_exists = FALSE;
    while(game->s_params.is_running){

        update_global_time();
        update_input_system(&game->is);
        
        if(action_pressed(&game->is, act_exit)){
            game->s_params.is_running = FALSE;
        }
        if(action_pressed(&game->is, act_create_ring)){
            if(!ring_exists){
                ring.center.x = game->is.mouse.x;
                ring.center.y = game->is.mouse.y;
                ring.radius = 300.0f;
                ring_exists = TRUE;
            }
        }
        if(action_pressed(&game->is, act_create_fire_sigil)){
            add_type(FIRE_SIGIL, elems);
        }
        if(action_pressed(&game->is, act_create_water_sigil)){
            add_type(WATER_SIGIL, elems);
        }
        if(action_pressed(&game->is, act_create_wind_sigil)){
            add_type(WIND_SIGIL, elems);
        }
        if(action_pressed(&game->is, act_create_earth_sigil)){
            add_type(EARTH_SIGIL, elems);
        }
        if(action_pressed(&game->is, act_create_light_sigil)){
            add_type(LIGHT_SIGIL, elems);
        }
        if(action_pressed(&game->is, act_create_column_sign)){
            add_type(COLUMN_SIGN, elems);
        }
        if(action_pressed(&game->is, act_create_levitation_sign)){
            add_type(LEVITATION_SIGN, elems);
        }
        if(action_pressed(&game->is, act_create_convergence_sign)){
            add_type(CONVERGENCE_SIGN, elems);
        }
        if(action_pressed(&game->is, act_spell_activate)){
            magic = spell_active(spell);
            spell.is_active = TRUE;
        }

        clear_game_window(game);
        if(ring_exists){
            draw_circle_f(game->win->context, ring.center.x, ring.center.y, ring.radius, ring_color, 2);
            put_pixel_f(game->win->context, ring.center.x, ring.center.y, ring_color);
        }
        
        
        if(spell.is_active){
            spell = get_result_spell(elems, ring);
            spell.is_active = TRUE;
        } else spell = get_result_spell(elems, ring);
        
        vec3f result_vector = vector_scale(spell.direction, spell.force_mag);
        draw_line_f(game->win->context, result_color, 
            (int)ring.center.x, (int)ring.center.y, 
            (int)(ring.center.x + result_vector.x),
            (int)(ring.center.y + result_vector.y)
        );
        render_spell_params(game, spell, text_color, 10, 10);
        render_mouse_pos(game, text_color, 10, game->win->h - 20);
    
        draw_all_elements(game, elems);

        magic_simulate(spell, magic);
        magic_draw(game, spell, magic);

        SDL_UpdateWindowSurface(game->win->window);
    }

    return 0;
}


void prepare_images(){
    Color white_color = (Color){.rgba = 0xFFFFFFFF};
    Color alpha_color = (Color){.rgba = 0x00FFFFFF};

    load_png(&fire_sigil_image, "resources/fire_sigil.png");
    load_png(&water_sigil_image, "resources/water_sigil.png");
    load_png(&wind_sigil_image, "resources/wind_sigil.png");
    load_png(&earth_sigil_image, "resources/earth_sigil.png");
    load_png(&light_sigil_image, "resources/light_sigil.png");
    load_png(&column_sign_image, "resources/column_sign.png");
    load_png(&levitation_sign_image, "resources/levitation_sign.png");
    load_png(&convergence_sign_image, "resources/convergence_sign.png");
    fire_sigil_image = minimize_resolution(fire_sigil_image, 10, 10);    
    water_sigil_image = minimize_resolution(water_sigil_image, 10, 10);    
    wind_sigil_image = minimize_resolution(wind_sigil_image, 10, 10);    
    earth_sigil_image = minimize_resolution(earth_sigil_image, 10, 10);    
    light_sigil_image = minimize_resolution(light_sigil_image, 10, 10);    
    column_sign_image = minimize_resolution(column_sign_image, 10, 10);    
    levitation_sign_image = minimize_resolution(levitation_sign_image, 10, 10);    
    convergence_sign_image = minimize_resolution(convergence_sign_image, 10, 10);    
    change_color(fire_sigil_image, white_color, alpha_color);
    change_color(water_sigil_image, white_color, alpha_color);
    change_color(wind_sigil_image, white_color, alpha_color);
    change_color(earth_sigil_image, white_color, alpha_color);
    change_color(light_sigil_image, white_color, alpha_color);
    change_color(column_sign_image, white_color, alpha_color);
    change_color(levitation_sign_image, white_color, alpha_color);
    change_color(convergence_sign_image, white_color, alpha_color);
}
