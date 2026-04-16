// #ifndef ASSETS_H
// #define ASSETS_H

// #include "engine_lib.h"

// typedef enum{
//     IMAGE = 0,
//     SPRITE,
//     ANIMATION,
//     AUDIO,
//     SOUND_EFFECT,
//     VISUAL_EFFECT
// } AssetType;

// typedef struct{
//     AssetType type;
//     void* source;
// } Asset;

// typedef struct{
//     Image image;
// } AssetImage;

// typedef struct{
//     Image image;
// } AssetSprite;

// typedef struct{
//     int rows;
//     int cols;
//     int offset_x;
//     int offset_y;
//     Image image;
// } ArrayImage;

// typedef struct{
//     ArrayImage* frame_images;
//     int* frame_order;
//     float* delays;
// } Animation;

// int load_asset(AssetImage* image, const char* path);
// int asset_to_array_image(ArrayImage* arr_image, AssetImage image, int offset_x, int offset_y);
// void array_image_to_anim(Animation* anim, ArrayImage arr_image);
// void anim_add_order(Animation* anim);
// void anim_add_delays(Animation* anim);
// #endif