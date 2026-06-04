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

inline vec3f vector_cross(vec3f a, vec3f b){
    return (vec3f){abs(a.y*b.z - a.z*b.y), -abs(a.x*b.z - a.z*b.x), abs(a.x*b.y - b.x*a.y)};
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
    vec3f f = vector_normalize(vector_sub(center, eye));
    vec3f s = vector_normalize(vector_cross(f, up));
    vec3f u = vector_cross(s, f);

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

    result.m[12] = -vector_dot(s, eye);
    result.m[13] = -vector_dot(u, eye);
    result.m[14] =  vector_dot(f, eye);

    return result;
}