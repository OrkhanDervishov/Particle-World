#include "chunk_system.h"


void pw_layers_prepare(PWLayers *layers){

    for(size_t i = 0; i < layers->count; i++){
        PWLayer *layer = &da_get(*layers, i);
        for(size_t j = 0; j < layer->sublayers.count; j++){
            PWSubLayer *sublayer = &da_get(layer->sublayers, j);
            da_free(sublayer->data);
            if(layer->type == PW_LAYER_GRID){
                da_any_reserve(sublayer->data, sublayer->item_size * layer->size);
            }
        }
    }
}

// PWLayers pw_layers_copy(PWLayers layers){
//     PWLayers res =

//     for(size_t i = 0; i < layers.count; i++){
//         PWLayer *layer = &da_get(layers, i);
//         layers
//         for(size_t j = 0; j < layer->sublayers.count; j++){
//             PWSubLayer *sublayer = &da_get(layer->sublayers, j);
//             sublayer->data = (DaAnyData){0};
//             if(layer->type == PW_LAYER_GRID){
//                 da_reserve(sublayer->data, sublayer->item_size * layer->size);
//             }
//         }
//     }
//     return layers;
// }

PWLayers pw_layers_copy(PWLayers layers){
    PWLayers result;
    da_zero(result);
    da_reserve(result, layers.count);

    for(size_t i = 0; i < layers.count; i++){
        PWLayer *src_layer = &da_get(layers, i);
        PWLayer dst_layer = *src_layer;
        
        PW_ASSERT(src_layer != NULL);
        da_zero(dst_layer.sublayers);
        da_zero(dst_layer.images);

        da_reserve(dst_layer.sublayers, src_layer->sublayers.count);
        
        for(size_t j = 0; j < src_layer->sublayers.count; j++){
            PWSubLayer *src_sub = &da_get(src_layer->sublayers, j);
            PWSubLayer dst_sub = *src_sub;
            
            PW_ASSERT(src_sub != NULL);
            da_any_init(dst_sub.data, src_sub->data.item_size);

            if(src_layer->type == PW_LAYER_GRID){
                da_any_reserve(dst_sub.data, src_sub->item_size * src_layer->size);
            }
            
            da_append(dst_layer.sublayers, dst_sub);
        }
        
        if(src_layer->type == PW_LAYER_GRID){
            for(size_t j = 0; j < src_layer->images.count; j++){
                Image image = {.buffer = NULL};
                pnt_create_image(&image, da_get(src_layer->images, j).width, da_get(src_layer->images, j).height);
                da_append(dst_layer.images, image);
            }
        }

        da_append(result, dst_layer);
    }
    
    return result;
}

void pw_layer_free(PWLayer *layer){
    if(layer == NULL) return;

    for(size_t i = 0; i < layer->sublayers.count; i++){
        da_free(da_get(layer->sublayers, i).data);
    }
    da_free(layer->sublayers);

    for(size_t i = 0; i < layer->images.count; i++){
        pnt_delete_image(&da_get(layer->images, i));
    }
    da_free(layer->images);
}

void pw_layers_free(PWLayers *layers){
    if(layers == NULL) return;
    for(size_t i = 0; i < layers->count; i++){
        pw_layer_free(&da_get(*layers, i));
    }

    da_free(*layers);
}

void* pw_layer_get(PWLayer layer, size_t x, size_t y){

}

//--------------------

PWLayer pw_layer_create(PWLayerType type, size_t item_size, size_t size, size_t width, size_t height){
    return (PWLayer){
        .id = -1,
        .type = type,
        .item_size = item_size,
        .size = type == PW_LAYER_GRID ? width*height : size,
        .width = width,
        .height = height
    };
}

void pw_layer_add_sublayer(PWLayer *layer, size_t item_size){
    PWSubLayer sublayer = {
        .item_size = item_size
    };
    
    da_any_init(sublayer.data, item_size);
    if(layer->type == PW_LAYER_GRID){
        da_any_reserve(sublayer.data, sublayer.item_size * layer->size);
    }

    da_append(layer->sublayers, sublayer);
}

void pw_layer_add_image(PWLayer *layer, size_t width, size_t height){
    Image image;
    image.buffer = NULL;
    if(pnt_create_image(&image, width, height)){
        printf("layer image was not created\n");
    }
    da_append(layer->images, image);
}

