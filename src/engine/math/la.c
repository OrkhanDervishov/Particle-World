#include "la.h"
#include <stdio.h>
#include <math.h>


// Untils
inline float randf(float min, float max){
    return min + (max-min) * ((float)rand() / (float)RAND_MAX);
}

// Vector 2

inline vec2f vec2_inv(vec2f v){
    return (vec2f){
        -v.x,
        -v.y
    };
}

inline vec2f vec2_sum(vec2f a, vec2f b){
    return (vec2f){
        a.x + b.x,
        a.y + b.y
    };
}

inline vec2f vec2_sub(vec2f a, vec2f b){
    return (vec2f){
        a.x - b.x,
        a.y - b.y
    };
}

inline vec2f vec2_scale(vec2f v, float scale){
    return (vec2f){
        v.x * scale,
        v.y * scale
    };
}

inline float vec2_dot(vec2f a, vec2f b){
    return a.x*b.x + a.y*b.y;
}

inline float vec2_cross(vec2f a, vec2f b){
    return a.x*b.y - a.y*b.x;
}

inline float vec2_length(vec2f v){
    return sqrtf(v.x*v.x + v.y*v.y);
}

inline float vec2_get_angle(vec2f a, vec2f b){
    float a_dist = vec2_length(a);
    float b_dist = vec2_length(b);
    float dot = vec2_dot(a, b);
    float angle = acosf(dot/(a_dist*b_dist));
    return angle;
}

inline float vec2_get_angle_360(vec2f a, vec2f b){
    float dot = vec2_dot(a, b);
    float cross = vec2_cross(a, b);

    float angle = atan2f(cross, dot);
    angle = angle < 0.0f ? angle + 2*PI_CONST : angle;

    return angle;
}

inline vec2f vec2_normalize(vec2f v){
    float len = vec2_length(v);

    if(len < 0.000001f)
        return (vec2f){0.0f, 0.0f};

    return (vec2f){
        v.x / len,
        v.y / len,
    };
}

extern vec2f vec2_rotate(vec2f v, float theta){
    return (vec2f){
        v.x*cosf(theta) - v.y*sinf(theta),
        v.x*sinf(theta) + v.y*cosf(theta)
    };
}


// Vector 3

inline vec3f vec3_inv(vec3f v){
    return (vec3f){
        -v.x,
        -v.y,
        -v.z
    };
}

inline vec3f vec3_sum(vec3f a, vec3f b){
    return (vec3f){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

inline vec3f vec3_sub(vec3f a, vec3f b){
    return (vec3f){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

inline vec3f vec3_scale(vec3f v, float scale){
    return (vec3f){
        v.x * scale,
        v.y * scale,
        v.z * scale
    };
}

inline float vec3_dot(vec3f a, vec3f b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline vec3f vec3_cross(vec3f a, vec3f b){
    return (vec3f){(a.y*b.z - a.z*b.y), -(a.x*b.z - a.z*b.x), (a.x*b.y - b.x*a.y)};
}

inline float vec3_length(vec3f v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline float vec3_get_angle(vec3f a, vec3f b){
    float a_dist = vec3_length(a);
    float b_dist = vec3_length(b);
    float dot = vec3_dot(a, b);
    float angle = cosf(dot/(a_dist*b_dist));
    return angle;
}

inline float vec3_get_cos(vec3f a, vec3f b){
    float a_dist = vec3_length(a);
    float b_dist = vec3_length(b);
    float dot = vec3_dot(a, b);
    return dot/(a_dist*b_dist);
}


inline vec3f vec3_normalize(vec3f v){
    float len = vec3_length(v);

    if(len < 0.000001f)
        return (vec3f){0.0f, 0.0f, 0.0f};

    return (vec3f){
        v.x / len,
        v.y / len,
        v.z / len
    };
}


/*******************************************/
// Matrix

inline mat4f matrix_identity(void)
{
    mat4f r = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};
    return r;
}

inline mat4f matrix_mul(mat4f a, mat4f b)
{
    mat4f r;

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            r.m[col * 4 + row] =
                a.m[0 * 4 + row] * b.m[col * 4 + 0] +
                a.m[1 * 4 + row] * b.m[col * 4 + 1] +
                a.m[2 * 4 + row] * b.m[col * 4 + 2] +
                a.m[3 * 4 + row] * b.m[col * 4 + 3];
        }
    }

    return r;
}

inline mat4f matrix_translation(float x, float y, float z)
{
    mat4f r = matrix_identity();

    r.m[12] = x;
    r.m[13] = y;
    r.m[14] = z;

    return r;
}

inline mat4f matrix_scale(float x, float y, float z)
{
    mat4f r = {0};

    r.m[0]  = x;
    r.m[5]  = y;
    r.m[10] = z;
    r.m[15] = 1.0f;

    return r;
}

inline mat4f matrix_rotation_x(float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);

    mat4f r = matrix_identity();

    r.m[5]  = c;
    r.m[6]  = s;
    r.m[9]  = -s;
    r.m[10] = c;

    return r;
}

inline mat4f matrix_rotation_y(float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);

    mat4f r = matrix_identity();

    r.m[0]  = c;
    r.m[2]  = -s;
    r.m[8]  = s;
    r.m[10] = c;

    return r;
}

inline mat4f matrix_rotation_z(float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);

    mat4f r = matrix_identity();

    r.m[0] = c;
    r.m[1] = s;
    r.m[4] = -s;
    r.m[5] = c;

    return r;
}

inline mat4f matrix_perspective(float fov_radians, float aspect, float near, float far)
{
    float f = 1.0f / tanf(fov_radians * 0.5f);

    mat4f r = {0};

    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (far + near) / (near - far);
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * far * near) / (near - far);

    return r;
}

inline mat4f matrix_ortho(float left, float right, float bottom, float top, float near, float far)
{
    mat4f r = matrix_identity();

    r.m[0]  = 2.0f / (right - left);
    r.m[5]  = 2.0f / (top - bottom);
    r.m[10] = -2.0f / (far - near);

    r.m[12] = -(right + left) / (right - left);
    r.m[13] = -(top + bottom) / (top - bottom);
    r.m[14] = -(far + near) / (far - near);

    return r;
}

inline mat4f matrix_look_at(vec3f eye, vec3f center, vec3f up)
{
    vec3f f = vec3_normalize(vec3_sub(center, eye));
    vec3f s = vec3_normalize(vec3_cross(f, up));
    vec3f u = vec3_cross(s, f);

    mat4f result = matrix_identity();

    result.m[0] = s.x;
    result.m[1] = u.x;
    result.m[2] = -f.x;

    result.m[4] = s.y;
    result.m[5] = u.y;
    result.m[6] = -f.y;

    result.m[8]  = s.z;
    result.m[9]  = u.z;
    result.m[10] = -f.z;

    result.m[12] = -vec3_dot(s, eye);
    result.m[13] = -vec3_dot(u, eye);
    result.m[14] =  vec3_dot(f, eye);

    return result;
}

void print_vec3(vec3f v){
    printf("x:%f y:%f z:%f\n", v.x, v.y, v.z);
}

void print_matrix4(mat4f m){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%f ", m.m[i*4 + j]);
        }
        printf("\n");
    }
}