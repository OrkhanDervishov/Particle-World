#include "chunk_system.h"

void pw_draw_chunk(Image context, PWChunk *chunk){
    Rect rect = {
        .x = chunk->x,
        .y = chunk->y,
        .w = chunk->w,
        .h = chunk->h,
    };
    pnt_draw_rect(context, rect, (Color){.rgba=0xFF00FF00}, 1);
}


PWLayers pw_layers_copy(PWLayers layers){
    for(size_t i = 0; i < layers.count; i++){
        da_get(layers, i).data = (AnyData){0};
        if(da_get(layers, i).type == PW_LAYER_GRID){
            da_reserve(layers, da_get(layers, i).item_size * da_get(layers, i).size);
        }
    }
    return layers;
}

void pw_layer_free(PWLayer layer){
    da_free(layer.data);
}

void pw_layers_free(PWLayers layers){
    for(size_t i = 0; i < layers.count; i++){
        pw_layer_free(da_get(layers, i));
    }
}



/*********************************************/

PWChunk pw_chunk_create(
    pw_chunk_coord_t x, pw_chunk_coord_t y,
    pw_chunk_coord_t w, pw_chunk_coord_t h,
    PWLayers layers
){
    PWChunk chunk = {
        .x = x, .y = y, .w = w, .h = h,
        .layers = pw_layers_copy(layers)
    };

    return chunk;
}

void pw_chunk_delete(PWChunk *chunk){
    pw_layers_free(chunk->layers);
}


/*********************************************/

const char* pw_gen_region_path(pw_chunk_coord_t x, pw_chunk_coord_t y){
    
    return NULL;
}

PWRegion pw_region_create(
    pw_chunk_coord_t x, pw_chunk_coord_t y, 
    size_t width_in_chunk, size_t height_in_chunk, 
    size_t chunk_width, size_t chunk_height
){
    PWRegion region = {
        .x = x, .y = y, 
        .width_in_chunk = width_in_chunk, .height_in_chunk = height_in_chunk,
        .width = width_in_chunk*chunk_width, .height = height_in_chunk*chunk_height,
        .loaded = TRUE, .id = 0
    };
    region.chunks = (PWChunk*)malloc(region.width_in_chunk * region.height_in_chunk * sizeof(PWChunk));
    return region;
}

void pw_region_delete(PWRegion* region){
    free(region->chunks);
}

int pw_region_load(PWRegion *region, const char *path){
    // TODO: figure out file structure
    return 1;
}

int pw_region_save(const char *path, PWRegion region){
    // TODO: figure out file structure
    return 0;
}

PWRegion pw_region_create_or_load(
    const char* path, 
    pw_chunk_coord_t x, pw_chunk_coord_t y, 
    size_t width_in_chunk, size_t height_in_chunk,
    size_t chunk_width, size_t chunk_height
){
    PWRegion region;
    if(pw_region_load(&region, path)){
        region = pw_region_create(x, y, width_in_chunk, height_in_chunk, chunk_width, chunk_height);
    }
    return region;
}

/*********************************************/



int pw_field_init(
    PWField *field, 
    size_t region_width_in_chunks, size_t region_height_in_chunks,
    size_t field_width_in_chunks, size_t field_height_in_chunks,
    size_t chunk_width, size_t chunk_height
){
    if(!field) return 1;

    printf("works0\n");
    field->field_width_in_chunks = field_width_in_chunks;
    field->field_height_in_chunks = field_height_in_chunks;
    
    field->region_width_in_chunks = region_width_in_chunks;
    field->region_height_in_chunks = region_height_in_chunks;
    
    field->chunk_width = chunk_width;
    field->chunk_height = chunk_height;
    
    printf("works1\n");
    
    if(field->chunks != NULL){
        free(field->chunks);
    }
    printf("works2\n");
    pool_free(field->regions);
    printf("works3\n");
    
    field->regions = (PWRegionPool){0};
    printf("works4\n");
    field->chunks = (PWChunk*)calloc(
        field->field_width_in_chunks * field->field_height_in_chunks, 
        sizeof(PWChunk)
    );
    printf("works5\n");

    return 0;
}

void pw_field_destroy(PWField *field){
    pool_free(field->regions);
    free(field->chunks);
}