int pw_layer_sys_layer_add(
    PWLayerSystem *ls, PWLayer layer, PWLayerRenderer layer_renderer, PWLayerGenerator layer_generator
)
{
    layer.id = ls->registered_layer_count;
    da_append(ls->layers, layer);
    da_append(ls->layer_renderers, layer_renderer);
    da_append(ls->layer_generators, layer_generator);
    ls->registered_layer_count++;
    return 0;
}

void pw_layer_sys_info(PWLayerSystem *ls){
    printf("Layer System Info\n");

    for(size_t i = 0; i < ls->layers.count; i++){
        PWLayer layer = da_get(ls->layers, i);
        printf(
            "------------------\n"
            "Layer      %zu\n"
            "id         %zu\n"
            "type:      %s\n"
            "item size: %zu\n"
            "size:      %zu\n"
            "width:     %zu\n"
            "height:    %zu\n"
            "------------------\n",
            i, layer.id, layer.type == PW_LAYER_GRID ? "GRID" : "ENTITY", layer.item_size, layer.size, layer.width, layer.height
        );
    }
}

void pw_layer_sys_free(PWLayerSystem* ls){
    da_free(ls->layer_renderers);
    da_free(ls->layer_generators);
}


/*
    Generate all layers
*/
void pw_layers_generate(PWLayers *layers, PWLayerSystem ls, pw_chunk_coord_t x, pw_chunk_coord_t y){
    for(size_t i = 0; i < layers->count; i++){
        PWLayer *layer = &da_get(*layers, i);
        if(da_get(ls.layer_generators, layer->id) == NULL) continue;
        da_get(ls.layer_generators, layer->id)(layer, x, y);
    }
}

/*
    Render all layers
*/
void pw_layers_render(
    PWLayers layers, PWLayerSystem ls,         
    Image context, PWCamera2D camera, 
    pw_chunk_coord_t x, pw_chunk_coord_t y,
    size_t chunk_width, size_t chunk_height
){
    for(size_t i = 0; i < layers.count; i++){
        PWLayer layer = da_get(layers, i);
        if(da_get(ls.layer_renderers, layer.id) == NULL) continue;
        da_get(ls.layer_renderers, layer.id)(context, camera, layer, x, y, chunk_width, chunk_height);
    }
}

/*********************************************/

PWChunk pw_chunk_create(
    pw_chunk_coord_t x, pw_chunk_coord_t y,
    pw_chunk_coord_t w, pw_chunk_coord_t h,
    PWLayerSystem ls, bool generate
){
    PWChunk chunk = {
        .x = x, .y = y, .w = w, .h = h,
        .layers = pw_layers_copy(ls.layers)
    };
    if(generate){
        pw_layers_generate(&chunk.layers, ls, x/(pw_chunk_coord_t)chunk.w, y/(pw_chunk_coord_t)chunk.h);
    }

    return chunk;
}

void pw_chunk_delete(PWChunk *chunk){
    pw_layers_free(&chunk->layers);
}


/*********************************************/

const char* pw_gen_region_path(pw_chunk_coord_t x, pw_chunk_coord_t y){
    
    return NULL;
}

PWRegion pw_region_create(
    pw_chunk_coord_t x, pw_chunk_coord_t y, 
    size_t width_in_chunk, size_t height_in_chunk, 
    size_t chunk_width, size_t chunk_height,
    PWLayerSystem ls
){
    PWRegion region = {
        .x = x, .y = y, 
        .width_in_chunk = width_in_chunk, .height_in_chunk = height_in_chunk,
        .width = width_in_chunk*chunk_width, .height = height_in_chunk*chunk_height,
        .loaded = TRUE, .id = 0
    };
    region.chunks = (PWChunk*)malloc(region.width_in_chunk * region.height_in_chunk * sizeof(PWChunk));

    for(size_t i = 0; i < height_in_chunk; i++){
        for(size_t j = 0; j < width_in_chunk; j++){
            region.chunks[i*width_in_chunk + j] = pw_chunk_create(x + j*chunk_width, y - i*chunk_height, chunk_width, chunk_height, ls, TRUE);
        }
    }

    return region;
}

void pw_region_delete(PWRegion* region){
    for(size_t i = 0; i < region->width_in_chunk*region->height_in_chunk; i++){
        pw_chunk_delete(&region->chunks[i]);
    }
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
    size_t chunk_width, size_t chunk_height,
    PWLayerSystem ls
){
    PWRegion region;
    if(pw_region_load(&region, path)){
        region = pw_region_create(x, y, width_in_chunk, height_in_chunk, chunk_width, chunk_height, ls);
    }
    return region;
}

