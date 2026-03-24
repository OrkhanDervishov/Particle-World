#ifndef ENTITY_H
#define ENTITY_H

#include "engine_lib.h"

#define MAX_ENTITY_COUNT 4096

typedef uint16_t Entity;

typedef struct{
    Image sprite;
} Sprite;

typedef struct{
    Rect collider;
} Collider;



#endif