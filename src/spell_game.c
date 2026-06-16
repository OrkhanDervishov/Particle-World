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

#include "magic_creator.c"
#include "magic_simulator.c"
#include "magic_renderer.c"


void translate_magic(Magic* dest, Magic src, vec3f translate){
    if(dest->parts.capacity < src.parts.capacity){
        da_reserve(dest->parts, src.parts.capacity);
        dest->parts.count = src.parts.count;
    }

    for(size_t i = 0; i < src.parts.count; i++){
        dest->parts.items[i].pos = vec3_scale(vec3_sub(src.parts.items[i].pos, translate), 0.01f);
    }
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

    GL_Shader magic_vertex_shader = load_gl_shader(GL_VERTEX_SHADER, "shaders/magic.vert");
    GL_Shader magic_fragment_shader = load_gl_shader(GL_FRAGMENT_SHADER, "shaders/magic.frag");
    GL_Program magic_program = create_gl_program(magic_vertex_shader, magic_fragment_shader);

    Model model = create_model();
    Object3d obj1 = create_object3d((vec3f){0.0f, 0.0f, -10.0f}, model);
    Particles3d parts3d = create_particles(NULL, 0);
    
    Camera3d camera = create_camera((vec3f){0.0f, 0.0f, 1.0f});
    camera_update_vectors(&camera);

    
    // Add uniforms
    glUseProgram(program.program);
    program_add_uniform(&program, "view");
    program_add_uniform(&program, "perspective");
    program_add_uniform(&program, "model");

    glUseProgram(magic_program.program);
    program_add_uniform(&magic_program, "view");
    program_add_uniform(&magic_program, "perspective");
    program_add_uniform(&magic_program, "point_size");
    program_add_uniform(&magic_program, "camera_pos");
    program_add_uniform(&magic_program, "part_color");


    prepare_draw(game->win, color_to_colorf(game->s_params.clear_color));
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
    Color sector_line_color = (Color){.rgba = 0xFF0000FF};
    Color Magic_pos_color = (Color){.rgba = 0xFFFF0000};
    Color text_color = (Color){.rgba = 0xFFFFFFFF};
    
    action_t act_exit                       = 0;
    action_t act_create_ring                = 1;
    action_t act_create_fire_sigil          = 2;
    action_t act_create_water_sigil         = 3;
    action_t act_create_wind_sigil          = 4;
    action_t act_create_earth_sigil         = 5;
    action_t act_create_light_sigil         = 6;
    action_t act_create_column_sign         = 7;
    action_t act_create_levitation_sign     = 8;
    action_t act_create_convergence_sign    = 9;
    action_t act_spell_activate             = 10;
    action_t act_magic_draw_mode_toggle     = 11;
    action_t act_magic_erase                = 12;
    action_t act_create_collect_sign        = 13;
    action_t act_create_pull_sign           = 14;
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
    add_binding(&game->is, BUTTON_Z, act_create_collect_sign);
    add_binding(&game->is, BUTTON_X, act_create_pull_sign);

    add_binding(&game->is, BUTTON_LALT, act_spell_activate);
    add_binding(&game->is, BUTTON_TAB, act_magic_draw_mode_toggle);
    add_binding(&game->is, BUTTON_BACKSPACE, act_magic_erase);

    action_t act_camera_forward = 101;
    action_t act_camera_back    = 102;
    action_t act_camera_left    = 103;
    action_t act_camera_right   = 104;
    action_t act_camera_up      = 105;
    action_t act_camera_down    = 106;
    add_binding(&game->is, BUTTON_W, act_camera_forward);
    add_binding(&game->is, BUTTON_S, act_camera_back);
    add_binding(&game->is, BUTTON_A, act_camera_left);
    add_binding(&game->is, BUTTON_D, act_camera_right);
    add_binding(&game->is, BUTTON_SPACE, act_camera_up);
    add_binding(&game->is, BUTTON_LSHIFT, act_camera_down);
    /***************************************/
    
    bool magic_draw_mode = FALSE;
    if(magic_draw_mode){
        SDL_ShowCursor(SDL_ENABLE);
        SDL_SetRelativeMouseMode(SDL_DISABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRelativeMouseMode(SDL_ENABLE);
    }

    vec2f direct = {200.0f, 0.0f};


    Spell spell;
    Magic magic = {0};
    Magic magic_origin = {0};
    MagicRing ring = {0};
    bool ring_exists = FALSE;
    spell = get_result_spell(elems, ring);
    while(game->s_params.is_running){

        update_global_time();
        update_input_system(&game->is);
        
        if(action_pressed(&game->is, act_exit)){
            game->s_params.is_running = FALSE;
        }
        if(action_pressed(&game->is, act_magic_draw_mode_toggle)){
            magic_draw_mode = magic_draw_mode ? FALSE : TRUE;
            if(magic_draw_mode){
                SDL_ShowCursor(SDL_ENABLE);
                SDL_SetRelativeMouseMode(SDL_DISABLE);
            }
            else{
                SDL_ShowCursor(SDL_DISABLE);
                SDL_SetRelativeMouseMode(SDL_ENABLE);
            }
        }
        if(action_pressed(&game->is, act_spell_activate)){
            free(magic.parts.items);
            magic = create_magic(spell);
            spell.is_active = TRUE;

            // Vectors3f part_positions = {0};
            // for(size_t i = 0; i < magic_origin.parts.count; i++){
            //     da_append(part_positions, magic_origin.parts.items[i].pos);
            // }

            // translate_magic(&magic_origin, magic, magic.pos);
            // update_particles(parts3d, part_positions.items, part_positions.count);
        }
        if(action_pressed(&game->is, act_magic_erase)){
            free(elems.items);
            elems.count = 0;
        }

        if(magic_draw_mode){
            if(action_pressed(&game->is, act_create_ring)){
                if(!ring_exists){
                    ring.center.x = game->is.mouse.x;
                    ring.center.y = game->is.mouse.y;
                    ring.radius = 300.0f;
                    ring_exists = TRUE;
                }
            }
            vec2f mouse_pos = (vec2f){(float)game->is.mouse.x, (float)game->is.mouse.y};
            if(action_pressed(&game->is, act_create_fire_sigil)){
                add_type(FIRE_SIGIL, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_water_sigil)){
                add_type(WATER_SIGIL, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_wind_sigil)){
                add_type(WIND_SIGIL, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_earth_sigil)){
                add_type(EARTH_SIGIL, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_light_sigil)){
                add_type(LIGHT_SIGIL, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_column_sign)){
                add_type(COLUMN_SIGN, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_levitation_sign)){
                add_type(LEVITATION_SIGN, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_convergence_sign)){
                add_type(CONVERGENCE_SIGN, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_collect_sign)){
                add_type(COLLECTION_SIGN, mouse_pos, direct, 1.0f, elems);
            }
            if(action_pressed(&game->is, act_create_pull_sign)){
                add_type(PULL_SIGN, mouse_pos, direct, 1.0f, elems);
            }
        }   
        // Camera movement
        else{
            float speed_scale = 6.0f;
            if(action_down(&game->is, act_camera_forward)){
                camera.pos = vec3_sum(camera.pos, vec3_scale(camera.forward, speed_scale * get_global_delta()));
            }
            if(action_down(&game->is, act_camera_back)){
                camera.pos = vec3_sub(camera.pos, vec3_scale(camera.forward, speed_scale * get_global_delta()));
            }
            if(action_down(&game->is, act_camera_left)){
                camera.pos = vec3_sub(camera.pos, vec3_scale(camera.right, speed_scale * get_global_delta()));
            }
            if(action_down(&game->is, act_camera_right)){
                camera.pos = vec3_sum(camera.pos, vec3_scale(camera.right, speed_scale * get_global_delta()));
            }
            if(action_down(&game->is, act_camera_up)){
                camera.pos.y += 0.01f;
            }
            if(action_down(&game->is, act_camera_down)){
                camera.pos.y -= 0.01f;
            }
            camera_look(&camera, game->is.mouse.xrel, game->is.mouse.yrel);
            camera_update_vectors(&camera);
        }



        /***************************************/

        if(button_pressed(&game->is, BUTTON_1)){
            if(spell.is_active){
                spell = get_result_spell(elems, ring);
                spell.is_active = TRUE;
            } else spell = get_result_spell(elems, ring);
        }

        magic_simulate(game, spell, magic);

        // Hardware rendering
        if(!magic_draw_mode){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Render particles
            glEnable(GL_PROGRAM_POINT_SIZE);
            glUseProgram(magic_program.program);
            program_set_uniform_mat4f(&magic_program, "view", camera_view(camera));
            program_set_uniform_mat4f(&magic_program, "perspective", camera.perspective);
            program_set_uniform_float(&magic_program, "point_size", 10.0f);
            program_set_uniform_vec3f(&magic_program, "camera_pos", camera.pos);
            program_set_uniform_vec3f(&magic_program, "part_color", (vec3f){1.0f, 1.0f, 0.0f});

            if(spell.is_active){

                Vectors3f part_positions = {0};
                for(size_t i = 0; i < magic_origin.parts.count; i++){
                    da_append(part_positions, magic_origin.parts.items[i].pos);
                }
                // render3d_points(magic.parts);
                translate_magic(&magic_origin, magic, magic.pos);
                update_particles(parts3d, part_positions.items, part_positions.count);
                render_particles(parts3d, part_positions.count); 
            }

            // Render objects
            glUseProgram(program.program);
            program_set_uniform_mat4f(&program, "model", obj1.model_matrix); 
            program_set_uniform_mat4f(&program, "view", camera_view(camera));
            program_set_uniform_mat4f(&program, "perspective", camera.perspective);
            draw_model(model);



            SDL_GL_SwapWindow(game->win->window);
        }
        else{
            clear_game_window(game);
            if(ring_exists){
                draw_circle_f(game->win->context, ring.center.x, ring.center.y, ring.radius, ring_color, 2);
                put_pixel_f(game->win->context, ring.center.x, ring.center.y, ring_color);
            }
            
            
            
            // Magic direction line
            vec3f result_vec3 = vec3_scale(spell.direction, spell.force_mag);
            draw_line_f(game->win->context, direct_line_color, 
                (int)ring.center.x, (int)ring.center.y, 
                (int)(ring.center.x + result_vec3.x),
                (int)(ring.center.y + result_vec3.y)
            );
            //Magic source position
            draw_filled_rect_f(game->win->context,
                (Rect){
                    (int)spell.position.x,
                    (int)spell.position.y,
                    4, 4
                }, Magic_pos_color
            );
            draw_sectors(game, spell, sector_line_color);

            direct = vec2_rotate(direct, -game->is.mouse.ywheel/10);
            draw_line_f(game->win->context, sector_line_color,
                (int)game->is.mouse.x, (int)game->is.mouse.y,
                (int)(game->is.mouse.x + direct.x),
                (int)(game->is.mouse.y + direct.y)
            );  

            vec2f origin_vec = MAGIC_RING_ORIGIN_VEC2(spell.ring);
            vec2f rotated_vec = vec2_rotate(origin_vec, PI_CONST);

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