/*********************************************/



int pw_field_init(
    PWField *field, 
    size_t region_width_in_chunks, size_t region_height_in_chunks,
    size_t field_width_in_chunks, size_t field_height_in_chunks,
    size_t chunk_width, size_t chunk_height,
    size_t max_loaded_regions_count,
    PWLayerSystem ls
){
    if(!field) return 1;

    field->field_width_in_chunks = field_width_in_chunks;
    field->field_height_in_chunks = field_height_in_chunks;
    
    field->region_width_in_chunks = region_width_in_chunks;
    field->region_height_in_chunks = region_height_in_chunks;
    
    field->chunk_width = chunk_width;
    field->chunk_height = chunk_height;
    
    field->max_loaded_regions_count = max_loaded_regions_count;
    field->loading_algo = PW_FIELD_LOAD_LAST;

    field->ls = ls;

    if(field->chunks != NULL){
        free(field->chunks);
    }
    
    ll_init(field->region_list);

    field->chunks = (PWChunk**)calloc(
        field->field_width_in_chunks * field->field_height_in_chunks, 
        sizeof(PWChunk*)
    );

    return 0;
}

void pw_field_destroy(PWField *field){
    // pool_free(field->regions);
    LL_TYPEOF(field->region_list) curr_region = field->region_list.head;
    while(curr_region){
        pw_region_delete(&curr_region->value);
        curr_region = curr_region->next;
    }
    ll_free(field->region_list);
    free(field->chunks);
}

