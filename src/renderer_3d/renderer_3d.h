#ifndef RENDERER_3D_H_
#define RENDERER_3D_H_

#include <glad/glad.h>
#include "sdl2_layer.h"
#include "window.h"
#include "la.h"

typedef struct{
    GLuint shader;
} GL_Shader;

typedef struct{
    GLuint program;
} GL_Program;


typedef struct{
    vec3f pos;
    vec3f forward, up;
    float near, far;
    float fov, aspect;
} Camera3d;


void get_gl_info();
int init_opengl(Window* window);

int load_source(const char* filename, char** source);
GL_Shader load_gl_shader(GLuint type, const char* source);
GL_Program create_gl_shader_program(GL_Shader vertex_shader, GL_Shader fragment_shader);


#endif //RENDERER_3D_H_