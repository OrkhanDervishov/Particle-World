#include "entity.h"




/************************************************************/
// Storing entities as objects

entity_id_t entity_add(EntityPool* pool, Entity item){
    if(pool->free_indices.count > 0){
        entity_id_t index = da_back(pool->free_indices);
        pool->items.items[index] = item;
        pool->slots.items[index] = TRUE;
        pool->free_indices.count--;
        return index;
    }
    da_append(pool->items, item);
    da_append(pool->slots, TRUE);
    return pool->items.count-1;
}

void entity_delete(EntityPool* pool, entity_id_t index){
    if(index >= pool->items.count || pool->slots.items[index] == FALSE) return;
    pool->slots.items[index] = FALSE;
    da_append(pool->free_indices, index);
}

void entity_pool_print_stats(EntityPool* pool){
    printf("Entity count: %zu\n", pool->items.count - pool->free_indices.count);
}   

/************************************************************/












/************************************************************/
// Other

void draw_rect_collider(Image image, RectCollider rc, Color color){
    Rect rect = {
        (int)rc.collider.x,
        (int)rc.collider.y,
        (int)rc.collider.w,
        (int)rc.collider.h
    };
    draw_rect(image, rect, color, 1);
}

void draw_rect_collider_f(FormatImage image, RectCollider rc, Color color){
    Rect rect = {
        (int)rc.collider.x,
        (int)rc.collider.y,
        (int)rc.collider.w,
        (int)rc.collider.h
    };
    draw_rect_f(image, rect, color, 1);
}

bool collide_rect_to_particle(ChunkSpace* cs, RectCollider rc){
    Rectf rect = rc.collider;
    Rect fit_rect = {
        (int)rect.x/DEFAULT_PARTICLE_SIZE,
        (int)rect.y/DEFAULT_PARTICLE_SIZE,
        (int)rect.w/DEFAULT_PARTICLE_SIZE,
        (int)rect.h/DEFAULT_PARTICLE_SIZE
    };
    
    // int i = fit_rect.y + fit_rect.h;
    for(int i = fit_rect.y + fit_rect.h-2; i < fit_rect.y + fit_rect.h+1; i++)
    for(int j = fit_rect.x; j < fit_rect.x + fit_rect.w; j++){
        if(!CS_P_CHECK_BOUNDS(cs, j, i)) continue;
        uint32_t flags = typeFlagsList[CS_GET_TYPE(cs, j, i)];
        if(CHECK_FLAG(flags, IS_DUST) || CHECK_FLAG(flags, IS_SOLID)){
            return TRUE;
        }
    }
    return FALSE;
}

bool collide_rect_to_rect(RectCollider a, RectCollider b){
    float a_endx = a.collider.x + a.collider.w;
    float a_endy = a.collider.y + a.collider.h;
    float b_endx = b.collider.x + b.collider.w;
    float b_endy = b.collider.y + b.collider.h;
    
    if(
        a.collider.x < b_endx && a_endx > b.collider.x && 
        a.collider.y < b_endy && a_endy > b.collider.y
    ){
        return TRUE;
    }
    
    return FALSE;
}
/************************************************************/