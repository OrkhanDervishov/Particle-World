#include "entity.h"


int pw_entity_manager_init(PWEntityManager* em){
    *em = (PWEntityManager){0};
}

int pw_entity_manager_free(PWEntityManager* em){
    pool_free(em->pool);
}

/************************************************************/
// Storing entities as objects

pw_entity_id_t pw_entity_manager_add(PWEntityManager* em, PWEntity item){
    pw_entity_id_t entity_id;
    pool_append(em->pool, item, entity_id);
    return entity_id;
}

void pw_entity_manager_delete(PWEntityManager* em, pw_entity_id_t index){
    pool_delete(em->pool, index);
}

PWEntity* pw_entity_manager_get(PWEntityManager* em, pw_entity_id_t id){
    return &pool_get(em->pool, id);
}

void pw_entity_manager_print_stats(PWEntityManager* em){
    printf("Entity count: %zu\n", pool_get_count(em->pool));
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
    pnt_draw_rect(image, rect, color, 1);
}

void draw_rect_aabb_collider(Image image, vec2f pos, PWColliderAABB aabb, Color color){
    Rect rect = {
        (int)pos.x,
        (int)pos.y,
        (int)aabb.w,
        (int)aabb.h
    };
    pnt_draw_rect(image, rect, color, 1);
    // pnt_draw_filled_rect(image, rect, color);
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




bool are_colliding_aabb(PWEntity e1, PWEntity e2){

    float a_sx = e1.pos.x + e1.aabb.x;
    float a_sy = e1.pos.y + e1.aabb.y;
    float b_sx = e2.pos.x + e2.aabb.x;
    float b_sy = e2.pos.y + e2.aabb.y;
    
    float a_endx = e1.pos.x + e1.aabb.x + e1.aabb.w;
    float a_endy = e1.pos.y + e1.aabb.y + e1.aabb.h;
    float b_endx = e2.pos.x + e2.aabb.x + e2.aabb.w;
    float b_endy = e2.pos.y + e2.aabb.y + e2.aabb.h;
    
    if(
        a_sx < b_endx && a_endx > b_sx && 
        a_sy < b_endy && a_endy > b_sy
    ){
        return TRUE;
    }
    
    return FALSE;
}
/************************************************************/