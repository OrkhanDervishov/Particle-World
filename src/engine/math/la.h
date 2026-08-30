#ifndef LINEAR_H_
#define LINEAR_H_

#include <stdlib.h>
#include <stdint.h>

#define PI_CONST 3.14159265f

typedef struct{
    size_t x, y;
} vec2ui;

typedef struct{
    int x, y;
} vec2;
typedef vec2 vec2i;

typedef struct{
    int64_t x, y;
} vec2li;

typedef struct{
    int x, y, z;
} vec3;

typedef struct{
    int x, y, z, t;
} vec4;

typedef struct{
    float x, y;
} vec2f;
typedef vec2f Posf;

typedef struct{
    float x, y, z;
} vec3f;

typedef struct{
    float x, y, z, t;
} vec4f;

typedef struct{
    float m[16];
} mat4f;

/*
    TODO: create macro functions
*/

extern float randf(float min, float max);

extern vec2f vec2_inv(vec2f v);
extern vec2f vec2_sum(vec2f a, vec2f b);
extern vec2f vec2_sub(vec2f a, vec2f b);
extern vec2f vec2_scale(vec2f v, float scale);
extern float vec2_dot(vec2f a, vec2f b);
extern float vec2_length(vec2f v);
extern float vec2_get_angle(vec2f a, vec2f b);
extern float vec2_get_angle_360(vec2f a, vec2f b);
extern vec2f vec2_normalize(vec2f v);
extern vec2f vec2_rotate(vec2f v, float theta);

extern vec3f vec3_inv(vec3f v);
extern vec3f vec3_sum(vec3f a, vec3f b);
extern vec3f vec3_sub(vec3f a, vec3f b);
extern vec3f vec3_scale(vec3f v, float scale);
extern float vec3_dot(vec3f a, vec3f b);
extern vec3f vec3_cross(vec3f a, vec3f b);
extern float vec3_length(vec3f v);
extern float vec3_get_angle(vec3f a, vec3f b);
extern float vec3_get_cos(vec3f a, vec3f b);
extern vec3f vec3_normalize(vec3f v);

extern mat4f matrix_identity();
extern mat4f matrix_mul(mat4f a, mat4f b);
extern mat4f matrix_translation(float x, float y, float z);
extern mat4f matrix_scale(float x, float y, float z);
extern mat4f matrix_rotation_x(float radians);
extern mat4f matrix_rotation_y(float radians);
extern mat4f matrix_rotation_z(float radians);
extern mat4f matrix_perspective(float fov_radians, float aspect, float near, float far);
extern mat4f matrix_ortho(float left, float right, float bottom, float top, float near, float far);
extern mat4f matrix_look_at(vec3f eye, vec3f center, vec3f up);


void print_vec3(vec3f v);
void print_matrix4(mat4f m);

#endif //LINEAR_H_