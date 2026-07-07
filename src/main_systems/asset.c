#include <stdarg.h>
#include "asset.h"
#include "da.h"
#include "la.h"

#define HT_IMPLEMENTATION
#include "ht.h"

PWAsset* pw_asset_manager_get_asset(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset *asset = &pool_get(am->asset_pool, asset_id);
    return asset;
}

Image* pw_asset_manager_get_image(PWAssetManager* am, pw_image_t image_id){
    Image *image = &pool_get(am->image_pool, image_id);
    return image;
}

inline Image pw_image_get_frame(Image img, PWSprite sprite, pw_anim_frame_t frame_index){
    vec2ui frame_coords = {frame_index % sprite.cols, frame_index / sprite.cols};
    vec2ui img_coords = {frame_coords.x * sprite.offsetx, frame_coords.y * sprite.offsety};

    Image frame_image = {
        .buffer = img.buffer + img.width * img_coords.y + img_coords.x,
        .width = sprite.offsetx,
        .height = sprite.offsety,
    };

    return frame_image;
}

void pw_make_asset_image_multiple(PWAssetManager* am, pw_asset_t asset_id, vec2 start, vec2 offset, vec2 rows_cols){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return;
    }

    PWSprite sprite = asset.sprite;
    Image image = pool_get(am->image_pool, sprite.image_id);
    sprite.multiple = TRUE;
    
    // TODO: Ensure all values are valid
    sprite.startx = start.x >= 0 && start.x < image.width ? start.x : 0;
    sprite.starty = start.y >= 0 && start.y < image.height ? start.y : 0;
    sprite.offsetx = offset.x;
    sprite.offsety = offset.y;
    sprite.rows = rows_cols.x;
    sprite.cols = rows_cols.y;
}

void pw_make_asset_image_multiple_auto(PWAssetManager* am, pw_asset_t asset_id, vec2 rows_cols){
    PWAsset *asset = &pool_get(am->asset_pool, asset_id);
    if(asset->type != PW_ASSET_SPRITE){
        return;
    }

    PWSprite* sprite = &(asset->sprite);
    Image image = pool_get(am->image_pool, sprite->image_id);
    sprite->multiple = TRUE;
    
    sprite->startx = 0;
    sprite->starty = 0;
    sprite->offsetx = image.width/rows_cols.y;
    sprite->offsety = image.height/rows_cols.x;
    sprite->rows = rows_cols.x;
    sprite->cols = rows_cols.y;
}

Image pw_sprite_multiple_get_image(PWAssetManager* am, pw_asset_t asset_id, int index){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return (Image){0};
    }
    PWSprite sprite = asset.sprite;
    Image sprite_image = pool_get(am->image_pool, sprite.image_id);
    
    // printf("offsetx: %d\n", sprite.offsetx);
    // printf("offsety: %d\n", sprite.offsety);
    // printf("rows: %d\n",    sprite.rows);
    // printf("cols: %d\n",    sprite.cols);


    int x = index % sprite.cols;
    int y = index / sprite.cols;
    int xi = x*sprite.offsetx;
    int yi = y*sprite.offsety;
    int offset = (yi*sprite_image.width) + (xi);
    
    Image part = {
        .buffer = sprite_image.buffer + offset,
        .width  = sprite.offsetx,
        .height = sprite.offsety,
        .stride = sprite_image.width
    };

    return part;
}

pw_image_t pw_load_asset_image(PWAssetManager* am, const char* path){
    pw_image_t index = ERROR_IMAGE_ID;
    pw_image_t *index_p = ht_find(&(am->loaded_images), path);
    if(index_p == NULL){
        Image image = {0};
        pnt_load_image(&image, path);
        pool_append(am->image_pool, image, index);
        *ht_put(&(am->loaded_images), path) = index;
    } else {
        index = *index_p;
    }
    return index;
}

PWAsset pw_load_sprite(PWAssetManager* am, const char* path){
    PWAsset asset;
    PWSprite image_asset;
    image_asset.image_id = pw_load_asset_image(am, path);

    asset.type = PW_ASSET_SPRITE;
    asset.sprite = image_asset;

    return asset;
}



// SPRITE ANIMATION
/*
    If frame_order or delay are not defined yet, they can be set to NULL
*/
PWSpriteAnimation pw_sprite_animation_create(pw_asset_t frame_images, PWFrames frames, PWTimes delays){
    pw_time_t lasts = 0.0f;
    for(size_t i = 0; i < delays.count; i++){
        lasts += delays.items[i];
    }

    PWSpriteAnimation anim = {
        .frame_images = frame_images,
        .lasts = lasts
    };
    da_copy(anim.frame_order, frames);
    da_copy(anim.delays, delays);

    return anim;
}

PWSpriteAnimation pw_sprite_animation_set_frames_args(PWSpriteAnimation anim, int n, ...){
    va_list args;
    va_start(args, n);
    anim.frame_order.count = (size_t)n;
    for(int i = 0; i < n; i++){
        da_set(anim.frame_order, i, (pw_anim_frame_t)va_arg(args, int));
    }
    va_end(args);
    return anim;
}

PWSpriteAnimation pw_sprite_animation_set_delays_args(PWSpriteAnimation anim, int n, ...){
    va_list args;
    va_start(args, n);
    anim.delays.count = (size_t)n;
    for(int i = 0; i < n; i++){
        da_set(anim.delays, i, (pw_time_t)va_arg(args, double));
    }
    va_end(args);
    return anim;
}


// SPRITE ANIMATOR

