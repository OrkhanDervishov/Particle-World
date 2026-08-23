#ifndef PW_CAMERA_H
#define PW_CAMERA_H

#include "core.h"
#include "painter.h"

typedef struct{
    vec2f pos;
    vec2f resolution;
    float zoom;
} PWCamera2D;

PWCamera2D pw_create_camera2d(vec2f pos, vec2f resolution, float zoom);
vec2 pw_world_to_view_pos(vec2f world_pos, PWCamera2D camera);
vec2f pw_world_to_view_posf(vec2f world_pos, PWCamera2D camera);
Rect pw_world_to_view_rect(Rect rect, PWCamera2D camera);
Rectf pw_world_to_view_rectf(Rectf rect, PWCamera2D camera);
Transforms2d pw_world_to_view_transform(Transforms2d transform, PWCamera2D camera);

#endif //PW_CAMERA_H