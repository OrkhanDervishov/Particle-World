#ifndef MAGIC_RENDERER_C_
#define MAGIC_RENDERER_C_

#include "magic_creator.c"
// #include "magic_simulator.c"

// 2d renderer

// Loading element images
/*****************************************************/
typedef struct{
    const char** items;
    size_t count;
    size_t capacity;
} Paths;

typedef struct{
    Image* items;
    size_t count;
    size_t capacity;
} ElemImages;

static Paths elem_image_paths;
static ElemImages elem_images;

#define GET_ELEM_IMAGE(elem_index) elem_images.items[(elem_index)]

static void load_image_paths(){
    elem_image_paths = (Paths){0};

    // sigils
    da_append(elem_image_paths, "resources/fire_sigil.png");
    da_append(elem_image_paths, "resources/water_sigil.png");
    da_append(elem_image_paths, "resources/wind_sigil.png");
    da_append(elem_image_paths, "resources/earth_sigil.png");
    da_append(elem_image_paths, "resources/light_sigil.png");

    // signs
    da_append(elem_image_paths, "resources/column_sign.png");
    da_append(elem_image_paths, "resources/levitation_sign.png");
    da_append(elem_image_paths, "resources/convergence_sign.png");
    da_append(elem_image_paths, "resources/pull_sign.png");
    da_append(elem_image_paths, "resources/dispersion_sign.png");
    da_append(elem_image_paths, "resources/region_sign.png");
    da_append(elem_image_paths, "resources/collection_sign.png");

    da_append(elem_image_paths, "resources/billow_sign.png");
    da_append(elem_image_paths, "resources/repetition_sign.png");
    da_append(elem_image_paths, "resources/weave_sign.png");
    da_append(elem_image_paths, "resources/float_sign.png");
    da_append(elem_image_paths, "resources/cool_sign.png");

    for(size_t i = 0; i < elem_image_paths.count; i++){
        printf("%s\n", elem_image_paths.items[i]);
    }
}

static void prepare_images(){
    Color white_color = (Color){.rgba = 0xFFFFFFFF};
    Color black_color = (Color){.rgba = 0xFF000000};
    Color alpha_color = (Color){.rgba = 0x00FFFFFF};
    
    elem_images = (ElemImages){0};
    for(size_t i = 0; i < elem_image_paths.count; i++){
        Image image = {0};
        load_png(&image, elem_image_paths.items[i]);
        change_color(image, white_color, alpha_color);
        change_color(image, black_color, white_color);
        da_append(elem_images, image);
    }
}

/*****************************************************/

static void render_parameter(ParticleGame* game, const char* param_name, float value, Color color, int x, int y){
    char text[64];
    sprintf(text, "%s: %.4f", param_name, value);
    BasicTextRender(game->win, text, x, y, 2, color);
}

static void render_vec3(ParticleGame* game, const char* param_name, vec3f value, Color color, int x, int y){
    char text[64];
    sprintf(text, "%s: [ x:%.2f y:%.2f z:%.2f ]", param_name, value.x, value.y, value.z);
    BasicTextRender(game->win, text, x, y, 2, color);
}

static void render_vec2(ParticleGame* game, const char* param_name, vec2f value, Color color, int x, int y){
    char text[64];
    sprintf(text, "%s: [ x:%.2f y:%.2f ]", param_name, value.x, value.y);
    BasicTextRender(game->win, text, x, y, 2, color);
}

static void draw_sector(ParticleGame* game, MagicRing ring, Sector sector, Color color){

    vec2f origin_vec = MAGIC_RING_ORIGIN_VEC2(ring);
    // origin_vec.x -= ring.center.x;
    // origin_vec.y -= ring.center.y;
    
    vec2f start = vec2_rotate(origin_vec, sector.start_angle);
    vec2f end = vec2_rotate(origin_vec, sector.end_angle);
    vec2f image_start = start;
    vec2f image_end = end;

    image_start.x += ring.center.x;
    image_start.y += ring.center.y;
    image_end.x   += ring.center.x;
    image_end.y   += ring.center.y;
    
    draw_line_f(game->win->context, color, ring.center.x, ring.center.y, image_start.x, image_start.y);
    draw_line_f(game->win->context, color, ring.center.x, ring.center.y, image_end.x, image_end.y);
}

