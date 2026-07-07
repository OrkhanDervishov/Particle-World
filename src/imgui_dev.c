#include "particle_game.h"
#include "input_system.h"


// #define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_SDL2
#define CIMGUI_USE_SDLRENDERER2
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "cimgui/cimgui_bridge.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _WIN32
#include <windows.h>        // SetProcessDPIAware()
#endif

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

#define igGetIO igGetIO_Nil


// ImGuiIO* io = igGetIO();
// ImGuiStyle* style = igGetStyle();
static ImGuiIO* io;
static ImGuiStyle* style;

static bool show_demo;
static bool show_another;
static bool quit;

static ImVec4 clear_color;


static void imgui_init(ParticleEngine* engine){
#ifdef _WIN32
    // SetProcessDPIAware();
#endif

    SDL_Window* window = engine->win->window;
    SDL_Renderer* renderer = engine->win->renderer;
    // SDL_GLContext gl_context = engine->win->gl_context;

    const char* glsl_version = "#version 130";
    float main_scale = cImGui_ImplSDL2_GetContentScaleForDisplay(0);

    // SDL_GL_SetSwapInterval(1);

    // Create ImGui context
    igCreateContext(NULL);

    io = igGetIO();
    style = igGetStyle();

    // DPI scaling
    ImGuiStyle_ScaleAllSizes(style, main_scale);
    style->FontScaleDpi = main_scale;
    io->ConfigDpiScaleFonts = true;
    io->ConfigDpiScaleViewports = true;

    // Enable features
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

#ifdef IMGUI_HAS_DOCK
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

    // Initialize backends
    // cImGui_ImplOpenGL3_Init(glsl_version);
    // cImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    // cImGui_ImplSDLRenderer2_Init(renderer);
    cImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    cImGui_ImplSDLRenderer2_Init(renderer);

    // Style
    igStyleColorsDark(NULL);
    ImFontAtlas_AddFontDefault(io->Fonts, NULL);

    show_demo = true;
    show_another = false;
    quit = false;

    clear_color = (ImVec4){0.45f, 0.55f, 0.60f, 1.00f};
}

static void imgui_uninit(ParticleEngine* engine){
    // cImGui_ImplOpenGL3_Shutdown();
    cImGui_ImplSDLRenderer2_Shutdown();
    cImGui_ImplSDL2_Shutdown();
    igDestroyContext(NULL);
}


static int imgui_dev(ParticleEngine* engine)
{

    SDL_Window* window = engine->win->window;
    SDL_Renderer* renderer = engine->win->renderer;
    // SDL_GLContext gl_context = engine->win->gl_context;

    // Begin frame
    // cImGui_ImplOpenGL3_NewFrame();
    cImGui_ImplSDL2_NewFrame();
    cImGui_ImplSDLRenderer2_NewFrame();
    igNewFrame();

    if (show_demo)
        igShowDemoWindow(&show_demo);

    {
        static float f = 0.0f;
        static int counter = 0;

        igBegin("Hello, world!", NULL, 0);

        igText("This is some useful text");
        igCheckbox("Demo window", &show_demo);
        igCheckbox("Another window", &show_another);

        igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
        igColorEdit3("clear color", (float*)&clear_color, 0);

        ImVec2 sz = {0,0};
        if (igButton("Button", sz))
            counter++;

        igSameLine(0, -1);
        igText("counter = %d", counter);

        igText("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io->Framerate,
                io->Framerate);

        igEnd();
    }

    if (show_another)
    {
        igBegin("Another Window", &show_another, 0);
        igText("Hello from imgui");

        ImVec2 sz = {0,0};
        if (igButton("Close me", sz))
            show_another = false;

        igEnd();
    }

    // Render
    igRender();

    // SDL_GL_MakeCurrent(window, gl_context);

    // glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
    // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);


    // cImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
    cImGui_ImplSDLRenderer2_RenderDrawData(igGetDrawData(), renderer);

    // if (SDL_GetError()[0] != '\0')
    //     printf("SDL error: %s\n", SDL_GetError());

#ifdef IMGUI_HAS_DOCK
    // if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // {
    //     SDL_Window* backup_window = SDL_GL_GetCurrentWindow();
    //     SDL_GLContext backup_ctx = SDL_GL_GetCurrentContext();

    //     igUpdatePlatformWindows();
    //     igRenderPlatformWindowsDefault(NULL, NULL);

    //     // SDL_GL_MakeCurrent(backup_window, backup_ctx);
    // }
#endif

    // SDL_GL_SwapWindow(window);

    return 0;
}
