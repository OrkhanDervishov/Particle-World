#include "game.h"
#include "chunk_system.h"
#include "imgui_dev.c"
#include "chunk_system.h"

void Guide(ParticleEngine* game, Color textColor);
void init_buttons(ParticleEngine* game, Button** buttons);

bool cursor_enabled = FALSE;
void toggle_cursor(ParticleEngine* game){
    cursor_enabled = cursor_enabled ? FALSE : TRUE;
}

bool guide_enabled = FALSE;
void toggle_guide(ParticleEngine* game){
    guide_enabled = guide_enabled ? FALSE : TRUE;
}

bool is_controlled = FALSE;
void toggle_control(ParticleEngine* game){
    is_controlled = is_controlled ? FALSE : TRUE;
}

void call_all_callbacks(ParticleEngine* game){
    for(int i = 0; i < CB_COUNT_MAX; i++){
        if(game->callbacks[i] != NULL)
            game->callbacks[i](game);
    }
}


void func(){
    printf("Hello\n");
}

void clear_space(ParticleEngine* game){
    ClearFullCS(&game->cs);
}

void select_sand(ParticleEngine* game){
    game->g_params.selectedParticleType = SAND;
}
void select_water(ParticleEngine* game){
    game->g_params.selectedParticleType = WATER;
}
void select_steam(ParticleEngine* game){
    game->g_params.selectedParticleType = STEAM;
}
void select_acid(ParticleEngine* game){
    game->g_params.selectedParticleType = ACID;
}
void select_wood(ParticleEngine* game){
    game->g_params.selectedParticleType = WOOD;
}
void select_wall(ParticleEngine* game){
    game->g_params.selectedParticleType = WALL;
}
void select_fire(ParticleEngine* game){
    game->g_params.selectedParticleType = FIRE;
}
void select_fire_smoke(ParticleEngine* game){
    game->g_params.selectedParticleType = FIRE_SMOKE;
}
void select_fire_liquid(ParticleEngine* game){
    game->g_params.selectedParticleType = FIRE_LIQUID;
}
void select_smoke(ParticleEngine* game){
    game->g_params.selectedParticleType = SMOKE;
}
void select_coal(ParticleEngine* game){
    game->g_params.selectedParticleType = COAL;
}
void select_powder(ParticleEngine* game){
    game->g_params.selectedParticleType = POWDER;
}
void select_oil(ParticleEngine* game){
    game->g_params.selectedParticleType = OIL;
}
void select_lava(ParticleEngine* game){
    game->g_params.selectedParticleType = LAVA;
}
void select_fungus(ParticleEngine* game){
    game->g_params.selectedParticleType = FUNGUS;
}
void select_obsidian(ParticleEngine* game){
    game->g_params.selectedParticleType = OBSIDIAN;
}
void select_phantom(ParticleEngine* game){
    game->g_params.selectedParticleType = PHANTOM;
}
void select_source(ParticleEngine* game){
    game->g_params.selectedParticleType = SOURCE;
}


int RunEntityGame(ParticleEngine* game);
// int RunSpellGame(ParticleEngine* game);

