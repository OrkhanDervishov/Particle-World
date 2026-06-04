#include "particle_game.h"
#include "basic_text_renderer.h"
#include "gui_renderer_sw.h"
#include "gui_handler.h"
#include "input_system.h"
#include "renderer_sw.h"
#include "image_manipulation.h"
#include "entity.h"
#include "custom_parser.h"
#include "game.h"
#include "renderer_3d.h"


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
    float spread;
    bool is_active;
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
    float power;
    float duration;
} Magic;


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

Paths elem_image_paths;
ElemImages elem_images;

#define GET_ELEM_IMAGE(elem_index) elem_images.items[(elem_index)]

void load_image_paths(){
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

void prepare_images(){
    Color white_color = (Color){.rgba = 0xFFFFFFFF};
    Color alpha_color = (Color){.rgba = 0x00FFFFFF};
    
    elem_images = (ElemImages){0};
    for(size_t i = 0; i < elem_image_paths.count; i++){
        Image image = {0};
        Image min_image;
        load_png(&image, elem_image_paths.items[i]);
        min_image = minimize_resolution(image, 10, 10);
        change_color(min_image, white_color, alpha_color);
        da_append(elem_images, min_image);
        delete_image(&image);
    }
}

/*****************************************************/

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


// Drawing
void draw_element(ParticleGame* game, SpellElement elem){
    draw_image_on_fimage_scaled(game->win->context, GET_ELEM_IMAGE(elem.type), (int)elem.pos.x, (int)elem.pos.y, 4, 4);
}

void draw_all_elements(ParticleGame* game, SpellElements elems){
    for(size_t i = 0; i < elems.count; i++){
        draw_element(game, elems.items[i]);
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
        draw_filled_circle_f(game->win->context, (int)magic.parts.items[i].x, (int)magic.parts.items[i].y, 3, magic_color);
    }
}

// Text rendering
void render_spell_params(ParticleGame* game, Spell spell, Color color, int x, int y){
    char text[256];
    sprintf(text, "Spell:\ntype:%s\ndirection:[ x:%.2f y:%.2f z:%.2f ]\nposition:[ x:%.2f y:%.2f z:%.2f ]\nforce_mag:%.2f\ndensity:%.2f\nconvergence:%.2f\nspread:%.2f", 
        spell.magic == 5 ? "LIGHT" : spell.magic == 4 ? "EARTH" : spell.magic == 3 ? "WIND" : spell.magic == 2 ? "WATER" : spell.magic == 1 ? "FIRE" : "UNKNOWN", 
        spell.direction.x, spell.direction.y, spell.direction.z, 
        spell.position.x, spell.position.y, spell.position.z,
        spell.force_mag, spell.density, spell.convergence, spell.spread);
    BasicTextRender(game->win, text, x, y, 2, color);
}

void render_magic_params(ParticleGame* game, Magic magic, Color color, int x, int y){
    char text[256];
    sprintf(text, "Magic:\ntype:%s\nposition:[ x:%.2f y:%.2f z:%.2f ]\npower:%.2f\nduration:%.2f", 
        magic.type == 5 ? "LIGHT" : magic.type == 4 ? "EARTH" : magic.type == 3 ? "WIND" : magic.type == 2 ? "WATER" : magic.type == 1 ? "FIRE" : "UNKNOWN", 
        magic.pos.x, magic.pos.y, magic.pos.z,
        magic.power, magic.duration);
    BasicTextRender(game->win, text, x, y, 2, color);
}

void render_mouse_pos(ParticleGame* game, Color color, int x, int y){
    char text[64];
    sprintf(text, "mouse: x:%.2f y:%.2f", game->is.mouse.x, game->is.mouse.y);
    BasicTextRender(game->win, text, x, y, 2, color);
}

void render_parameter(ParticleGame* game, const char* param_name, float value, Color color, int x, int y){
    char text[64];
    sprintf(text, "%s: %.4f", param_name, value);
    BasicTextRender(game->win, text, x, y, 2, color);
}

void render_vector3(ParticleGame* game, const char* param_name, vec3f value, Color color, int x, int y){
    char text[64];
    sprintf(text, "%s: [ x:%.2f y:%.2f z:%.2f ]", param_name, value);
    BasicTextRender(game->win, text, x, y, 2, color);
}


// Mechanics
Spell get_result_spell(SpellElements elems, MagicRing ring){
    Spell res = {0};
    res.is_active = FALSE;
    res.ring = ring;
    res.position = ring.center;
    res.magic = UNKNOWN_MAGIC;
    res.spread = 1.0f;

    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(distance3f(vector_sub(ring.center, elem.pos)) >= ring.radius) continue;
        
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
                    vec3f current_force_vector = vector_scale(res.direction, res.force_mag);
                    // column scalar is defined by the length of column sign
                    vec3f new_force_vector = vector_scale(vector_normalize(vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f);
                    vec3f force_vector = vector_sum(current_force_vector, new_force_vector);
                    res.force_mag = distance3f(force_vector);
                    res.direction = vector_normalize(force_vector);
                } break;
            case LEVITATION_SIGN:
                {
                    // levitation scalar is defined by the length of levitation sign
                    vec3f levitation_vector = vector_scale(vector_normalize(vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})), 50.0f);
                    res.position = vector_sum(res.position, levitation_vector);
                } break;
            case CONVERGENCE_SIGN:
                {
                    res.density += 20.0f;
                    res.convergence += 10.0f;
                } break;
        }
    }

    float angle_sum = 0;
    int column_count = 0;
    for(size_t i = 0; i < elems.count; i++){
        SpellElement elem = elems.items[i];
        if(elem.type == COLUMN_SIGN){
            float angle = get_angle(res.direction, vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f}));
            // if(angle < 0.0f) angle = -angle;
            angle_sum += angle;
            column_count++;
            // printf("angle: %f\n", get_angle(res.direction, vector_sub(ring.center, (vec3f){elem.pos.x, elem.pos.y, -20.0f})));
        }
    }
    if(column_count != 0) res.spread = angle_sum/(float)column_count;

    return res;
}


