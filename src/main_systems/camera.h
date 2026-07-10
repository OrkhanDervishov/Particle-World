#ifndef CAMERA_H
#define CAMERA_H

#include "core.h"

typedef struct{
    vec2f pos;
    vec2f resolution;
    float zoom;
} PWCamera2D;

PWCamera2D pw_create_camera2d(vec2f pos, vec2f resolution, float zoom);

#endif