int RunParticleEngine(ParticleEngine* game){
    
    // RunSpellGame(game);
    // return 0;
    // Window* win = game->win;
    // ChunkSpace* cs = &(game->cs);
    
    // clock_t minTime = 1000/game->s_params.frameLock;


    // clock_t iter_start = 0, iter_end = 1;
    // clock_t start = 0, end = 1;
    // clock_t draw_start = 0, draw_end = 0;
    // clock_t sim_start = 0, sim_end = 0;
    // clock_t simh_start = 0, simh_end = 0;
    
    // Color buttonColor = {.rgba = 0xFF0000FF};
    // vec2 pos = {10, win->h -50};
    // vec2 pos2 = {65, win->h -50};
    // vec2 sizes = {50, 25};

    // Button* buttons[100];
    // init_buttons(game, buttons);
    
    // // Create Objects
    // Image final_image;
    // Image part_map;
    // Image light_map;
    // Image blurred;
    // final_image.buffer = NULL;
    // part_map.buffer = NULL;
    // light_map.buffer = NULL;
    // blurred.buffer = NULL;
    // create_image(&final_image, win->context.width/DEFAULT_PARTICLE_SIZE, win->context.height/DEFAULT_PARTICLE_SIZE);
    // create_similar(&part_map, final_image);
    // create_similar(&light_map, final_image);
    // create_similar(&blurred, final_image);

    // Image background;
    // background.buffer = NULL;
    // load_image(&background, "resources/background.png");
    // Image minimized_bg = minimize_resolution(background, 16, 16);

    // EntityPool pool;
    // POOL_INIT(pool);

    // Entity bomb;
    // Image bomb_image;
    // bomb_image.buffer = NULL;
    // load_image(&bomb_image, "resources/bomb.png");
    // bomb.sprite.sprite = &bomb_image;
    // bomb.collider = (RectCollider){.collider = (Rectf){500.0f, 10.0f, (float)bomb_image.width*2, (float)bomb_image.height*2}};
    // bool deleted = FALSE;

    // Entity wizard;
    // Image wizard_image;
    // wizard_image.buffer = NULL;
    // load_image(&wizard_image, "resources/wizard.png");
    // wizard.sprite.sprite = &wizard_image;
    // wizard.collider = (RectCollider){.collider = (Rectf){10.0f, 10.0f, (float)wizard_image.width*2, (float)wizard_image.height*2}};
    // game->camera.pos.x = 100.0f;
    // game->camera.pos.y = 100.0f;

    // entity_id_t bomb_id = entity_add(&pool, bomb);
    // entity_id_t bomb_id1 = entity_add(&pool, bomb);
    // entity_id_t bomb_id2 = entity_add(&pool, bomb);
    // entity_id_t bomb_id3 = entity_add(&pool, bomb);
    // entity_id_t wizard_id = entity_add(&pool, wizard);

    // entity_pool_print_stats(&pool);

    // InputSystem is = {0};
    // init_input_system(&is);
    // action_t act0 = 0, act1 = 1, act2 = 2, act3 = 3, act_exit = 4;
    // add_binding(&is, BUTTON_Q, act0);
    // add_binding(&is, BUTTON_1, act0);
    // add_binding(&is, BUTTON_MOUSE_MIDDLE, act0);
    // add_binding(&is, BUTTON_TAB, act0);
    // add_binding(&is, BUTTON_W, act1);
    // add_binding(&is, BUTTON_MOUSE_RIGHT, act1);
    // add_binding(&is, BUTTON_E, act2);
    // add_binding(&is, BUTTON_MOUSE_LEFT, act2);
    // add_binding(&is, BUTTON_R, act3);
    // add_binding(&is, BUTTON_MOUSE_X1, act3);
    // add_binding(&is, BUTTON_MOUSE_X2, act3);
    // add_binding(&is, BUTTON_ESCAPE, act_exit);

    // char fpstext[64];
    // char typetext[64];
    // char brushtext[64];
    // char controltext[64];
    // SetChunkSpace(&(game->cs));
    // InitGuiRenderer();
    // start_particle_lighting_sw(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    // StartChunkRendererSW(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    // // start_particle_lighting_sw(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, 1);
    // // StartChunkRendererSW(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, 1);
    // InitBasicTextRenderer();
    // Color textColor = {.r=255, .g=0, .b=0, .a=255};
    // Color textColor2 = {.r=0, .g=255, .b=0, .a=255};
    // // Loop
    // while(win->isrunning){

    //     update_global_time();
    //     iter_start = clock();
    //     start = clock();
        
    //     update_input_system(&is);

    //     // Simulations
    //     if(!game->s_params.paused){
    //         // WallBoxCS(cs);
            
    //         // // Particle simulation
    //         // sim_start = GetTimeNano()/1000;
    //         // SimulateChunkSpace(cs);
    //         // RefreshChunkSpace(cs);
    //         // sim_end = GetTimeNano()/1000;
            
    //         // // Heatmap simulation
    //         // simh_start = GetTimeNano()/1000;
    //         // simh_end = GetTimeNano()/1000;
    //     }
        
    //     draw_start = GetTimeNano()/1000;
    //     // Entities
    //     fill_f(win->context, game->s_params.clear_color);
    //     {
    //         draw_image_on_fimage_scaled(
    //             win->context, *ENTITY_GET(pool, wizard_id).sprite.sprite, 
    //             (int)(ENTITY_GET(pool, wizard_id).collider.collider.x), (int)(ENTITY_GET(pool, wizard_id).collider.collider.y),
    //             2, 2
    //         );
    //         // draw_rect_collider_f(win->context, entity_collider, (Color){.rgba=0xFF00FFFF});
    //         if(!deleted){
    //             draw_image_on_fimage_scaled(
    //                 win->context, *ENTITY_GET(pool, bomb_id).sprite.sprite, 
    //                 (int)(ENTITY_GET(pool, bomb_id).collider.collider.x), (int)(ENTITY_GET(pool, bomb_id).collider.collider.y),
    //                 2, 2
    //             );
    //             // draw_rect_collider_f(win->context, entity_collider2, (Color){.rgba=0xFF00FFFF});
    //         }
    //     }

    //     {
    //         //     entity_collider.collider.x = game->camera.pos.x;
    //         // if(is_controlled){
    //             //     entity_collider.collider.y = game->camera.pos.y;
    //             // } else {
    //             if(
    //                 is_controlled ||
    //                 (!collide_rect_to_particle(cs, ENTITY_GET(pool, wizard_id).collider) && 
    //                 !collide_rect_to_rect(ENTITY_GET(pool, wizard_id).collider, ENTITY_GET(pool, bomb_id).collider))
    //             ){
    //                 ENTITY_GET(pool, wizard_id).collider.collider.y += 400.0f * get_global_delta();
    //                 // game->camera.pos.x = entity_collider.collider.x;
    //                 // game->camera.pos.y = entity_collider.collider.y;
    //                 ENTITY_GET(pool, wizard_id).collider.collider.x += game->camera.pos.x;
    //                 ENTITY_GET(pool, wizard_id).collider.collider.y += game->camera.pos.y;
    //             }
    //             game->camera.pos = (vec2f){0.0, 0.0};
    //         if(!deleted){
    //             if(
    //                 !collide_rect_to_particle(cs, ENTITY_GET(pool, bomb_id).collider) && 
    //                 !collide_rect_to_rect(ENTITY_GET(pool, bomb_id).collider, ENTITY_GET(pool, wizard_id).collider)
    //             ){
    //                 ENTITY_GET(pool, bomb_id).collider.collider.y += 100.0f * get_global_delta();
    //             } else {
    //                 deleted = TRUE;
    //                 CreateParticlesCircleCS(
    //                     cs, 
    //                     (int)ENTITY_GET(pool, bomb_id).collider.collider.x/DEFAULT_PARTICLE_SIZE, 
    //                     (int)ENTITY_GET(pool, bomb_id).collider.collider.y/DEFAULT_PARTICLE_SIZE,
    //                     10,
    //                     FIRE
    //                 );
    //                 Explosion(
    //                     cs, 
    //                     (int)ENTITY_GET(pool, bomb_id).collider.collider.x/DEFAULT_PARTICLE_SIZE, 
    //                     (int)ENTITY_GET(pool, bomb_id).collider.collider.y/DEFAULT_PARTICLE_SIZE,
    //                     1000, 1000000, FIRE_SMOKE
    //                 );
    //             }
    //         }
    //     }

        
    //     // fill_f(win->context, game->s_params.clear_color);
    //     // Rendering
    //     {
    //         // // draw_image_on_fimage_scaled(win->context, background, 0, 0, 2, 2);
    //         // // draw_image_on_image_scaled(final_image, minimized_bg, 0, 0, 4, 4);
    //         // // fill_image(final_image, game->s_params.clear_color);
    //         // fill_image(final_image, (Color){.rgba=0x00000000});
    //         // // draw_image_on_image(final_image, background, 0, 0);
    //         // // fill_image(part_map, (Color){.rgba=0x00000000});
    //         // // fill_image(light_map, (Color){.rgba=0x00000000});
    //         // DrawChunkSpaceSW(part_map, cs, 0, 0);
    //         // // draw_cs_lightmap(light_map, cs, 0, 0);
    //         // // blur_lightmap_strong(&blurred, light_map, 9, 1);
    //         // // blur_lightmap2(&blurred, light_map, 3);
    //         // // save_image_png(&light_map, "lightmap.png");
    //         // // additive_blend(final_image, part_map);
    //         // alpha_blend(final_image, part_map);
    //         // // additive_blend(final_image, blurred);
    //         // // draw_image_on_fimage_scaled(win->context, final_image, (int)game->camera.pos.x, (int)game->camera.pos.y, DEFAULT_PARTICLE_SIZE, DEFAULT_PARTICLE_SIZE);
    //         // draw_image_on_fimage_scaled(win->context, final_image, 0, 0, DEFAULT_PARTICLE_SIZE, DEFAULT_PARTICLE_SIZE);
    //     }
    //     // Call ParticleEngine callbacks
    //     // call_all_callbacks(game);
        
    //     {
    //         Color mouse_color = {.rgba = 0xFFFFFFFF};
    //         int mx, my;
    //         int state = SDL_GetMouseState(&mx, &my);
    //         int px = mx / DEFAULT_PARTICLE_SIZE;
    //         int py = my / DEFAULT_PARTICLE_SIZE;
    //         DrawGuiElement(win, &game->gui, 0, 0);
    //         draw_circle_f(win->context, mx, my, game->g_params.brush_size*DEFAULT_PARTICLE_SIZE, mouse_color, 2);
    //         Rect rect = {mx-1, my-1, 2, 2};
    //         draw_filled_rect_f(win->context, rect, mouse_color);
    //         char pointer_text[64];
    //         if(px >= 0 && px < cs->width_p && py >= 0 && py < cs->height_p){
    //             sprintf(pointer_text, "%s %d, %d", typeNameList[CS_GET_TYPE(cs, px, py)], px, py);
    //             BasicTextRender(game->win, pointer_text, mx+5, my, 1, textColor2);
    //         }  


    //         {
    //             char input_text[256];
    //             sprintf(input_text, "inputs: ");
                
    //             if(action_down(&is, act0)){
    //                 strcat(input_text, "act0, ");
    //             }
    //             if(action_pressed(&is, act1)){
    //                 strcat(input_text, "act1, ");
    //             }
    //             if(action_pressed(&is, act2)){
    //                 strcat(input_text, "act2, ");
    //             }
    //             if(action_pressed(&is, act3)){
    //                 strcat(input_text, "act3, ");
    //             }
    //             if(action_pressed(&is, act_exit)){
    //                 return 0;
    //             }
                
    //             BasicTextRender(game->win, input_text,     10, 100, 2, textColor);  
    //         }
    //         // RenderText
    //         BasicTextRender(game->win, fpstext,     10, 10, 2, textColor);  
    //         BasicTextRender(game->win, typetext,    10, 30, 2, textColor);  
    //         BasicTextRender(game->win, brushtext,   10, 50, 2, textColor); 
    //         BasicTextRender(game->win, controltext, 10, 70, 2, textColor); 
    //         if(guide_enabled) Guide(game, textColor);
            
    //         if(cursor_enabled) draw_cursor(win->context, game->mouse);
    //     }

    //     SDL_UpdateWindowSurface(win->window);
    //     draw_end = GetTimeNano()/1000;
        
        
    //     end = clock();
        
        
    //     deltaTime = end - start;
        
        
    //     // End Frame
    //     if(game->s_params.frameLockEnabled){
    //         if(deltaTime < minTime){
    //             SDL_Delay((minTime - deltaTime));
    //         }
    //     }
    //     else
    //     SDL_Delay(game->s_params.delay);
        
    //     iter_end = clock();
    //     sumTime += deltaTime;

    //     if(sumTime > 200){
    //     fps = 1000.0f / ((float)iter_end - (float)iter_start);
    //     sumTime = 0;
    //         sprintf(win->title, "Particles: %d FPS: %,0f Draw time: %d Sim time: %d HeatSim time: %d Overall: %d Delay: %d", 
    //                 0, 
    //                 fps, 
    //                 draw_end - draw_start,
    //                 sim_end - sim_start,
    //                 simh_end - simh_start,
    //                 end - start,
    //                 game->s_params.delay
    //             );
    //         SDL_SetWindowTitle(win->window, win->title);
    //         sprintf(fpstext, "fps: %.0f / draw_time: %.2fms / sim_time: %.2fms", 
    //             fps, (float)(draw_end - draw_start)/1000.f, (float)(sim_end - sim_start)/1000.f);
    //     }
    //     sprintf(typetext,   "selected type: %s", typeNameList[game->g_params.selectedParticleType]);
    //     sprintf(brushtext,  "brush radius: %d", game->g_params.brush_size);
    //     sprintf(controltext,"is controlled: %s", is_controlled ? "true" : "false");
    // }

    // EndBasicTextRenderer();
    // EndChunkRendererSW();
    // end_particle_lighting_sw();
    
    // // Delete Objects

    // // DeleteButton(&button1);
    // // DeleteButton(&button2);

    // printf("game running: %d\n", game->win->isrunning);
    return 0;
}