void pw_field_chunk_organize(PWField field){
    // Organize chunks from region list
    for(size_t i = 0; i < field.field_height_in_chunks; i++)
    for(size_t j = 0; j < field.field_width_in_chunks; j++){
        pw_chunk_coord_t cx = field.x + j * field.chunk_width;
        pw_chunk_coord_t cy = field.y - i * field.chunk_height;

        size_t region_width = field.region_width_in_chunks * field.chunk_width;
        size_t region_height = field.region_height_in_chunks * field.chunk_height;
        pw_chunk_coord_t rx = (cx / region_width) * region_width;
        pw_chunk_coord_t ry = (cy / region_height) * region_height;

        PWChunk* chunk;

        LL_TYPEOF(field.region_list) curr_region = field.region_list.head;
        while(curr_region){

            if(curr_region->value.x == rx && curr_region->value.y == ry){
                size_t ri = (ry - cy) / field.chunk_height;
                size_t rj = (cx - rx) / field.chunk_width;

                size_t index = ri * (field.region_width_in_chunks) + rj;
                chunk = &curr_region->value.chunks[index];
                break;
            }
            curr_region = curr_region->next;
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
        is_changed = TRUE;
    }

    field->x_center = (x / field->chunk_width) * field->chunk_width; 
    field->y_center = (y / field->chunk_height + 1) * field->chunk_height;
    field->x = field->x_center - (field->field_width_in_chunks/2) * field->chunk_width;
    field->y = field->y_center + (field->field_height_in_chunks/2) * field->chunk_height;

    // 2 - check if all required regions are loaded

    if(is_changed){
        pw_field_regions_load(field, x, y);
        
        // 4 - organize chunks in field
        pw_field_chunk_organize(*field);
    }
}


static pw_chunk_coord_t ceil_div_mul(pw_chunk_coord_t v, pw_chunk_coord_t step){
    pw_chunk_coord_t q = v / step;
    pw_chunk_coord_t r = v % step;
    if(r != 0 && v > 0) q += 1;
    // else if(r != 0 && v < 0)
    return q * step;
}

vec2_chunk_coord pw_field_coord_region(PWField field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    size_t region_width = field.region_width_in_chunks * field.chunk_width;
    size_t region_height = field.region_height_in_chunks * field.chunk_height;

    return (vec2_chunk_coord){
        (x / (pw_chunk_coord_t)region_width + (x < 0 ? -1 : 0)) * (pw_chunk_coord_t)region_width,
        ceil_div_mul(y, (pw_chunk_coord_t)region_height)
        // (y / (pw_chunk_coord_t)region_height + (y < 0 ? 0 : 1)) * (pw_chunk_coord_t)region_height
    };
}

vec2_chunk_coord pw_field_get_region(PWField field){
    return pw_field_coord_region(field, field.x_center, field.y_center);
}

bool pw_field_region_is_loaded(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y, size_t *index){
    bool is_loaded = FALSE;
    LL_TYPEOF(field->region_list) curr_region = field->region_list.head;

    size_t i = 0;
    while(curr_region){
        if(curr_region->value.x == x && curr_region->value.y == y){
            is_loaded = TRUE;
            break;
        }
        curr_region = curr_region->next;
        i++;
    }
    
    if(is_loaded && index != NULL) *index = i;
    return is_loaded;
}


void pw_field_regions_load(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    /*
    If region is stored - load region
    If region is not generated - generate
    */
    // TODO: need region file naming convention

    pw_chunk_coord_t field_endx = field->x + (pw_chunk_coord_t)(field->chunk_width * (field->field_width_in_chunks));
    pw_chunk_coord_t field_endy = field->y - (pw_chunk_coord_t)(field->chunk_height * (field->field_height_in_chunks));

    vec2_chunk_coord region_start_pos = pw_field_coord_region(*field, field->x, field->y);
    vec2_chunk_coord region_end_pos = pw_field_coord_region(*field, field_endx, field_endy);

    pw_chunk_coord_t start_rx = region_start_pos.x;
    pw_chunk_coord_t start_ry = region_start_pos.y;
    pw_chunk_coord_t end_rx = region_end_pos.x;
    pw_chunk_coord_t end_ry = region_end_pos.y;

    for(pw_chunk_coord_t i = start_ry; i >= end_ry; i -= field->region_height_in_chunks*field->chunk_height){
        for(pw_chunk_coord_t j = start_rx; j <= end_rx; j += field->region_width_in_chunks*field->chunk_width){
            pw_field_region_load(field, j, i);   
        }
    }
}





/*************************************************/
// Region Load/Unload algorithms
// TODO: Store regions before deleteing

int pw_field_regions_to_unload_count(PWField field){
    return (int)field.region_list.count - (int)field.max_loaded_regions_count;
}


void pw_field_region_unload_last(PWField *field){
    int unload_count = pw_field_regions_to_unload_count(*field) + 1;
    if(unload_count <= 0) return;
    // if(field->region_list.count < field->max_loaded_regions_count) return;
    // if(field->region_list.count == 0) return;
    for(int i = 0; i < unload_count; i++){
        pw_region_delete(&field->region_list.head->value);
        ll_pop_head(field->region_list);
    }
}

void pw_field_region_load_last(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    if(pw_field_region_is_loaded(field, x, y, NULL)) return;

    pw_field_region_unload_last(field);

    PWRegion region = pw_region_create_or_load(
        pw_gen_region_path(x, y), 
        x, y, 
        field->region_width_in_chunks,
        field->region_height_in_chunks,
        field->chunk_width,
        field->chunk_height,
        field->ls
    );
    ll_push_tail(field->region_list, region);
}


#define SQUARE_DISTANCE(x1, y1, x2, y2) ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
/*
    This function stores and deletes all regions that are to far from the field.
    TODO: Consider field width and height
*/
void pw_field_region_unload_far(PWField *field){
    int unload_count = pw_field_regions_to_unload_count(*field) + 1;
    if(unload_count <= 0) return;

    pw_chunk_coord_t field_width = (pw_chunk_coord_t)(field->chunk_width * field->field_width_in_chunks);
    pw_chunk_coord_t field_height = (pw_chunk_coord_t)(field->chunk_height * field->field_height_in_chunks);
    for(int i = 0; i < unload_count; i++){
        pw_chunk_coord_t max_dist = -1.0f;
        LL_TYPEOF(field->region_list) curr_region = field->region_list.head;
        LL_TYPEOF(field->region_list) region_for_delete = NULL;
        while(curr_region){
            // Sum of distances from 4 corners of field to current region center
            pw_chunk_coord_t region_center_x = curr_region->value.x + field->region_width_in_chunks*field->chunk_width/2;
            pw_chunk_coord_t region_center_y = curr_region->value.y + field->region_height_in_chunks*field->chunk_height/2;
            float dist = 
            SQUARE_DISTANCE(field->x, field->y, region_center_x, region_center_y) + 
            SQUARE_DISTANCE(field->x + field_width, field->y, region_center_x, region_center_y) + 
            SQUARE_DISTANCE(field->x, field->y + field_height, region_center_x, region_center_y) + 
            SQUARE_DISTANCE(field->x + field_width, field->y + field_height, region_center_x, region_center_y);

            if(dist > max_dist){
                max_dist = dist;
                region_for_delete = curr_region;
            }
            curr_region = curr_region->next;
        }
        pw_region_delete(&region_for_delete->value);
        ll_pop_node(field->region_list, region_for_delete);
    }
}


void pw_field_region_load_far(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    if(pw_field_region_is_loaded(field, x, y, NULL)) return;

    pw_field_region_unload_far(field);

    PWRegion region = pw_region_create_or_load(
        pw_gen_region_path(x, y), 
        x, y, 
        field->region_width_in_chunks,
        field->region_height_in_chunks,
        field->chunk_width,
        field->chunk_height,
        field->ls
    );
    ll_push_tail(field->region_list, region);
}

void pw_field_region_load_lru(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    size_t region_index;
    if(pw_field_region_is_loaded(field, x, y, &region_index)){

        LL_TYPEOF(field->region_list) node = NULL;
        ll_get_node_by_index(field->region_list, node, region_index);
        PWRegion region = node->value;
        ll_pop_node(field->region_list, node);
        ll_push_tail(field->region_list, region);
    } 
    else {
        PWRegion region = pw_region_create_or_load(
            pw_gen_region_path(x, y), 
            x, y, 
            field->region_width_in_chunks,
            field->region_height_in_chunks,
            field->chunk_width,
            field->chunk_height,
            field->ls
        );

        if(field->region_list.count >= field->max_loaded_regions_count)
            pw_field_region_unload_last(field);
        ll_push_tail(field->region_list, region);
    }
}

void pw_field_region_load(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y){
    if(field->max_loaded_regions_count <= 0) return;
    switch(field->loading_algo){
        case PW_FIELD_LOAD_LAST:
            pw_field_region_load_last(field, x, y);
            break;
        case PW_FIELD_LOAD_FAR:
            pw_field_region_load_far(field, x, y);
            break;
        case PW_FIELD_LOAD_LRU:
            pw_field_region_load_lru(field, x, y);
            break;
    }
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

void pw_chunk_render(Image context, PWCamera2D camera, PWField field, PWChunk* chunk, float x, float y, Color color){
    if(chunk == NULL) return;
    Rect rect = {
        .x = (int)x, 
        .y = (int)y, 
        .w = (int)chunk->w, 
        .h = (int)chunk->h
    };
    rect = pw_world_to_view_rect(rect, camera);
    pw_chunk_coord_t coord_x = (pw_chunk_coord_t)x;
    pw_chunk_coord_t coord_y = (pw_chunk_coord_t)y;
    
    pw_layers_render(chunk->layers, field.ls, context, camera, coord_x, coord_y, field.chunk_width, field.chunk_height);
    pnt_draw_rect(context, rect, color, 1);
}

void pw_region_render(Image context, PWCamera2D camera, PWField field, PWRegion region, size_t chunk_width, size_t chunk_height, bool show_chunks){
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
        // for(int i = region.height_in_chunk - 1; i >= 0; i--)
        for(int i = 0; i < region.height_in_chunk; i++)
        for(int j = 0; j < region.width_in_chunk; j++){
            pw_chunk_render(
                context, camera, field,
                &region.chunks[i*region.width_in_chunk + j], 
                (float)(region.x + j*(pw_chunk_coord_t)(chunk_width)), 
                (float)(region.y - i*(pw_chunk_coord_t)(chunk_height)),
                purple
            );
            // pw_rect_render(
            //     context, camera, 
            //     // &region.chunks[i*region.width_in_chunk + j], 
            //     (float)(region.x + j*(pw_chunk_coord_t)(chunk_width)), 
            //     (float)(region.y - i*(pw_chunk_coord_t)(chunk_height)),
            //     (float)(chunk_width),
            //     (float)(chunk_height),
            //     purple
            // );
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
    
    LL_TYPEOF(field.region_list) curr_region = field.region_list.head;
    while(curr_region){
        // if(pool_is_deleted(field.regions, i)) continue;
        // printf("works\n");
        pw_region_render(context, camera, field, curr_region->value, field.chunk_width, field.chunk_height, show_chunks);
        curr_region = curr_region->next;
    }
}


/*********************************************/




/*
    TODO 1: Create a better dynamic array. [COMPLETED]
    TODO 2: Fix crash that occurs when region or chunk is destroyed. [COMPLETED]
    TODO 3: Review all chunk system code for memory leaks. []
    TODO 4: Improve rendering system for layers. []
*/