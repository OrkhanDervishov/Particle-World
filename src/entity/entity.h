#ifndef ENTITY_H
#define ENTITY_H

#include "engine_lib.h"
#include "chunk_space.h"
#include "da.h"

#define MAX_ENTITY_COUNT 4096

typedef size_t entity_id_t;


typedef struct{
    Image* sprite;
} Sprite;

typedef struct{
    Rectf collider;
} RectCollider;


/************************************************************/
// Storing entities as objects

/*
DOCUMENTATION:

To start the system init initialize an entity pool: POOL_INIT(ep);
*/

#define ENTITY_GET(pool, index) (pool).entities.items[(index)]
#define ENTITY_SET(pool, index, new) (pool).entities.items[(index)] = (new);
#define POOL_INIT(pool) pool = {0}

/*
TODO: Implement entity physics

typedef struct{
    vec2f velocity;
    vec2f acceleration;
} EntityPhysics;
*/

typedef struct{
    Posf pos;
    Sprite sprite;
    RectCollider collider;
} Entity;

typedef struct{
    Entity* items;
    size_t count;
    size_t capacity;
} Entities;

typedef struct{
    entity_id_t* items;
    size_t count;
    size_t capacity;
} Indices;

typedef struct{
    bool* items;
    size_t count;
    size_t capacity;
} Slots;

typedef struct{
    Entities entities;
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