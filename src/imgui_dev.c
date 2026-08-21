#define PW_USE_IMGUI
#ifdef PW_USE_IMGUI

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


static ImGuiIO* io;
static ImGuiStyle* style;

static bool show_demo;
static bool show_another;
static bool quit;

static ImVec4 clear_color;


static bool paused = false;
static bool show_colliders = false;
static bool show_positions = true;
static bool levitate = false;
static float simulation_speed = 1.0f;
static float speed_x = 700.0f;
static float speed_y = 1500.0f;
static float gravity0 = 9.81f;






static bool imgui_read_event(SDL_Event e){
    cImGui_ImplSDL2_ProcessEvent(&e);   
    ImGuiIO* io = igGetIO();
    if (io->WantCaptureMouse || io->WantCaptureKeyboard)
        return true;
    return false;
}

static void imgui_init(ParticleEngine* engine){

    SDL_Window* window = engine->win->window;
    SDL_Renderer* renderer = engine->win->renderer;

    const char* glsl_version = "#version 130";
    float main_scale = cImGui_ImplSDL2_GetContentScaleForDisplay(0);

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
    cImGui_ImplSDLRenderer2_Shutdown();
    cImGui_ImplSDL2_Shutdown();
    igDestroyContext(NULL);
}


static int imgui_dev(ParticleEngine* engine)
{
    SDL_Renderer* renderer = engine->win->renderer;

    // Begin frame
    cImGui_ImplSDL2_NewFrame();
    cImGui_ImplSDLRenderer2_NewFrame();
    igNewFrame();


    igBegin("Particle Engine", NULL, 0);

    igText("Particle Sandbox");
    igSeparator();

    igCheckbox("Pause Simulation", &paused);
    igCheckbox("Show Colliders", &show_colliders);
    igCheckbox("Show Positions", &show_positions);
    igCheckbox("Levitate", &levitate);
    // igSliderFloat("Simulation Speed", &simulation_speed,0.1f, 5.0f, "%.2fx", 0);
    igSliderFloat("Speed X", &speed_x, 0.0f, 3000.0f, "%.2f", 0);
    igSliderFloat("Speed Y", &speed_y, 0.0f, 3000.0f, "%.2f", 0);
    igSliderFloat("Gravity", &gravity0, -20.0f, 20.0f, "%.2f", 0);

    igSeparator();

    if (igButton("Reset", (ImVec2){120, 0}))
    {
        simulation_speed = 1.0f;
        speed_x = 700.0f;
        speed_y = 1500.0f;
        gravity0 = 9.81f;
        paused = false;
        show_colliders = false;
        show_positions = false;
        levitate = false;

    }

    igSeparator();

    igText("FPS: %.1f", io->Framerate);
    igText("Frame Time: %.3f ms", 1000.0f / io->Framerate);

    igEnd();

    // Render
    igRender();
    cImGui_ImplSDLRenderer2_RenderDrawData(igGetDrawData(), renderer);

    return 0;
}

#endif //PW_USE_IMGUI