void poll_events(InputSystem* is, InputSystem* gui){
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        bool mouse_update = true;
        #ifdef PW_USE_IMGUI
        if(imgui_read_event(e)) mouse_update = false;
        #endif

        update_sdl_event_input_system(is, e, mouse_update);
    }
}


Transforms2d get_view_transform(Transforms2d transform, PWCamera2D camera){
    // printf("resx:%f, resy:%f\n", camera.resolution.x, camera.resolution.y);
    transform.translation.x -= camera.pos.x - camera.resolution.x/2;
    transform.translation.y -= camera.pos.y + camera.resolution.y/2;

    return transform;
}

void draw_entity(ParticleEngine* game, pw_entity_id_t id){

    Transforms2d model = {
        .translation = (vec2f){(ENTITY_GET(game->em.pool, id).pos.x), (ENTITY_GET(game->em.pool, id).pos.y)},
        .rotation = 0.0f,
        .scale = (vec2f){2.0f, 2.0f}
    }; 

    Transforms2d view = pw_world_to_view_transform(model, game->camera);
    // Transforms2d view = get_view_transform(model, game->camera);
    
    pw_draw_renderable(
        game->win->context, 
        &game->am,
        &ENTITY_GET(game->em.pool, id).renderable,
        view,
        PW_DELTA_TIME
    );


    if(show_positions){
        Rect rect = {
            .x = (int)view.translation.x,
            .y = -(int)view.translation.y,
            .w = 5,
            .h = 5,
        };
        pnt_draw_filled_rect(game->win->context, rect, (Color){.rgba = 0xFF00FF00});
    }


    if(show_colliders){
        view.translation.y = -view.translation.y;
        view.translation.x -= ENTITY_GET(game->em.pool, id).aabb.w/2;
        view.translation.y -= ENTITY_GET(game->em.pool, id).aabb.h/2;
        draw_rect_aabb_collider(
            game->win->context, 
            view.translation,
            ENTITY_GET(game->em.pool, id).aabb,
            (Color){.rgba = 0xFF00FFFF}
        );
    }
}

