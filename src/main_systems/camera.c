#include "camera.h"

PWCamera2D pw_create_camera2d(vec2f pos, vec2f resolution, float zoom){
    PWCamera2D camera = {
        .pos = pos,
        .resolution = resolution,
        .zoom = zoom
    };
    return camera;
}

vec2 pw_world_to_view_pos(vec2f world_pos, PWCamera2D camera){
    return (vec2){
        .x = (world_pos.x - camera.pos.x) * camera.zoom + camera.resolution.x*0.5f,
        .y = (camera.pos.y - world_pos.y) * camera.zoom + camera.resolution.y*0.5f
    };
}

vec2f pw_world_to_view_posf(vec2f world_pos, PWCamera2D camera){
    return (vec2f){
        .x = (world_pos.x - camera.pos.x) * camera.zoom + camera.resolution.x*0.5f,
        .y = (world_pos.y - camera.pos.y) * camera.zoom - camera.resolution.y*0.5f
        // .y = (world_pos.y - camera.pos.y) * camera.zoom + camera.resolution.y*0.5f
    };
}

Rect pw_world_to_view_rect(Rect rect, PWCamera2D camera){
    return (Rect){
        .x = (int)((float)rect.x - camera.pos.x) * camera.zoom + camera.resolution.x*0.5f,
        .y = (int)(camera.pos.y - (float)rect.y) * camera.zoom + camera.resolution.y*0.5f,
        .w = (int)((float)rect.w * camera.zoom),
        .h = (int)((float)rect.h * camera.zoom)
    };
}

Rectf pw_world_to_view_rectf(Rectf rect, PWCamera2D camera){
    return (Rectf){
        .x = (rect.x - camera.pos.x) * camera.zoom + camera.resolution.x*0.5f,
        .y = (camera.pos.y - rect.y) * camera.zoom + camera.resolution.y*0.5f,
        .w = rect.w * camera.zoom,
        .h = rect.h * camera.zoom
    };
}

Transforms2d pw_world_to_view_transform(Transforms2d transform, PWCamera2D camera){
    return (Transforms2d){
        .translation = pw_world_to_view_posf(transform.translation, camera),
        .rotation = transform.rotation,
        .scale = (vec2f){transform.scale.x * camera.zoom, transform.scale.y * camera.zoom}
    };
}