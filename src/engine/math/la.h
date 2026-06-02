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

/*
    TODO: create macro functions
*/


extern float randf(float min, float max);
extern vec3f vector_inv(vec3f v);
extern vec3f vector_sum(vec3f a, vec3f b);
extern vec3f vector_sub(vec3f a, vec3f b);
extern vec3f vector_scale(vec3f v, float scale);
extern float vector_dot(vec3f a, vec3f b);
extern float distance3f(vec3f v);
extern float get_angle(vec3f a, vec3f b);
extern vec3f vector_normalize(vec3f v);


#endif //LINEAR_H_