void draw_entities(ParticleEngine* game){
    for(pw_entity_id_t i = 0; i < game->em.pool.elems.count; i++){
        if(!ENTITY_IS_DELETED(game->em.pool, i)) continue;
        draw_entity(game, i);
    }
}

void delete_all_entities(ParticleEngine* game){
    for(pw_entity_id_t i = 0; i < game->em.pool.elems.count; i++){
        if(!ENTITY_IS_DELETED(game->em.pool, i)) continue;
        pw_entity_manager_delete(&game->em, i);
    }    
}

void clear_game_window(ParticleEngine* game){
    pnt_fill(game->win->context, game->s_params.clear_color);
}


void draw_scene(ParticleEngine* game){
    clear_game_window(game);
    draw_entities(game);
    if(game->s_params.use_custom_cursor)
        draw_cursor(game->win->context, game->mouse);
}


PWEntity block;
PWEntity player;

vec2f last_move;
vec2f prev_pos;

pw_entity_id_t player_id;
pw_entity_id_t entity_ids[10];

void init_scene(ParticleEngine *game){
    block.pos = (Posf){0.0f, -50.0f};
    entity_ids[0] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-200.0f, 0.0f};
    entity_ids[1] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-400.0f, 50.0f};
    entity_ids[2] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-600.0f, 100.0f};
    entity_ids[3] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-800.0f, 150.0f};
    entity_ids[4] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-1000.0f, 200.0f};
    entity_ids[5] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-1200.0f, 250.0f};
    entity_ids[6] = pw_entity_manager_add(&game->em, block);
    block.pos = (Posf){-1400.0f, 300.0f};
    entity_ids[7] = pw_entity_manager_add(&game->em, block);

    player_id = pw_entity_manager_add(&game->em, player);
}

#define GRAVITY gravity0*50.0f * PW_DELTA_TIME
void apply_gravity(ParticleEngine* game){
    for(int i = 0; i < 8; i++){
        PWEntity* ent = pw_entity_manager_get(&game->em, entity_ids[i]);
        if(ent->type == PW_ENTITY_DYNAMIC){
            ent->pos.y -= GRAVITY;
        }
    }
    
    PWEntity* ent = pw_entity_manager_get(&game->em, player_id);
    if(ent->type == PW_ENTITY_DYNAMIC){
        ent->pos.y -= GRAVITY;
        game->camera.pos.y -= GRAVITY;
    }
}


void handle_collisions_x(ParticleEngine* game){
    PWEntity* player_ent = pw_entity_manager_get(&game->em, player_id);

    for(int i = 0; i < 8; i++){
        PWEntity* ent = pw_entity_manager_get(&game->em, entity_ids[i]);
        
        int side = are_colliding_aabb(*player_ent, *ent);
        if(side){
            if(side == 1){
                player_ent->pos.x   -= last_move.x;
                game->camera.pos.x  -= last_move.x;
            }
            // if(side == 2){
            //     player_ent->pos.y   -= last_move.y;
            //     game->camera.pos.y  -= last_move.y;
            // }
            // player_ent->pos.y += GRAVITY;
            // game->camera.pos.y += GRAVITY;
        }
    }
}

bool colliding_y;
void handle_collisions_y(ParticleEngine* game){
    PWEntity* player_ent = pw_entity_manager_get(&game->em, player_id);
    
    colliding_y = false;
    for(int i = 0; i < 8; i++){
        PWEntity* ent = pw_entity_manager_get(&game->em, entity_ids[i]);
        
        int side = are_colliding_aabb(*player_ent, *ent);
        if(side){
            if(side == 2){
                player_ent->pos.y   -= last_move.y;
                game->camera.pos.y  -= last_move.y;
                colliding_y = true;
            }
        }
        colliding_y = colliding_y ? true : false;
    }
}

bool game_over(ParticleEngine* game){
    if(ENTITY_GET(game->em.pool, player_id).pos.y < -200.0f) return true;
    return false;
}

vec2f acc;

void jump(ParticleEngine* game){
    acc = (vec2f){0.0f, speed_y};
}

void accelerate(ParticleEngine* game){
    if(acc.y <= 0.0f) acc.y = 0.0f;
    ENTITY_GET(game->em.pool, player_id).pos.y += acc.y*PW_DELTA_TIME;
    acc.y -= speed_y/100;
}

