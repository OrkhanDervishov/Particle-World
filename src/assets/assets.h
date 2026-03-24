#ifndef ASSETS_H
#define ASSETS_H

#include "engine_lib.h"

typedef enum{
    IMAGE = 0,
    SPRITE,
    ANIMATION,
    AUDIO,
    SOUND_EFFECT,
    VISUAL_EFFECT
} AssetType;

typedef struct{
    AssetType type;
    void* source;
} Asset;

typedef struct{
    Image image;
} AssetImage;

typedef struct{
    Image image;
} AssetSprite;


#endif