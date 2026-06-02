#include "la.h"
#include <math.h>

inline float randf(float min, float max){
    return min + (max-min) * ((float)rand() / (float)RAND_MAX);
}

inline vec3f vector_inv(vec3f v){
    return (vec3f){
        -v.x,
        -v.y,
        -v.z
    };
}

inline vec3f vector_sum(vec3f a, vec3f b){
    return (vec3f){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

inline vec3f vector_sub(vec3f a, vec3f b){
    return (vec3f){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

inline vec3f vector_scale(vec3f v, float scale){
    return (vec3f){
        v.x * scale,
        v.y * scale,
        v.z * scale
    };
}

inline float vector_dot(vec3f a, vec3f b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline float distance3f(vec3f v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline float get_angle(vec3f a, vec3f b){
    float a_dist = distance3f(a);
    float b_dist = distance3f(b);
    float dot = vector_dot(a, b);
    float angle = cosf(dot/(a_dist*b_dist));
    return angle;
}


inline vec3f vector_normalize(vec3f v){
    float len = distance3f(v);
    return (vec3f){
        v.x / len,
        v.y / len,
        v.z / len
    };
}