void pw_field_chunk_organize(PWField field){
    
    // return;
    // Organize chunks from region list
    for(size_t i = 0; i < field.field_height_in_chunks; i++)
    for(size_t j = 0; j < field.field_width_in_chunks; j++){
        pw_chunk_coord_t cx = field.x + j * field.chunk_width;
        pw_chunk_coord_t cy = field.y + i * field.chunk_height;

        size_t region_width = field.region_width_in_chunks * field.chunk_width;
        size_t region_height = field.region_height_in_chunks * field.chunk_height;
        pw_chunk_coord_t rx = (cx / region_width) * region_width;
        pw_chunk_coord_t ry = (cy / region_height) * region_height;

        PWChunk chunk = {0};
        for(size_t k = 0; k < pool_get_count(field.regions); k++){
            PWRegion region = pool_get(field.regions, k);
            if(region.x == rx && region.y == ry){
                size_t ri = (cy - ry) / field.chunk_height;
                size_t rj = (cx - rx) / field.chunk_width;

                size_t index = ri * (field.region_width_in_chunks) + rj;
                chunk = pool_get(field.regions, k).chunks[index];
                break;
            }
        }

        field.chunks[i * field.field_width_in_chunks + j] = chunk;
    }
}


void pw_field_update(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){

    // 1 - check if all central chunk is changed
    // TODO: Also consider update distance
    bool is_changed = FALSE;

    pw_chunk_coord_t curr_central_chunk_x = field->x_center / field->chunk_width;
    pw_chunk_coord_t curr_central_chunk_y = field->y_center / field->chunk_height;

    pw_chunk_coord_t new_central_chunk_x = x / field->chunk_width;
    pw_chunk_coord_t new_central_chunk_y = y / field->chunk_height + 1;

    if(
        curr_central_chunk_x != new_central_chunk_x ||
        curr_central_chunk_y != new_central_chunk_y
    ) {
        // printf("curr_x:%d curr_y:%d\n", curr_central_chunk_x, curr_central_chunk_y);
        // printf("new_x:%d new_y:%d\n", new_central_chunk_x, new_central_chunk_y);
        is_changed = TRUE;
    }

    field->x_center = (x / field->chunk_width) * field->chunk_width; 
    field->y_center = (y / field->chunk_height + 1) * field->chunk_height;
    field->x = field->x_center - (field->field_width_in_chunks/2) * field->chunk_width;
    field->y = field->y_center + (field->field_height_in_chunks/2) * field->chunk_height;
    // printf("field_x:%d field_y:%d\n", field->x, field->y);
    // printf("field_x_c:%d field_y_c:%d\n", field->x_center, field->y_center);

    // 2 - check if all required regions are loaded

    if(is_changed){
        pw_field_region_load(field, x, y);
        
        // 4 - organize chunks in field
        pw_field_chunk_organize(*field);
    }
}

typedef struct{
    pw_chunk_coord_t x, y;
} vec2_chunk_coord;

vec2_chunk_coord pw_field_coord_region(PWField field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    size_t region_width = field.region_width_in_chunks * field.chunk_width;
    size_t region_height = field.region_height_in_chunks * field.chunk_height;

    return (vec2_chunk_coord){
        (x / (pw_chunk_coord_t)region_width + (x < 0 ? -1 : 0)) * (pw_chunk_coord_t)region_width,
        (y / (pw_chunk_coord_t)region_height + (y < 0 ? 0 : 1)) * (pw_chunk_coord_t)region_height
    };
}