#define MAGIC_GEN_OFFSET 80.0f
#define MAGIC_GEN_COUNT 500
Magic create_magic(Spell spell){

    Magic magic = {0};
    magic.type = spell.magic;
    magic.pos = spell.position;
    magic.duration = 200.0f;
    magic.power = 100.0f;
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

vec3f spread_vector(vec3f part, vec3f direction, vec3f pos, float spread)
{
    vec3f diff = vector_normalize(vector_sub(part, pos));
    // if(vector_dot(diff, direction) < 0.0f) diff = vector_inv(diff);

    float t = spread;
    if(t > 1.0f) t = 1.0f;
    if(t < 0.0f) t = 0.0f;

    vec3f result =
        vector_sum(
            vector_scale(direction, 1.0f - t),
            vector_scale(diff, t)
        );

    return vector_normalize(result);
}


float convergence_scale(vec3f v, vec3f c, float convergence, float scale){
    vec3f diff = vector_sub(c, v);
    float dist = distance3f(diff);
    return 10000.0f/(dist*dist*convergence);
}

void magic_simulate(ParticleGame* game, Spell spell, Magic magic){
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

        vec3f move_dir = spread_vector(*part, spell.direction, magic.pos, spell.spread);
        // *part = vector_sum(*part, vector_scale(spell.direction, speed*get_global_delta()));
        *part = vector_sum(*part, vector_scale(move_dir, speed*get_global_delta()));
        *part = random_move(*part, (converge_scale)*spell.spread*spell.density/20.0f);
        // *part = converge_to_center(*part, magic.pos, spell.convergence, 0.0001f);
    }
    render_parameter(game, "avg_speed", avg_speed/(float)magic.parts.count, (Color){.rgba=0xFFFFFFFF}, 10, 250);
    render_parameter(game, "avg_convergence_scale", avg_cs/(float)magic.parts.count, (Color){.rgba=0xFFFFFFFF}, 10, 270);
}


