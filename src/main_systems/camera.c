#include "camera.h"

PWCamera2D pw_create_camera2d(vec2f pos, vec2f resolution, float zoom){
    PWCamera2D camera = {
        .pos = pos,
        .resolution = resolution,
        .zoom = zoom
    };
    return camera;
}