static void draw_sectors(ParticleGame* game, Spell spell, Color color){
    for(size_t i = 0; i < spell.pull_sectors.count; i++){
        draw_sector(game, spell.ring, spell.pull_sectors.items[i].sector, color);
    }
}

static void draw_element(ParticleGame* game, SpellElement elem){
    vec2f y_axis = {0.0f, -1.0f};
    float angle = vec2_get_angle_360(y_axis, elem.direction);
    // float angle = vec2_get_angle_360(elem.direction, y_axis);
    draw_rotated_image_on_fimage(
        game->win->context, 
        GET_ELEM_IMAGE(elem.type), 
        (vec2f){(float)elem.pos.x, (float)elem.pos.y}, 
        angle, 
        (vec2f){0.5f, 0.5f}
    );
}

static void draw_all_elements(ParticleGame* game, SpellElements elems){
    for(size_t i = 0; i < elems.count; i++){
        draw_element(game, elems.items[i]);
    }
}


static Color ring_color;
static Color fire_color;
static Color water_color;
static Color wind_color;
static Color earth_color;
static Color light_color;
static Color sign_color;
static void magic_draw(ParticleGame* game, Spell spell, Magic magic){
    Color magic_color = (Color){.rgba = 0xFF000000};
    switch(magic.type){
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
        draw_filled_circle_f(game->win->context, (int)magic.parts.items[i].pos.x, (int)magic.parts.items[i].pos.y, 3, magic_color);
    }
}

// Text rendering
static void render_spell_params(ParticleGame* game, Spell spell, Color color, int x, int y){
    char text[256];
    sprintf(text, "Spell:\ntype:%s\ndirection:[ x:%.2f y:%.2f z:%.2f ]\nposition:[ x:%.2f y:%.2f z:%.2f ]\nforce_mag:%.2f\ndensity:%.2f\nconvergence:%.2f\nspread:%.2f", 
        spell.magic == 5 ? "LIGHT" : spell.magic == 4 ? "EARTH" : spell.magic == 3 ? "WIND" : spell.magic == 2 ? "WATER" : spell.magic == 1 ? "FIRE" : "UNKNOWN", 
        spell.direction.x, spell.direction.y, spell.direction.z, 
        spell.position.x, spell.position.y, spell.position.z,
        spell.force_mag, spell.density, spell.convergence, spell.spread);
    BasicTextRender(game->win, text, x, y, 2, color);
}

static void render_magic_params(ParticleGame* game, Magic magic, Color color, int x, int y){
    char text[256];
    sprintf(text, "Magic:\ntype:%s\nposition:[ x:%.2f y:%.2f z:%.2f ]\npower:%.2f\nduration:%.2f", 
        magic.type == 5 ? "LIGHT" : magic.type == 4 ? "EARTH" : magic.type == 3 ? "WIND" : magic.type == 2 ? "WATER" : magic.type == 1 ? "FIRE" : "UNKNOWN", 
        magic.pos.x, magic.pos.y, magic.pos.z,
        magic.power, magic.duration);
    BasicTextRender(game->win, text, x, y, 2, color);
}

static void render_mouse_pos(ParticleGame* game, Color color, int x, int y){
    char text[64];
    sprintf(text, "mouse: x:%.2f y:%.2f", game->is.mouse.x, game->is.mouse.y);
    BasicTextRender(game->win, text, x, y, 2, color);
}

/*******************************************************/
/*******************************************************/

// 3d renderer

static void render3d_points(MagicParticles parts){
    
}

#endif