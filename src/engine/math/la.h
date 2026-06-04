#ifndef LINEAR_H_
#define LINEAR_H_

#include <stdlib.h>

typedef struct{
    int x, y;
} vec2;
typedef vec2 vec2i;

typedef struct{
    int x, y, z;
} vec3;

typedef struct{
    int x, y, z, t;
} vec4;

typedef struct{
    float x, y;
} vec2f;
typedef vec2 Posf;

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
extern vec3f vector_inv(vec3f v);
extern vec3f vector_sum(vec3f a, vec3f b);
extern vec3f vector_sub(vec3f a, vec3f b);
extern vec3f vector_scale(vec3f v, float scale);
extern float vector_dot(vec3f a, vec3f b);
extern vec3f vector_cross(vec3f a, vec3f b);
extern float distance3f(vec3f v);
extern float get_angle(vec3f a, vec3f b);
extern vec3f vector_normalize(vec3f v);

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


#endif //LINEAR_H_