void pw_field_region_load(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    /*
    If region is stored - load region
    If region is not generated - generate
    */
    // TODO: need region file naming convention

    // size_t region_width = field->region_width_in_chunks * field->chunk_width;
    // size_t region_height = field->region_height_in_chunks * field->chunk_height;
    // pw_chunk_coord_t rx = (field->x / (pw_chunk_coord_t)region_width + (field->x < 0 ? -1 : 0)) * (pw_chunk_coord_t)region_width;
    // pw_chunk_coord_t ry = (field->y / (pw_chunk_coord_t)region_height + (field->y < 0 ? 0 : 1)) * (pw_chunk_coord_t)region_height;
    
    pw_chunk_coord_t field_endx = field->x + (pw_chunk_coord_t)(field->chunk_width * (field->field_width_in_chunks));
    pw_chunk_coord_t field_endy = field->y - (pw_chunk_coord_t)(field->chunk_height * (field->field_height_in_chunks));

    vec2_chunk_coord region_start_pos = pw_field_coord_region(*field, field->x, field->y);
    vec2_chunk_coord region_end_pos = pw_field_coord_region(*field, field_endx, field_endy);

    pw_chunk_coord_t start_rx = region_start_pos.x;
    pw_chunk_coord_t start_ry = region_start_pos.y;
    pw_chunk_coord_t end_rx = region_end_pos.x;
    pw_chunk_coord_t end_ry = region_end_pos.y;

    // printf("start_rx:%d start_ry:%d\n", start_rx, start_ry);
    // printf("end_rx:%d end_ry:%d\n", end_rx, end_ry);
    // printf("field.x:%d field.y:%d\n", field->x, field->y);
    // printf("field.ex:%d field.ey:%d\n", field->x + field, field->y);

    for(pw_chunk_coord_t i = start_ry; i >= end_ry; i -= field->region_height_in_chunks*field->chunk_height){
        for(pw_chunk_coord_t j = start_rx; j <= end_rx; j += field->region_width_in_chunks*field->chunk_width){
            bool is_loaded = FALSE;
            for(size_t k = 0; k < pool_get_count(field->regions); k++){
                PWRegion region = pool_get(field->regions, k);
                if(region.x == j && region.y == i){
                    is_loaded = TRUE;
                    break;
                }
            }

            if(!is_loaded){
                // printf("field_x:%d field_y:%d\n", field->x, field->y);
                // printf("requested region: %d %d\n", j, i);
                PWRegion region = pw_region_create_or_load(
                    pw_gen_region_path(j, i), 
                    j, i, 
                    field->region_width_in_chunks,
                    field->region_height_in_chunks,
                    field->chunk_width,
                    field->chunk_height
                );
                
                // printf("region: %d %d\n", region.x, region.y);
                size_t index;
                pool_append(field->regions, region, index);
            }
        }
    }

    // 3 - load all required regions
    
    // if(!is_loaded){
    //     printf("field_x:%d field_y:%d\n", field->x, field->y);
    //     printf("requested region: %d %d\n", rx, ry);
    //     PWRegion region = pw_region_create_or_load(
    //         pw_gen_region_path(rx, ry), 
    //         rx, ry, 
    //         field->region_width_in_chunks,
    //         field->region_height_in_chunks,
    //         field->chunk_width,
    //         field->chunk_height
    //     );
        
    //     printf("region: %d %d\n", region.x, region.y);
    //     size_t index;
    //     pool_append(field->regions, region, index);
    // }
}

int pw_field_regions_to_unload_count(PWField field){
    return (int)pool_get_count(field.regions) - (int)field.max_loaded_regions_count;
}


void pw_field_region_unload_last(PWField *field){
    int unload_count = pw_field_regions_to_unload_count(*field);

    
}

void pw_field_region_unload_far(PWField *field){
    int unload_count = pw_field_regions_to_unload_count(*field);


}

void pw_field_region_unload_lru(PWField *field){
    int unload_count = pw_field_regions_to_unload_count(*field);


}


/*********************************************/


void pw_rect_render(Image context, PWCamera2D camera, float x, float y, float w, float h, Color color){
    // printf("x:%.2f y:%.2f\n",x, y);
    // vec2 chunk_pos = pw_world_to_view((vec2f){x, y}, camera);
    Rect rect = {
        .x = (int)x, 
        .y = (int)y, 
        .w = (int)w, 
        .h = (int)h
    };
    rect = pw_world_to_view_rect(rect, camera);

    // CONSOLE_RECT(rect);
    pnt_draw_rect(context, rect, color, 1);
    // draw_filled_rect_f(window->context, rect, color);
}

void pw_chunk_render(Image context, PWCamera2D camera, PWChunk* chunk, float x, float y, Color color){
    Rect rect = {
        .x = (int)x, 
        .y = (int)y, 
        .w = (int)chunk->w, 
        .h = (int)chunk->h
    };
    rect = pw_world_to_view_rect(rect, camera);

    CONSOLE_RECT(rect);
    pnt_draw_rect(context, rect, color, 1);
    // draw_filled_rect_f(window->context, rect, color);
}

