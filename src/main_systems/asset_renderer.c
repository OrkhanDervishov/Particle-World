#include "asset_renderer.h"


void pw_draw_image(Image context, PWAssetManager* am, pw_image_t image_id, Transforms2d transforms){
    Image img = pool_get(am->image_pool, image_id);
    pnt_blit_transformed(context, img, transforms.translation, transforms.rotation, transforms.scale);    
}

void pw_draw_sprite_multiple(Image context , PWAssetManager* am, pw_asset_t asset_id, Transforms2d transforms, int index){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    Image img = pool_get(am->image_pool, asset.sprite.image_id);
    
    Image frame = pw_sprite_multiple_get_image(am, asset_id, index);
    pnt_blit_transformed(context, frame, transforms.translation, transforms.rotation, transforms.scale);
}

void pw_draw_sprite(Image context , PWAssetManager* am, pw_asset_t asset_id, Transforms2d transforms){
    PWAsset asset = pool_get(am->asset_pool, asset_id);
    Image img = pool_get(am->image_pool, asset.sprite.image_id);
    pnt_blit_transformed(context, img, transforms.translation, transforms.rotation, transforms.scale);
}

void pw_draw_animation(Image context , PWAssetManager* am, PWSpriteAnimator *animator, Transforms2d transforms, pw_time_t delta){
    PWAsset animation = pool_get(am->asset_pool, animator->animation);
    pw_sprite_animator_update(am, animator, delta);
    pw_draw_sprite_multiple(context, am, animation.sprite_animation.frame_images, transforms, animator->current_frame);
}

void pw_draw_renderable(Image context , PWAssetManager* am, PWRenderable *renderable, Transforms2d transforms, pw_time_t delta){
    switch(renderable->type){
        case PW_RENDERABLE_SPRITE_STATIC:
        pw_draw_sprite(context, am, renderable->asset, transforms);
        break;
        case PW_RENDERABLE_SPRITE_ANIMATOR:
        pw_draw_animation(context, am, &(renderable->sprite_animator), transforms, delta);
        break;
        default:
        printf("ERROR (pw_draw_asset): Unable to draw asset of given type\n"); // TODO: Get type name
    }
}