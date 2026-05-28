#ifndef ASSET_LOADER_H_
#define ASSET_LOADER_H_

#include "da.h"
#include "painter.h"

typedef size_t asset_id_t;


// ASSET TYPE
/************************************/
typedef enum{
    IMAGE = 0,
    SPRITE,
    ANIMATION,
    AUDIO,
    SOUND_EFFECT,
    VISUAL_EFFECT
} AssetType;


typedef struct{
    Image image;
} AssetImage;

typedef struct{
    Image image;
} AssetSprite;

typedef struct{
    int rows;
    int cols;
    int offset_x;
    int offset_y;
    Image image;
} ArrayImage;

typedef struct{
    ArrayImage* frame_images;
    int* frame_order;
    float* delays;
} Animation;

typedef struct{
    AssetType type;
    union{
        AssetImage  image;
        AssetSprite sprite;
        ArrayImage  arr_image;
        Animation   anim;
    };
} Asset;
/************************************/


// ASSET LOADER
/************************************/


typedef struct{
    Asset* items; 
    size_t count;
    size_t capacity;
} Assets;

typedef struct{
    asset_id_t* items;
    size_t count;
    size_t capacity;
} AssetIndices;


typedef struct{
    Assets items;
    AssetIndices free_indices;
    Slots slots;
} AssetPool;


/************************************/
#endif //ASSET_LOADER_H_
