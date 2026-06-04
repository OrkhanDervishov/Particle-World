#include "renderer_3d.h"
#define HT_IMPLEMENTATION
#include "ht.h"

void get_gl_info(){
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Lang: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int init_opengl(Window* window){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    window->gl_context = SDL_GL_CreateContext(window->window);
    if(window->gl_context == NULL){
        printf("ERROR: SDL_GL_CreateContext failed\n");
        return 1;
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        printf("ERROR: glad load failed\n");
        return 2;
    }

    return 0;
}



#define SHADER_READ_BUFFER_SIZE 256
int load_source(const char* filename, char** source){
    if(*source != NULL) return 1;

    FILE* file = fopen(filename, "r");
    if(file == NULL) return 2;
    
    char c;
    int index = 0;
    int pagenum = 1;
    while((c = getc(file)) != EOF) {
        if(!(*source) || index == SHADER_READ_BUFFER_SIZE) {
            (*source) = (char*) realloc((*source), SHADER_READ_BUFFER_SIZE * pagenum + 1);
            ++pagenum;
        }
        (*source)[index++] = c;
    }
    (*source)[index] = '\0';

    fclose(file);
    return 0;
}
#undef SHADER_READ_BUFFER_SIZE

#define SHADER_ERROR_BUFFER_LEN 1024
GL_Shader load_gl_shader(GLuint type, const char* filename){
    GLuint shader_obj;
    
    if(type == GL_VERTEX_SHADER){
        shader_obj = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type == GL_FRAGMENT_SHADER){
        shader_obj = glCreateShader(GL_FRAGMENT_SHADER);
    }
    
    char* source = NULL;
    int err;
    if((err = load_source(filename, &source))){
        printf("load_gl_shader: could not read file. ERROR: %d\n", err);
        return (GL_Shader){0};
    }
    printf("shader\n%s\n", source);
    glShaderSource(shader_obj, 1, &source, NULL);
    glCompileShader(shader_obj);

    GLint success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
    if(!success){
        char log[SHADER_ERROR_BUFFER_LEN];
        glGetShaderInfoLog(shader_obj, SHADER_ERROR_BUFFER_LEN, NULL, log);
        printf("%s shader compile error:\n%s\n", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", log);
    }
    
    GL_Shader shader = {
        .shader = shader_obj
    };

    return shader;
}


GL_Program create_gl_program(GL_Shader vertex_shader, GL_Shader fragment_shader){
    GLuint program_obj = glCreateProgram();

    glAttachShader(program_obj, vertex_shader.shader);
    glAttachShader(program_obj, fragment_shader.shader);
    glLinkProgram(program_obj);

    GLint success;
    glGetProgramiv(program_obj, GL_LINK_STATUS, &success);
    if(!success){
        char log[SHADER_ERROR_BUFFER_LEN];
        glGetProgramInfoLog(program_obj, sizeof(log), NULL, log);
        printf("Program link error:\n%s\n", log);
    }
    
    glValidateProgram(program_obj);
    
    GLint valid;
    glGetProgramiv(program_obj, GL_VALIDATE_STATUS, &valid);
    if(!valid){
        char log[SHADER_ERROR_BUFFER_LEN];
        glGetProgramInfoLog(program_obj, sizeof(log), NULL, log);
        printf("Program validation error:\n%s\n", log);
    }
    
    GL_Program program = {
        .program = program_obj,
        .uniforms = {
            .hasheq = ht_cstr_hasheq
        }
    };

    return program;
}
#undef SHADER_ERROR_BUFFER_LEN


void program_add_uniform(GL_Program *program, const char* name){
    GLint loc = glGetUniformLocation(program->program, name);
    printf("%s -> %d\n", name, loc);
    *ht_put(&(program->uniforms), name) = loc;
}

void program_set_uniform_mat4f(GL_Program *program, const char* name, mat4f mat){
    GLint loc = *ht_find(&(program->uniforms), name);
    // printf("%s <- %d\n", name, loc);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat.m);
}

void program_set_uniform_vec3f(GL_Program *program, const char* name, vec3f vec){
    GLint loc = *ht_find(&(program->uniforms), name);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void program_set_uniform_vec4f(GL_Program *program, const char* name, vec4f vec){
    GLint loc = *ht_find(&(program->uniforms), name);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.t);
}

void program_set_uniform_float(GL_Program *program, const char* name, float value){
    GLint loc = *ht_find(&(program->uniforms), name);
    glUniform1f(loc, value);
}

/****************************************************/
/****************************************************/

Camera3d create_camera(vec3f init_pos){
    Camera3d cam;

    cam.pos = init_pos;
    cam.forward = (vec3f){0.0f, 0.0f, -1.0f};
    cam.up = (vec3f){0.0f, 1.0f,  0.0f};

    cam.fov = 45.0f * (3.14159265f / 180.0f);
    cam.aspect = 16.0f / 9.0f;
    cam.near = 0.1f;
    cam.far  = 100.0f;

    cam.perspective = matrix_perspective(cam.fov, cam.aspect, cam.near, cam.far);

    return cam;
}


Model create_model(){
    
    // float triangle_model[] = {
    //     -0.8f, -0.8f,  0.0f, 
    //      0.8f, -0.8f,  0.0f, 
    //      0.0f,  0.8f,  0.0f
    // };
    float triangle_model[] = {

        -0.5f,  0.5f,  0.0f, 
        -0.5f, -0.5f,  0.0f, 
         0.5f, -0.5f,  0.0f,

        -0.5f,  0.5f,  0.0f, 
         0.5f,  0.5f,  0.0f, 
         0.5f, -0.5f,  0.0f
    };

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), triangle_model, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    Model m = {
        .vao = vao,
        .vbo = vbo
    };
    return m;
}

void draw_model(Model model){
    glBindVertexArray(model.vao);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void prepare_draw(Window* window, Colorf colorf){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, window->w, window->h);
    glClearColor(colorf.r, colorf.g, colorf.b, colorf.a);
}

Object3d create_object3d(vec3f pos, Model model){
    Object3d obj;

    obj.pos = pos;
    obj.model = model;

    obj.translation = matrix_translation(pos.x, pos.y, pos.z);
    obj.rotation = matrix_identity();
    obj.scale = matrix_identity();

    object_model_matrix(&obj);

    return obj;
}

void move_object(Object3d *obj, vec3f translation){
    obj->pos.x += translation.x;
    obj->pos.y += translation.y;
    obj->pos.z += translation.z;
    obj->translation = matrix_mul(obj->translation, matrix_translation(translation.x, translation.y, translation.z));
}

void rotate_object(Object3d *obj, vec3f rotation){
    mat4f rx = matrix_rotation_x(rotation.x);
    mat4f ry = matrix_rotation_y(rotation.y);
    mat4f rz = matrix_rotation_z(rotation.z);

    obj->rotation = matrix_mul(obj->rotation, matrix_mul(rz, matrix_mul(ry, rx)));
}

void scale_object(Object3d *obj, vec3f scale){
    obj->scale = matrix_mul(obj->scale, matrix_scale(scale.x, scale.y, scale.z));
}

void object_model_matrix(Object3d *obj){
    obj->model_matrix = matrix_mul(obj->translation, matrix_mul(obj->scale, obj->rotation));
}

mat4f camera_view(Camera3d camera){
    return matrix_look_at(
        camera.pos,
        vector_sum(camera.pos, camera.forward),
        camera.up
    );
}