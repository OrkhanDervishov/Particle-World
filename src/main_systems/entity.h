#ifndef ENTITY_H
#define ENTITY_H

#include "chunk_space.h"
#include "constants.h"
#include "da.h"
#include "core.h"
#include "asset.h"

#define MAX_ENTITY_COUNT 4096

typedef size_t entity_id_t;



typedef struct{
    Rectf collider;
} RectCollider;


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

typedef struct{
    PWRenderable renderable;
    Posf pos;
    RectCollider collider;
} Entity;

typedef struct{
    Entity* items;
    size_t count;
    size_t capacity;
} Entities;

typedef struct{
    Entities elems;
    Indices free_indices;
    Slots slots;
} EntityPool;

entity_id_t entity_add(EntityPool* pool, Entity item);
void entity_delete(EntityPool* pool, entity_id_t index);
void entity_pool_print_stats(EntityPool* pool);

/************************************************************/


/************************************************************/
// Other


void draw_rect_collider(Image image, RectCollider rc, Color color);
void draw_rect_collider_f(FormatImage image, RectCollider rc, Color color);
bool collide_rect_to_particle(ChunkSpace* cs, RectCollider rc);
bool collide_rect_to_rect(RectCollider a, RectCollider b);

/************************************************************/

#endif