PWSpriteAnimator pw_sprite_animator_create(
    PWAssetManager* am, pw_asset_t animation, pw_anim_frame_t current_frame, 
    pw_time_t timer, bool playing, bool looping
){
    PWSpriteAnimator animator = {
        .animation = animation,
        .current_frame = current_frame,
        .timer = timer,
        playing = playing,
        .looping = looping
    };

    return animator;
}

// User Interface
/************************************************************/

int pw_asset_manager_init(PWAssetManager* am){
    am->asset_pool = (PWAssetPool){0};
    am->image_pool = (PWAssetImagePool){0};
    size_t dummy;
    pool_append(am->asset_pool, (PWAsset){0}, dummy);
    pool_append(am->image_pool, (Image){0}, dummy);
    am->loaded_images.hasheq = ht_cstr_hasheq;
    return 0;
}

void pw_asset_manager_free(PWAssetManager* am){
    pool_free(am->asset_pool);
    pool_free(am->image_pool);
    ht_free(&am->loaded_images);
}


pw_asset_t pw_load_asset(PWAssetManager* am, const char* path, PWAssetType type){
    pw_asset_t asset_id;

    PWAsset asset;
    switch(type){
        case PW_ASSET_SPRITE:
        {
            asset = pw_load_sprite(am, path);
            if(asset.sprite.image_id == ERROR_IMAGE_ID){
                printf("ERROR: Could not load asset - %s", path);
                return ERROR_ASSET_ID;
            }
        } break;
        case PW_ASSET_SPRITE_ANIMATION:
        {

        } break;

        default:
            printf("ERROR: undefined asset type - %s", path);
            return ERROR_ASSET_ID;
    }

    pool_append(am->asset_pool, asset, asset_id);
    return asset_id;
}


// SPRITE ANIMATION

pw_asset_t pw_sprite_animation_create_load(PWAssetManager* am, pw_asset_t frame_images, PWFrames frame_order, PWTimes delays){
    if(frame_order.count < delays.count){
        delays.count = frame_order.count;
    } else {
        frame_order.count = delays.count;
    }

    PWSpriteAnimation anim = pw_sprite_animation_create(frame_images, frame_order, delays);

    PWAsset asset = {
        .type = PW_ASSET_SPRITE_ANIMATION,
        .sprite_animation = anim
    };

    pw_asset_t asset_id;
    pool_append(am->asset_pool, asset, asset_id);
    return asset_id;
}

int pw_sprite_animation_set_frame_images(PWAssetManager* am, pw_asset_t anim_id, pw_asset_t frame_images){
    PWAsset* anim = &pool_get(am->asset_pool, anim_id);
    anim->sprite_animation.frame_images = frame_images;
    return 0;
}

int pw_sprite_animation_set_frames(PWAssetManager* am, pw_asset_t anim_id, PWFrames frames){
    PWAsset* anim = &pool_get(am->asset_pool, anim_id);
    da_copy(anim->sprite_animation.frame_order, frames);
    return 0;
}

int pw_sprite_animation_set_delays(PWAssetManager* am, pw_asset_t anim_id, PWTimes delays){
    PWAsset* anim = &pool_get(am->asset_pool, anim_id);
    da_copy(anim->sprite_animation.delays, delays);
    return 0;
}

/************************************************************/





// RENDERABLES
/************************************************************/

PWRenderable pw_sprite_animator_create_renderable(PWAssetManager* am, pw_asset_t animation, bool playing, bool looping){
    PWSpriteAnimator animator = pw_sprite_animator_create(am, animation, 0, 0.0f, playing, looping);

    PWRenderable renderable = {
        .type = PW_RENDERABLE_SPRITE_ANIMATOR,
        .sprite_animator = animator
    };

    return renderable;
}

void pw_sprite_animator_set_playing(PWAssetManager* am, PWSpriteAnimator* animator, bool playing){
    animator->playing = playing;
}

void pw_sprite_animator_set_looping(PWAssetManager* am, PWSpriteAnimator* animator, bool looping){
    animator->looping = looping;
}

void pw_sprite_animator_update(PWAssetManager* am, PWSpriteAnimator* animator, pw_time_t delta_time){
    PWAsset* animation_asset = &pool_get(am->asset_pool, animator->animation);
    PWSpriteAnimation *animation = &animation_asset->sprite_animation;

    if(!animator->playing) return;

    pw_time_t delta_sum = 0.0f;
    animator->timer += delta_time;
    if(animator->timer > animation->lasts && animator->looping){
        animator->timer -= animation->lasts;
    }

    for(size_t i = 0; i < animation->delays.count; i++){
        delta_sum += animation->delays.items[i];
        if(animator->timer < delta_sum){
            animator->current_frame = animation->frame_order.items[i];
            break;
        }
    }
}




// INFO


PWAssetType pw_get_asset_type(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    return asset.type;
}


// TODO: Change the first 2

size_t pw_asset_manager_get_image_height(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return 0;
    }

    PWSprite sprite = asset.sprite;
    Image image = pool_get(am->image_pool, sprite.image_id);

    return image.height;
}

size_t pw_asset_manager_get_image_width(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return 0;
    }

    PWSprite sprite = asset.sprite;
    Image image = pool_get(am->image_pool, sprite.image_id);

    return image.width;
}


size_t pw_asset_manager_get_sprite_offsetx(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return 0;
    }
    PWSprite sprite = asset.sprite;
    return sprite.offsetx;
}

size_t pw_asset_manager_get_sprite_offsety(PWAssetManager* am, pw_asset_t asset_id){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    if(asset.type != PW_ASSET_SPRITE){
        return 0;
    }
    PWSprite sprite = asset.sprite;
    return sprite.offsety;
}



/************************************************************/
