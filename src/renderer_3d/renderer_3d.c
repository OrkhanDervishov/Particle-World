#include "renderer_3d.h"

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

GL_Shader load_gl_shader(GLuint type, const char* filename){
    
    GLuint shader_obj;
    
    
    if(type == GL_VERTEX_SHADER){
        shader_obj = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type == GL_FRAGMENT_SHADER){
        shader_obj = glCreateShader(GL_FRAGMENT_SHADER);
    }
    
    char* source = NULL;
    if(!load_source(filename, &source)){
        printf("load_gl_shader: could not read file\n");
        return (GL_Shader){0};
    }
    glShaderSource(shader_obj, 1, source, 1);
    glCompileShader(shader_obj);
    
    return (GL_Shader){shader_obj};
}

GL_Program create_gl_shader_program(GL_Shader vertex_shader, GL_Shader fragment_shader){
    GLuint program_obj = glCreateProgram();

    glAttachShader(program_obj, vertex_shader.shader);
    glAttachShader(program_obj, fragment_shader.shader);
    glLinkProgram(program_obj);

    glValidateProgram(program_obj);

    return (GL_Program){program_obj};
}
