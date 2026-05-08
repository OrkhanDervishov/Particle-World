#ifndef ENTITY_H
#define ENTITY_H

#include "engine_lib.h"
#include "chunk_space.h"

#define MAX_ENTITY_COUNT 4096

typedef uint16_t Entity;


typedef struct{
    Image* sprite;
} Sprite;

typedef struct{
    Rectf collider;
} RectCollider;

typedef struct{
    Sprite sprite;
    RectCollider collider;
} EntityObj;

void draw_rect_collider(Image image, RectCollider rc, Color color);
void draw_rect_collider_f(FormatImage image, RectCollider rc, Color color);
bool collide_rect_to_particle(ChunkSpace* cs, RectCollider rc);
bool collide_rect_to_rect(RectCollider a, RectCollider b);

#endif