void draw_key(ParticleEngine* game, PWSpriteFonts fonts, const char* key, Color color, vec2 pos){
    Rect button_rect = {pos.x, pos.y, 50, 50};
    pnt_draw_rect(game->win->context, button_rect, color, 5);
    pw_draw_sprite_text(
        game->win->context, 
        &game->am, &fonts, 
        key, 
        (Transforms2d){
            .translation = (vec2f){(float)pos.x, (float)-(pos.y + button_rect.h/2)},
            .rotation = 0.0f,
            .scale = (vec2f){5.0f, 5.0f}
        }
    );
}

void show_if_hold(ParticleEngine* game, PWSpriteFonts fonts){
    Color white = {.rgba = 0xFFFFFFFF};
    Color yellow = {.rgba = 0xFF00FFFF};

    int offset = 5;
    vec2 w_pos = {70 + offset*2, game->win->h-130-offset};
    vec2 s_pos = {70 + offset*2, game->win->h-70};
    vec2 a_pos = {10 + offset, game->win->h-70};
    vec2 d_pos = {130 + offset*3, game->win->h-70};

    if(button_down(&game->is, BUTTON_W)){
        draw_key(game, fonts, "W", yellow, w_pos);
    } else {
        draw_key(game, fonts, "W", white, w_pos);
    }

    if(button_down(&game->is, BUTTON_S)){
        draw_key(game, fonts, "S", yellow, s_pos);
    } else {
        draw_key(game, fonts, "S", white, s_pos);
    }

    if(button_down(&game->is, BUTTON_A)){
        draw_key(game, fonts, "A", yellow, a_pos);
    } else {
        draw_key(game, fonts, "A", white, a_pos);
    }

    if(button_down(&game->is, BUTTON_D)){
        draw_key(game, fonts, "D", yellow, d_pos);
    } else {
        draw_key(game, fonts, "D", white, d_pos);
    }
}



#define CHUNK_WIDTH     256
#define CHUNK_HEIGHT    256
#define FIELD_WIDTH     10
#define FIELD_HEIGHT    10

#define TURN_CHUNK_X(x, w) ((x) / (w)) * (w)
#define TURN_CHUNK_Y(y, h) ((y) / (h)) * (h)

#define VECF_TO_VECI(vf) (vec2){(int)vf.x, (int)vf.y}
#define VECI_TO_VECF(vi) (vec2f){(float)vi.x, (float)vi.y}

int chunk_x(int x)
{
    return (x >= 0) ? (x / CHUNK_WIDTH) * CHUNK_WIDTH : ((x - CHUNK_WIDTH + 1) / CHUNK_WIDTH) * CHUNK_WIDTH;
}

int chunk_y(int y)
{
    return (y >= 0) ? (y / CHUNK_HEIGHT) * CHUNK_HEIGHT : ((y - CHUNK_HEIGHT + 1) / CHUNK_HEIGHT) * CHUNK_HEIGHT;
}

vec2f get_view_vec(vec2f pos, PWCamera2D camera){
    pos.x -= camera.pos.x - camera.resolution.x/2;
    pos.y -= camera.pos.y + camera.resolution.y/2;
    return pos;
}

void draw_chunk_grid(ParticleEngine *game, PWSpriteFonts fonts){
    vec2f pos = ENTITY_GET(game->em.pool, player_id).pos;
    vec2 posi = {(int)pos.x, (int)pos.y};
    vec2 chunk_pos = {chunk_x(posi.x), chunk_y(posi.y)};
    vec2 rel_chunk_pos = VECF_TO_VECI(get_view_vec(VECI_TO_VECF(chunk_pos), game->camera));

    char chunk_info[256];
    sprintf(chunk_info, "x:%d y:%d", chunk_pos.x, chunk_pos.y);
    pw_draw_sprite_text(
        game->win->context, 
        &game->am, &fonts, 
        chunk_info, 
        (Transforms2d){
            .translation = (vec2f){0.0f, -40.0f},
            .rotation = 0.0f,
            .scale = (vec2f){2.0f, 2.0f}
        }
    );

    for(int i = -FIELD_HEIGHT/2; i < FIELD_HEIGHT/2+1; i++){
        for(int j = -FIELD_WIDTH/2; j < FIELD_WIDTH/2+1; j++){
            Rect rect = {
                .x = rel_chunk_pos.x + j*CHUNK_WIDTH,// CHUNK_WIDTH,
                .y = -rel_chunk_pos.y - CHUNK_HEIGHT + i*CHUNK_HEIGHT,
                .w = CHUNK_WIDTH,
                .h = CHUNK_HEIGHT
            };
            pnt_draw_rect(game->win->context, rect, (Color){.rgba=0xFF00FF00}, 1);
            // char rel_chunk_info[256];
            // sprintf(rel_chunk_info, "x:%d y:%d", rect.x, rect.y);
            // pw_draw_sprite_text(
            //     game->win->context, 
            //     &game->am, &fonts, 
            //     rel_chunk_info, 
            //     (Transforms2d){
            //         .translation = (vec2f){0.0f, -60.0f},
            //         .rotation = 0.0f,
            //         .scale = (vec2f){2.0f, 2.0f}
            //     }
            // );
        }
    }


}