void pw_region_render(Image context, PWCamera2D camera, PWRegion region, size_t chunk_width, size_t chunk_height, bool show_chunks){
    Color red = {.rgba = 0xFF0000FF};
    Color purple = {.rgba = 0xFFFF00FF};

    // size_t field_width = region.field_width_in_chunks * field.chunk_width;
    // size_t field_height = region.field_height_in_chunks * field.chunk_height;

    // vec2 region_pos = pw_world_to_view((vec2f){(float)region.x, (float)region.y}, camera);
    Rect rect = {
        .x = region.x,
        .y = region.y,
        .w = region.width,
        .h = region.height
    };
    rect = pw_world_to_view_rect(rect, camera);
    // CONSOLE_RECT(rect);
    pnt_draw_rect(context, rect, red, 3);

    // Rect rect = {
    //     .x = region.x-(pw_chunk_coord_t)camera.pos.x, 
    //     .y = region.y-(pw_chunk_coord_t)camera.pos.y, 
    //     .w = region.width, .h = region.height
    // };
    // pnt_draw_rect(context, rect, red, 1);

    if(show_chunks){
        for(int i = 0; i < region.height_in_chunk; i++)
        for(int j = 0; j < region.width_in_chunk; j++){
            pw_rect_render(
                context, camera, 
                // &region.chunks[i*region.width_in_chunk + j], 
                (float)(region.x + j*(pw_chunk_coord_t)(chunk_width)), 
                (float)(region.y - i*(pw_chunk_coord_t)(chunk_height)),
                (float)(chunk_width),
                (float)(chunk_height),
                purple
            );
        }
    }
}

void pw_field_chunks_render(Image context, PWCamera2D camera, PWField field, bool show_chunks){
    Color blue = {.rgba = 0xFFFF0000};
    size_t field_width = field.field_width_in_chunks * field.chunk_width;
    size_t field_height = field.field_height_in_chunks * field.chunk_height;

    // vec2 field_pos = pw_world_to_view((vec2f){(float)field.x, (float)field.y}, camera);
    Rect rect = {
        .x = field.x,
        .y = field.y,
        .w = field.field_width_in_chunks*field.chunk_width,
        .h = field.field_height_in_chunks*field.chunk_height
    };
    rect = pw_world_to_view_rect(rect, camera);
    pnt_draw_rect(context, rect, blue, 3);


    if(!show_chunks) return;

    Color green = {.rgba = 0xFF00FF00};
    Color yellow = {.rgba = 0xFF00FFFF};
    bool row_toggle = FALSE;
    bool col_toggle = FALSE;
    for(int i = 0; i < field.field_height_in_chunks; i++){
        row_toggle = row_toggle ? FALSE : TRUE;
        col_toggle = row_toggle;
        for(int j = 0; j < field.field_width_in_chunks; j++){
            col_toggle = col_toggle ? FALSE : TRUE;
            Color color = col_toggle ? green : yellow;
            // printf("field_x:%d field_y:%d\n", field.x, field.y);
            // printf("x:%d y:%d\n", (field.x + j*field.chunk_width), (field.y + i*field.chunk_height));
            // printf("x:%.2f y:%.2f\n", (float)(field.x + j*(pw_chunk_coord_t)field.chunk_width), (float)(field.y + i*(pw_chunk_coord_t)field.chunk_height));
            pw_rect_render(
                context, camera, 
                // &field.chunks[i*field.field_width_in_chunks + j], 
                (float)(field.x + j*(pw_chunk_coord_t)field.chunk_width), 
                (float)(field.y - i*(pw_chunk_coord_t)field.chunk_height),
                field.chunk_width, field.chunk_height,
                color
            );
            // printf("times\n");
        }
    }
    // exit(0);
}

void pw_field_regions_render(Image context, PWCamera2D camera, PWField field, bool show_chunks){
    
    for(size_t i = 0; i < field.regions.elems.count; i++){
        // if(pool_is_deleted(field.regions, i)) continue;
        // printf("works\n");
        pw_region_render(context, camera, pool_get(field.regions, i), field.chunk_width, field.chunk_height, show_chunks);
    }
}


/*********************************************/