int RunSpellGame(ParticleGame* game){
    
    SpellElements elems = {0};
    load_image_paths();
    prepare_images();

    // Initializing 3d simulator
    /***************************************/
    init_opengl(game->win);
    get_gl_info();
    GL_Shader vertex_shader = load_gl_shader(GL_VERTEX_SHADER, "shaders/vertex_shader.vert");
    GL_Shader fragment_shader = load_gl_shader(GL_FRAGMENT_SHADER, "shaders/fragment_shader.frag");
    GL_Program program = create_gl_program(vertex_shader, fragment_shader);
    glUseProgram(program.program);
    Model model = create_model();
    Camera3d camera = create_camera((vec3f){0.0f, 0.0f, 1.0f});
    Object3d obj1 = create_object3d((vec3f){0.0f, 0.0f, -10.0f}, model);

    // Add uniforms
    program_add_uniform(&program, "view");
    program_add_uniform(&program, "perspective");
    program_add_uniform(&program, "model");


    /***************************************/
    
    
    // Initializing 2d simulator
    /***************************************/
    ring_color = (Color){.rgba = 0xFFFFFFFF};
    fire_color = (Color){.rgba = 0xFF0000FF};
    water_color = (Color){.rgba = 0xFFFF0000};
    wind_color = (Color){.rgba = 0xFFAAAAAA};
    earth_color = (Color){.rgba = 0xFF008888};
    light_color = (Color){.rgba = 0xFF00FFFF};
    sign_color = (Color){.rgba = 0xFFFFFFFF};
    
    Color direct_line_color = (Color){.rgba = 0xFFFF0000};
    Color Magic_pos_color = (Color){.rgba = 0xFFFF0000};
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
    action_t act_magic_draw_mode_toggle = 11;
    action_t act_camera_forward = 12;
    action_t act_camera_back = 13;
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
    add_binding(&game->is, BUTTON_TAB, act_magic_draw_mode_toggle);
    add_binding(&game->is, BUTTON_ARROW_UP, act_camera_forward);
    add_binding(&game->is, BUTTON_ARROW_DOWN, act_camera_back);
    /***************************************/
    
    bool magic_draw_mode = FALSE;

    Spell spell;
    Magic magic = {0};
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
            magic = create_magic(spell);
            spell.is_active = TRUE;
        }
        if(action_pressed(&game->is, act_magic_draw_mode_toggle)){
            magic_draw_mode = magic_draw_mode ? FALSE : TRUE;
        }
        // Camera movement
        if(action_down(&game->is, act_camera_forward)){
            camera.pos.z -= 0.01f;
        }
        if(action_down(&game->is, act_camera_back)){
            camera.pos.z += 0.01f;
        }

        if(spell.is_active){
            spell = get_result_spell(elems, ring);
            spell.is_active = TRUE;
        } else spell = get_result_spell(elems, ring);

        magic_simulate(game, spell, magic);

        // Hardware rendering
        if(!magic_draw_mode){
            prepare_draw(game->win, color_to_colorf(game->s_params.clear_color));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(program.program);
            
            program_set_uniform_mat4f(&program, "model", obj1.model_matrix); 
            program_set_uniform_mat4f(&program, "view", camera_view(camera));
            program_set_uniform_mat4f(&program, "perspective", camera.perspective);
            draw_model(model);
            SDL_GL_SwapWindow(game->win->window);
            // return 0;
        }
        else{
            clear_game_window(game);
            if(ring_exists){
                draw_circle_f(game->win->context, ring.center.x, ring.center.y, ring.radius, ring_color, 2);
                put_pixel_f(game->win->context, ring.center.x, ring.center.y, ring_color);
            }
            
            
            // Magic direction line
            vec3f result_vector = vector_scale(spell.direction, spell.force_mag);
            draw_line_f(game->win->context, direct_line_color, 
                (int)ring.center.x, (int)ring.center.y, 
                (int)(ring.center.x + result_vector.x),
                (int)(ring.center.y + result_vector.y)
            );
            //Magic source position
            draw_filled_rect_f(game->win->context, 
                (Rect){
                    (int)spell.position.x,
                    (int)spell.position.y,
                    4, 4
                }, Magic_pos_color
            );

            render_spell_params(game, spell, text_color, 10, 10);
            render_magic_params(game, magic, text_color, 10, 150);
            render_mouse_pos(game, text_color, 10, game->win->h - 20);
            
            
            //Software rendering
            draw_all_elements(game, elems);
            magic_draw(game, spell, magic);
            SDL_UpdateWindowSurface(game->win->window);
        }
    }

    return 0;
}
