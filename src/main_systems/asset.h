#ifndef ASSET_H_
#define ASSET_H_

#include "da.h"
#include "ht.h"
#include "painter.h"
#include "timing.h"

#define ERROR_ASSET_ID 0
#define ERROR_IMAGE_ID 0

typedef size_t pw_image_t;
typedef size_t pw_asset_t;
typedef uint16_t pw_anim_frame_t;

typedef enum{
    // PW_ASSET_IMAGE = 0,
    PW_SPRITE = 0,
    PW_ARRAY_IMAGE,
    PW_SPRITE_ANIMATION,
    PW_SPRITE_ANIMATOR,

    PW_AUDIO,
    PW_SOUND_EFFECT,
    PW_VISUAL_EFFECT
} PWAssetType;

// typedef struct{
//     pw_image_t image_id;
//     int rows;
//     int cols;
//     int offset_x;
//     int offset_y;
// } PWArrayImage;

typedef struct{
    pw_image_t image_id;

    bool multiple;
    struct{
        int startx, starty;
        int rows, cols;
        int offsetx, offsety;
    };
} PWSprite;


typedef struct{
    pw_anim_frame_t* items;
    size_t count;
    size_t capacity;
} PWFrames;

typedef struct{
    pw_asset_t frame_images;

    PWFrames frame_order;
    PWTimes delays;
    pw_time_t lasts;
} PWSpriteAnimation;

typedef struct{
    pw_asset_t animation;
    
    pw_anim_frame_t current_frame;
    pw_time_t timer;
    bool playing;
    bool looping;
} PWSpriteAnimator;

typedef struct{
    PWAssetType type;
    union{
        // PWArrayImage asset_aimage;
        // PWAssetImage asset_image;
        PWSprite sprite;
        PWSpriteAnimation sprite_animation;
        PWSpriteAnimator sprite_animator;
    };
} PWAsset;

typedef struct{
    Image *items;
    size_t count;
    size_t capacity;
} Images;

typedef struct{
    PWAsset *items;
    size_t count;
    size_t capacity;
} PWAssets;

typedef struct{
    Images elems;
    Indices free_indices;
    Slots slots;
} PWAssetImagePool;

typedef struct{
    PWAssets elems;
    Indices free_indices;
    Slots slots;
} PWAssetPool;

typedef struct{
    // File loading
    // Hash tables for storing resource parths and their indices
    Ht(char*, pw_image_t) loaded_images;

    // Storing resources
    // Pools for storing resources
    PWAssetImagePool image_pool;
    // Pool for storing assets
    PWAssetPool asset_pool;

    

} PWAssetManager;

#define LOAD_ASSET(asset_loader, asset, type)\


int pw_asset_manager_init(PWAssetManager* am);
PWAssetType get_asset_type(PWAssetManager* am, pw_asset_t asset_id);

pw_asset_t pw_load_asset(PWAssetManager* am, const char* path, PWAssetType type);

void pw_make_asset_image_multiple_auto(PWAssetManager* am, pw_asset_t asset_id, vec2 rows_cols);
Image pw_sprite_multiple_get_image(PWAssetManager* am, pw_asset_t asset_id, int index);

pw_asset_t pw_sprite_animation_create_load(PWAssetManager* am, pw_asset_t frame_images, PWFrames frame_order, PWTimes delays);
int pw_sprite_animation_set_frame_images(PWAssetManager* am, pw_asset_t anim_id, pw_asset_t frame_images);
int pw_sprite_animation_set_frames(PWAssetManager* am, pw_asset_t anim_id, PWFrames frames);
int pw_sprite_animation_set_delays(PWAssetManager* am, pw_asset_t anim_id, PWTimes delays);

pw_asset_t pw_sprite_animator_create_load(PWAssetManager* am, pw_asset_t animation, bool playing, bool looping);
int pw_sprite_animator_set_playing(PWAssetManager* am, pw_asset_t animator_id, bool playing);
int pw_sprite_animator_set_looping(PWAssetManager* am, pw_asset_t animator_id, bool looping);

void pw_sprite_animator_update(PWAssetManager* am, pw_asset_t animator_id, pw_time_t delta_time);

// int asset_to_array_image(ArrayImage* arr_image, AssetImage image, int offset_x, int offset_y);
// void array_image_to_anim(Animation* anim, ArrayImage arr_image);
// void anim_add_order(Animation* anim);
// void anim_add_delays(Animation* anim);

#endif //ASSET_H_