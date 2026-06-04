#ifndef RENDERER_3D_H_
#define RENDERER_3D_H_

#include <glad/glad.h>
#include "sdl2_layer.h"
#include "window.h"
#include "la.h"
#include "ht.h"

/****************************************************/
// OpenGL functions

typedef struct{
    GLuint shader;
} GL_Shader;

typedef struct{
    GLuint program;
    Ht(const char*, GLint) uniforms;
} GL_Program;

void get_gl_info();
int init_opengl(Window* window);

int load_source(const char* filename, char** source);
GL_Shader load_gl_shader(GLuint type, const char* filename);
GL_Program create_gl_program(GL_Shader vertex_shader, GL_Shader fragment_shader);

void program_add_uniform(GL_Program *program, const char* name);
void program_set_uniform_mat4f(GL_Program *program, const char* name, mat4f mat);
void program_set_uniform_vec3f(GL_Program *program, const char* name, vec3f vec);
void program_set_uniform_vec4f(GL_Program *program, const char* name, vec4f vec);
void program_set_uniform_float(GL_Program *program, const char* name, float value);

/****************************************************/


/****************************************************/
// 3D functions

typedef struct{
    vec3f pos;
    vec3f forward, up;
    float near, far;
    float fov, aspect;
    mat4f perspective;
} Camera3d;

// Dynamic array used for storing model's vertex data
typedef struct{
    float *items;
    size_t count;
    size_t capacity;
} ModelVertices;

typedef struct{
    GLuint vao;
    GLuint vbo;
} Model;

typedef struct{
    vec3f pos;
    Model model;
    mat4f translation;
    mat4f rotation;
    mat4f scale;
    mat4f model_matrix;
} Object3d;


Camera3d create_camera(vec3f init_pos);
mat4f camera_view(Camera3d camera);
Model create_model();
Object3d create_object3d(vec3f pos, Model model);
void move_object(Object3d *obj, vec3f translation);
void rotate_object(Object3d *obj, vec3f rotation);
void scale_object(Object3d *obj, vec3f scale);
void object_model_matrix(Object3d *obj);


/****************************************************/


/****************************************************/
// Rendering

void draw_model(Model model);
void prepare_draw(Window* window, Colorf colorf);

/****************************************************/

#endif //RENDERER_3D_H_