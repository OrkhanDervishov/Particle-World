#include "camera.h"

PWCamera2D pw_create_camera2d(vec2f pos, vec2f resolution, float zoom){
    PWCamera2D camera = {
        .pos = pos,
        .resolution = resolution,
        .zoom = zoom
    };
    return camera;
}

vec2 pw_world_to_view(vec2f world_pos, PWCamera2D camera){
    vec2 pos = {
        .x = (world_pos.x - camera.pos.x) * camera.zoom + camera.resolution.x*0.5f,
        .y = (camera.pos.y - world_pos.y) * camera.zoom + camera.resolution.y*0.5f
    };
    return pos;
}