typedef struct{
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

typedef struct{
    Numbers elems;
    Indices free_indices;
    Slots slots;
} NumberPool;



PWLayerSystem gen_ls(){
    PWLayerSystem ls = {0};
    PWLayer layer1 = pw_layer_create(PW_LAYER_GRID, sizeof(int), 100, 32, 32);
    pw_layer_add_sublayer(&layer1, sizeof(int));

    PWLayer layer2 = pw_layer_create(PW_LAYER_ENTITY, 16, 100, 32, 32);
    PWLayer layer3 = pw_layer_create(PW_LAYER_GRID, 20, 100, 32, 32);
    
    // printf("works1\n");
    pw_layer_sys_layer_add(&ls, layer1, pw_layer1_render, pw_layer1_generate);
    // printf("works2\n");
    // pw_layer_sys_layer_add(&ls, layer2);
    // pw_layer_sys_layer_add(&ls, layer3);
    pw_layer_sys_info(&ls);

    // layer1_fill(&layer1);

    return ls;
}

int RunEntityGame(ParticleEngine* game){
    
    // ParticleEngine* gui_engine;
    // CreateParticleEngine(&gui_engine, "./configs/gui_conf.conf");
    
    Image image;
    image.buffer = NULL;
    pnt_load_image(&image, "resources/CHESS.bmp");
    
    game->field.ls = gen_ls();
    
    pw_asset_t player_asset = pw_load_asset(&game->am, "resources/wizard.png", PW_ASSET_SPRITE);
    pw_asset_t block_asset = pw_load_asset(&game->am, "resources/block.png", PW_ASSET_SPRITE);
    pw_asset_t barrel_asset = pw_load_asset(&game->am, "resources/bomb.png", PW_ASSET_SPRITE);
    pw_asset_t active_bombs_asset = pw_load_asset(&game->am, "resources/active_bomb_sprites.png", PW_ASSET_SPRITE);
    pw_asset_t eye_asset = pw_load_asset(&game->am, "resources/eye_sprites.png", PW_ASSET_SPRITE);
    pw_make_asset_image_multiple_auto(&game->am, active_bombs_asset, (vec2){1,3});
    pw_make_asset_image_multiple_auto(&game->am, eye_asset, (vec2){1,5});

    PWFrames frames;
    da_append(frames, 0);
    da_append(frames, 1);
    da_append(frames, 2);
    PWTimes delays;
    da_append(delays, 0.2f);
    da_append(delays, 0.2f);
    da_append(delays, 0.2f);
    
    pw_asset_t bomb_animation = pw_sprite_animation_create_load(&game->am, active_bombs_asset, frames, delays);
    pw_asset_t eye_animation = pw_sprite_animation_create_load(&game->am, eye_asset, frames, delays);
    
    PWRenderable bomb_animator = pw_sprite_animator_create_renderable(&game->am, bomb_animation, TRUE, TRUE);
    PWRenderable eye_animator = pw_sprite_animator_create_renderable(&game->am, eye_animation, TRUE, TRUE);
    
    player = (PWEntity){
        .type = PW_ENTITY_DYNAMIC,
        .pos = {0.0f, 100.0f},
        .renderable.asset = player_asset,
        // .aabb = (PWColliderAABB){16.0f, 0.0f, 48.0f, 64.0f},
        .aabb = (PWColliderAABB){16.0f, 0.0f, 32.0f, 64.0f},
    };


    block = (PWEntity){
        .type = PW_ENTITY_STATIC,
        .pos = {100.0f, 100.0f},
        .renderable.asset = block_asset,
        .aabb = (PWColliderAABB){0.0f, 0.0f, 32.0f*2, 32.0f*2},
    };

    PWEntity bomb = {
        .type = PW_ENTITY_DYNAMIC,
        .pos = {100.0f, 100.0f},
        .renderable = bomb_animator,
        .aabb = (PWColliderAABB){0.0f, 0.0f, 10.0f*2, 10.0f*2},
    };

    init_scene(game);
    
    // entity_id_t bomb0 = entity_add(&game->em, bomb);
    
    /******************************************************/
    
    pw_asset_t font_sprite = pw_load_asset(&game->am, "resources/font.png", PW_ASSET_SPRITE);
    pw_make_asset_image_multiple_auto(&game->am, font_sprite, (vec2){1,99});
    pw_asset_t font2_sprite = pw_load_asset(&game->am, "resources/font2.png", PW_ASSET_SPRITE);
    pw_make_asset_image_multiple_auto(&game->am, font2_sprite, (vec2){6,18});
    
    PWAsset *font2_asset = pw_asset_manager_get_asset(&game->am, font2_sprite);
    Image *font2_image = pw_asset_manager_get_image(&game->am, font2_asset->sprite.image_id);
    pnt_change_color(*font2_image, (Color){.rgba = 0xFF000000}, (Color){.rgba = 0x00000000});
    
    PWSpriteFonts fonts = pw_load_sprite_fonts_manual(font_sprite, "\r!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\"");
    // PWSpriteFonts fonts2 = pw_load_sprite_fonts_manual(font2_sprite, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    
    
    
    /******************************************************/
    
    action_t act_exit = 0;
    action_t act_create_bomb = 1;
    action_t act_delete_entites = 2;
    action_t act_reload = 3;
    
    add_binding(&game->is, BUTTON_ESCAPE, act_exit);
    add_binding(&game->is, BUTTON_MOUSE_LEFT, act_create_bomb);
    add_binding(&game->is, BUTTON_SPACE, act_create_bomb);
    add_binding(&game->is, BUTTON_TAB, act_create_bomb);
    add_binding(&game->is, BUTTON_Q, act_create_bomb);
    add_binding(&game->is, BUTTON_C, act_delete_entites);
    add_binding(&game->is, BUTTON_X, act_delete_entites);
    add_binding(&game->is, BUTTON_1, act_delete_entites);
    add_binding(&game->is, BUTTON_F1, act_reload);
    
    #ifdef PW_USE_IMGUI
    imgui_init(game);
    #endif

    acc = (vec2f){0.0f, 0.0f};

    while(game->s_params.is_running){
    
        update_global_time();
        reset_button_states(&game->is);
        poll_events(&game->is, NULL);

        if(action_pressed(&game->is, act_exit)){
            game->s_params.is_running = FALSE;
        }
        if(action_down(&game->is, act_create_bomb)){
            bomb.pos.x = game->camera.pos.x + game->is.mouse.x;
            bomb.pos.y = game->camera.pos.y - game->is.mouse.y;
            // pw_entity_manager_add(&game->em, bomb);
        }
        if(action_pressed(&game->is, act_delete_entites)){
            delete_all_entities(game);
        }
        if(action_pressed(&game->is, act_reload)){
            game->s_params.is_running = FALSE;
            game->s_params.restart = TRUE;
        }

        if(!paused){
            vec2f move;
            #define SPEED_X speed_x * PW_DELTA_TIME
            #define SPEED_Y speed_y * PW_DELTA_TIME
            if(button_down(&game->is, BUTTON_W)){
                if(levitate){
                    game->camera.pos.y += SPEED_Y;
                    ENTITY_GET(game->em.pool, player_id).pos.y += SPEED_Y;
                }
                else{
                    if(colliding_y)
                        jump(game);
                }
            }
            if(button_down(&game->is, BUTTON_S)){
                if(levitate){
                    game->camera.pos.y -= SPEED_Y;
                    ENTITY_GET(game->em.pool, player_id).pos.y -= SPEED_Y;
                }
            }
            accelerate(game);
            apply_gravity(game);
            move.y = ENTITY_GET(game->em.pool, player_id).pos.y - prev_pos.y;
            last_move.y = move.y != 0.0f ? move.y : last_move.y;
            handle_collisions_y(game);
            if(button_down(&game->is, BUTTON_A)){
                game->camera.pos.x -= SPEED_X;
                ENTITY_GET(game->em.pool, player_id).pos.x -= SPEED_X;
            }
            if(button_down(&game->is, BUTTON_D)){
                game->camera.pos.x += SPEED_X;
                ENTITY_GET(game->em.pool, player_id).pos.x += SPEED_X;
            }
            move.x = ENTITY_GET(game->em.pool, player_id).pos.x - prev_pos.x;
            last_move.x = move.x != 0.0f ? move.x : last_move.x;
            handle_collisions_x(game);
            game->camera.pos = ENTITY_GET(game->em.pool, player_id).pos;
            // vec2f move = vec2_sub(ENTITY_GET(game->em.pool, player_id).pos, prev_pos);

            prev_pos = ENTITY_GET(game->em.pool, player_id).pos;
        }

        Transforms2d t = (Transforms2d){
            .translation = (vec2f){400.0f, 400.0f},
            .rotation = 0.0f,
            .scale = (vec2f){3.0f, 3.0f}
        };

        if(game_over(game)){
            ENTITY_GET(game->em.pool, player_id).pos = player.pos;
            game->camera.pos = player.pos;
        }

        // pw_draw_asset(
        //     game->win->context, 
        //     &game->am, 
        //     active_bombs_asset, 
        //     t
        // );

        // pnt_blit_transformed(game->win->context, frame0, t.translation, t.rotation, t.scale);

        // pnt_blit(game->win->context, frame0, 450, 400);
        // pnt_blit(game->win->context, frame1, 500, 400);
        // pnt_blit(game->win->context, frame2, 550, 400);

        // pnt_blit_scaled(game->win->context, image, 100, 10, 2.5f, 1.0f);


        // pw_draw_renderable(
        //     game->win->context, 
        //     &game->am,
        //     &bomb_animator,
        //      (Transforms2d){
        //         .translation = (vec2f){(float)game->win->w/2, (float)game->win->h/2},
        //         .rotation = 0.0f,
        //         .scale = (vec2f){10.0f, 10.0f}
        //     },
        //     PW_DELTA_TIME
        // );

        // Transforms2d font_transforms = {
        //     .translation = {100.0f, 100.0f},
        //     .rotation = 0.0f,
        //     .scale = {2.5f, 2.5f}
        // };
        // pw_draw_sprite_text(game->win->context, &game->am, &fonts, "Hello World!\nerfegrg\nwefefef", font_transforms);

        // pw_time_t layer_start = get_current_time();
        draw_scene(game);
        // pw_time_t layer_end = get_current_time();
        // printf("%lf\n", layer_end - layer_start);
        pw_field_update(
            &game->field, 
            (pw_chunk_coord_t)ENTITY_GET(game->em.pool, player_id).pos.x, 
            (pw_chunk_coord_t)ENTITY_GET(game->em.pool, player_id).pos.y 
        );
        // printf("region_count: %d\n", pool_get_count(field.regions));
        // printf("field_x:%d field_y:%d\n", field.x, field.y);
        pw_field_regions_render(game->win->context, game->camera, game->field, TRUE);
        // pw_field_chunks_render(game->win->context, game->camera, game->field, TRUE);
        
        // pw_time_t layer_start = get_current_time();
        // pw_layer1_render(
        //     game->win->context, game->camera, 
        //     da_get(game->field.ls.layers, 0), 
        //     0, 0, 
        //     game->field.chunk_width, game->field.chunk_height
        // );
        // pw_time_t layer_end = get_current_time();
        // printf("%lf\n", layer_end - layer_start);
        // pw_chunk_render(
        //     game->win->context, game->camera,
        //     &field.chunks[0], 
        //     (int)field.x + 0*field.chunk_width, 
        //     (int)field.y + 0*field.chunk_height,
        //     (Color){.rgba = 0xFF00FFFF}
        // );
        // printf("x:%d y:%d\n", (int)field.x, (int)field.y);
        // draw_chunk_grid(game, fonts);
        show_if_hold(game, fonts);

        char camera_info[256];
        sprintf(camera_info, "x:%.2f y:%.2f", game->camera.pos.x, game->camera.pos.y);
        pw_draw_sprite_text(
            game->win->context, 
            &game->am, &fonts, 
            camera_info, 
            (Transforms2d){
                .translation = (vec2f){0.0f, -20.0f},
                .rotation = 0.0f,
                .scale = (vec2f){2.0f, 2.0f}
            }
        );
        pw_window_prepare_renderer(game->win);
        #ifdef PW_USE_IMGUI
        imgui_dev(game);
        #endif
        pw_window_present_renderer(game->win);
    }

    #ifdef PW_USE_IMGUI
    imgui_uninit(game);
    #endif
    // DeleteParticleEngine(&gui_engine);
    
    return 0;
}

const char* guideChangeType =       "Change particle type - V";
const char* guideCreate =           "Create particles - Mouse left";
const char* guideDelete =           "Delete particles - Mouse right";
const char* guideClear =            "Delete all particles - C";
const char* guideBrushIncrease =    "Increase brush size - 1";
const char* guideBrushDecrease =    "Decrease brush size - 2";
const char* guideGravity =          "Reverse gravity - X";
const char* guideLine =             "Create particles along a line - Z";
const char* guideScreenshot =       "Take screenshot - TAB";
const char* guideSpell =            "Cast spell - A";
const char* guideChunks =           "Show chunks - SPACE";
const char* guideExit =             "Exit - ESC";
void Guide(ParticleEngine* game, Color textColor){
    // int startX = game->win->screen.width-350;
    int startX = 10;
    int startY = 100;
    BasicTextRender(game->win, guideChangeType,     startX, startY, 2, textColor);  
    BasicTextRender(game->win, guideCreate,         startX, startY+20, 2, textColor);  
    BasicTextRender(game->win, guideDelete,         startX, startY+40, 2, textColor);  
    BasicTextRender(game->win, guideClear,          startX, startY+60, 2, textColor);  
    BasicTextRender(game->win, guideBrushIncrease,  startX, startY+80, 2, textColor);  
    BasicTextRender(game->win, guideBrushDecrease,  startX, startY+100, 2, textColor);  
    BasicTextRender(game->win, guideGravity,        startX, startY+120, 2, textColor);  
    BasicTextRender(game->win, guideLine,           startX, startY+140, 2, textColor);  
    BasicTextRender(game->win, guideScreenshot,     startX, startY+160, 2, textColor);  
    BasicTextRender(game->win, guideSpell,          startX, startY+180, 2, textColor);  
    BasicTextRender(game->win, guideChunks,         startX, startY+200, 2, textColor);  
    BasicTextRender(game->win, guideExit,           startX, startY+220, 2, textColor);  
}


void init_buttons(ParticleEngine* game, Button** buttons){
    Color buttonColor = {.rgba = 0xFF0000FF};
    vec2 sizes = {70, 30};

    CreateButton(&buttons[0],   "Clear",      buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*0,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)clear_space);
    CreateButton(&buttons[18],  "Cursor",     buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*1,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)toggle_cursor);
    CreateButton(&buttons[19],  "Guide",      buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*2,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)toggle_guide);
    CreateButton(&buttons[20],  "Control",    buttonColor,                  TRUE, (Pos){10+(sizes.x+5)*3,   game->win->h -(sizes.y+5)*2 - 5},   sizes, (GENERIC_FUNC_POINTER)toggle_control);
    
    CreateButton(&buttons[1],   "Sand",       typeColorList[SAND][0],           TRUE, (Pos){10+(sizes.x+5)*0,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_sand);
    CreateButton(&buttons[2],   "Water",      typeColorList[WATER][0],          TRUE, (Pos){10+(sizes.x+5)*1,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_water);
    CreateButton(&buttons[3],   "Steam",      typeColorList[STEAM][0],          TRUE, (Pos){10+(sizes.x+5)*2,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_steam);
    CreateButton(&buttons[4],   "Acid",       typeColorList[ACID][0],           TRUE, (Pos){10+(sizes.x+5)*3,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_acid);
    CreateButton(&buttons[5],   "Wood",       typeColorList[WOOD][0],           TRUE, (Pos){10+(sizes.x+5)*4,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_wood);
    CreateButton(&buttons[6],   "Wall",       typeColorList[WALL][0],           TRUE, (Pos){10+(sizes.x+5)*5,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_wall);
    CreateButton(&buttons[7],   "Fire",       typeColorList[FIRE][0],           TRUE, (Pos){10+(sizes.x+5)*6,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire);
    CreateButton(&buttons[8],   "Fire Smoke", typeColorList[FIRE_SMOKE][0],     TRUE, (Pos){10+(sizes.x+5)*7,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire_smoke);
    CreateButton(&buttons[9],   "Fire Liquid",typeColorList[FIRE_LIQUID][0],    TRUE, (Pos){10+(sizes.x+5)*8,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fire_liquid);
    CreateButton(&buttons[10],   "Smoke",     typeColorList[SMOKE][0],          TRUE, (Pos){10+(sizes.x+5)*9,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_smoke);
    CreateButton(&buttons[11],  "Coal",       typeColorList[COAL][0],           TRUE, (Pos){10+(sizes.x+5)*10,   game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_coal);
    CreateButton(&buttons[12],  "Powder",     typeColorList[POWDER][0],         TRUE, (Pos){10+(sizes.x+5)*11,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_powder);
    CreateButton(&buttons[13],  "Oil",        typeColorList[OIL][0],            TRUE, (Pos){10+(sizes.x+5)*12,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_oil);
    CreateButton(&buttons[14],  "Lava",       typeColorList[LAVA][0],           TRUE, (Pos){10+(sizes.x+5)*13,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_lava);
    CreateButton(&buttons[15],  "Fungus",     typeColorList[FUNGUS][0],         TRUE, (Pos){10+(sizes.x+5)*14,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_fungus);
    CreateButton(&buttons[16],  "Phantom",    typeColorList[PHANTOM][0],        TRUE, (Pos){10+(sizes.x+5)*15,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_phantom);
    CreateButton(&buttons[17],  "Source",     typeColorList[SOURCE][0],         TRUE, (Pos){10+(sizes.x+5)*16,  game->win->h -(sizes.y+5)*1 - 5},   sizes, (GENERIC_FUNC_POINTER)select_source);
    
    // PrintButtonParams(buttons[0]);
    for(int i = 0; i < 21; i++){
        add_button_gui(&game->gui, buttons[i], 0.2f);
    }
    // add_button_gui(&game->gui, button2);
}
