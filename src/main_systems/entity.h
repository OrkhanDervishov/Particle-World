#ifndef ENTITY_H
#define ENTITY_H

#include "chunk_space.h"
#include "constants.h"
#include "da.h"
#include "core.h"
#include "asset.h"

#define MAX_ENTITY_COUNT 4096

typedef size_t pw_entity_id_t;



typedef struct{
    Rectf collider;
} RectCollider;

typedef Rectf PWColliderAABB;


/************************************************************/
// Storing entities as objects

/*
DOCUMENTATION:

To start the system init initialize an entity pool: POOL_INIT(ep);
*/

#define ENTITY_GET(pool, index) pool_get(pool, index)
#define ENTITY_SET(pool, index, new) pool_get(pool, new, index)
#define ENTITY_IS_DELETED(pool, index) pool_is_deleted(pool, index)
#define POOL_INIT(pool) (pool) = (EntityPool){0}

/*
TODO: Implement entity physics

typedef struct{
    vec2f velocity;
    vec2f acceleration;
} EntityPhysics;
*/

typedef enum{
    PW_ENTITY_STATIC,
    PW_ENTITY_DYNAMIC
} PWEntityType;

typedef struct{
    PWEntityType type;

    Posf pos;
    PWRenderable renderable;

    RectCollider collider;
    PWColliderAABB aabb;
} PWEntity;
typedef PWEntity Entity;

typedef struct{
    PWEntity* items;
    size_t count;
    size_t capacity;
} PWEntities;

typedef struct{
    PWEntities elems;
    Indices free_indices;
    Slots slots;
} PWEntityPool;

typedef struct{
    PWEntityPool pool;
} PWEntityManager;
typedef PWEntityManager EntityManager;

int pw_entity_manager_init(PWEntityManager* em);
int pw_entity_manager_free(PWEntityManager* em);

pw_entity_id_t pw_entity_manager_add(PWEntityManager* em, PWEntity item);
void pw_entity_manager_delete(PWEntityManager* em, pw_entity_id_t index);
PWEntity* pw_entity_manager_get(PWEntityManager* em, pw_entity_id_t id);
void pw_entity_manager_pool_print_stats(PWEntityManager* em);

/************************************************************/


/************************************************************/
// Other

void draw_rect_aabb_collider(Image image, vec2f pos, PWColliderAABB aabb, Color color);
void draw_rect_collider(Image image, RectCollider rc, Color color);
void draw_rect_collider_f(FormatImage image, RectCollider rc, Color color);
bool collide_rect_to_particle(ChunkSpace* cs, RectCollider rc);
bool collide_rect_to_rect(RectCollider a, RectCollider b);

int are_colliding_aabb(PWEntity e1, PWEntity e2);

/************************************************************/

#endif