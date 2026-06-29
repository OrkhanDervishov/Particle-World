#ifndef ASSET_RENDERER_H_
#define ASSET_RENDERER_H_

#include "asset.h"

void pw_draw_image(Image context, PWAssetManager* am, pw_image_t image_id, Transforms2d transforms);
void pw_draw_animation(Image context , PWAssetManager* am, pw_asset_t animator_id, Transforms2d transforms, pw_time_t delta);
void pw_draw_sprite_multiple(Image context , PWAssetManager* am, pw_asset_t asset_id, Transforms2d transforms, int index);
void pw_draw_asset(Image context , PWAssetManager* am, pw_asset_t asset_id, Transforms2d transforms, pw_time_t delta);

#endif //ASSET_RENDERER_H_