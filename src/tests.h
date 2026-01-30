#ifndef TESTS_H
#define TESTS_H

#include "engine/engine_api.h"
#include "partsim/particle_load.h"
#include "structs/chunk.h"
#include "structs/region.h"
#include "structs/chunk_space.h"
// #include "world.h"
// #include "particle_game.h"
#include "chunk_renderer.h"
#include "partsim/dirtyrect.h"
#include "partsim/particle_data.h"
#include "partsim/simulator.h"


int WindowTest(){
    Window* window;
    CONSOLE("Init Test\n");
    CreateWindow(&window, 720, 720, "WindowTest", FALSE);

    CONSOLE("Icon Test\n");
    if(SetWindowIcon(window, "resources/sand.bmp")) CONSOLE("Icon was not set");
    
    CONSOLE("Info Test\n");
    GetWindowInfo(window);

    SDL_Event event;
    CONSOLE("Running Test\n");
    while(window->isrunning){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) window->isrunning = FALSE;
    }

    DestroyWindow(&window);
    CONSOLE("Window test success\n\n");
    return 0;
}

int CoreTest(){
    // TODO: Test Malloc2d and Free2d
    return 0;
}

int GuiTest(){
    return 0;
}

int DirtyRectTest(){
    LinkedList* list;
    Rect r1 = {
        .x = 10,
        .y = 10,
        .w = 10,
        .h = 10
    };

    Rect r2 = {
        .x = -10,
        .y = -10,
        .w = 10,
        .h = 10
    };

    CONSOLE("DirtyRect test started\n");

    CONSOLE("DirtyRect creation test\n");
    InitDirtyRectList(&list);

    CONSOLE("DirtyRect correction test\n");
    CorrectDirtyRect(100, 100, &r1);
    CorrectDirtyRect(100, 100, &r2);
    printf("x:%d, y:%d, w:%d h:%d\n", r1.x, r1.y, r1.w, r1.h);
    printf("x:%d, y:%d, w:%d h:%d\n", r2.x, r2.y, r2.w, r2.h);

    CONSOLE("DirtyRect node addition test\n");
    AddDirtyRect(list, 100, 100, 10, 10, 10);

    CONSOLE("DirtyRect node deletion test\n");
    DeleteDirtyRect(list, list->head);

    CONSOLE("DirtyRect deletion test\n");
    DestroyDirtyRectList(&list);

    CONSOLE("DirtyRect test success\n\n");
    return 0;
}

// Not used anymore
// int ParticleMapTest(){
//     ParticleMap pmap;

//     CONSOLE("ParticleMap test started\n");

//     CONSOLE("ParticleMap creation test\n");
//     CreateParticleMap(&pmap, 256, 256);

//     CONSOLE("ParticleMap clear test\n");
//     ClearMap(&pmap);

//     CONSOLE("ParticleMap deletion test\n");
//     DeleteParticleMap(&pmap);

//     CONSOLE("ParticleMap test success\n\n");
//     return 0;
// }


int ChunkTest(){
    Chunk chunk;
    int chunk_size = 256;
    Pos world_pos = {
        .x = 0,
        .y = 0
    };

    CONSOLE("Chunk test started\n");

    CONSOLE("Chunk creation test\n");
    CreateChunk(&chunk, chunk_size, chunk_size, world_pos);

    CONSOLE("Chunk deletion test\n");
    DeleteChunk(&chunk);

    CONSOLE("Chunk test success\n\n");
    return 0;
}


int RegionTest(){
    
}

// Maybe will not be used anymore, or will be changed completely
// int WorldTest(){
//     World* world;

//     CONSOLE("World test started\n");

    
//     CONSOLE("World creation test\n");
//     CreateWorld(&world);

//     CONSOLE("World deletion test\n");
//     DeleteWorld(&world);

//     CONSOLE("World test success\n\n");
//     return 0;
// }


int ControlsTest(){
    return 0;
}

// int DrawerTest(){
//     Window* window;
//     World* world;

//     CONSOLE("Drawer test started\n");

//     CreateWindow(&window, 720, 720, "Drawer test", FALSE);
//     CreateWorld(&world);
//     InitParticles();
    
//     Clear(window);
    
//     Pos wp = {0, 0};
//     Region region;
//     CreateRegion(&region, 2, 2, wp);
//     FillRegion(&region);
//     ClearRegion(&region);
//     // printf("%d\n", region.chunks[0].pmap.map[0][0].c.r);
//     DrawRegion(window, &region, 0, 0);

//     // Chunk chunk;
//     // CreateChunk(&chunk, 180, 180, wp);
//     // ChunkFillWithAir(&chunk);
//     // ChunkWallBox(&chunk);
//     // ChunkCreateManyParticlesCircle(&chunk, SAND, 15, 32, 20);
//     // ChunkCreateManyParticlesCircle(&chunk, ACID, 100, 60, 60);
//     // ChunkCreateManyParticlesCircle(&chunk, WOOD, 100, 150, 50);
//     // ChunkCreateManyParticlesCircle(&chunk, WATER, 40, 40, 70);
//     // ChunkCreateManyParticlesCircle(&chunk, SAND, 100, 60, 20);
//     // AddDirtyRect(chunk.dirty_rect_list, chunk.h, chunk.w, 10, 10, 100);
//     // DrawChunk2(window, &chunk);
//     SDL_RenderPresent(window->renderer);
//     // SetChunk(&chunk);

//     SDL_Event event;
//     while(window->isrunning){
//         SDL_PollEvent(&event);
//         if(event.type == SDL_QUIT) window->isrunning = FALSE;
//         // SimulateChunk(&chunk);
//         // DrawChunk2(window, &chunk);
//         // DrawChunkDirtyRects(window, &chunk);
//         SDL_RenderPresent(window->renderer);
//         Clear(window);
//         SDL_Delay(10);
//     }

//     // SDL_Delay(1000);
//     // DeleteChunk(&chunk);
//     DeleteRegion(&region);
//     DeleteWorld(&world);
//     DestroyWindow(&window);

//     CONSOLE("Drawer test success\n\n");
//     return 0;
// }

void ChunkRenderTest(){

    // Window* window;

    // CONSOLE("Chunk render test started\n");

    // CreateWindow(&window, 1080, 720, "ChunkRenderer test", TRUE);
    // Clear(window);
    // // InitParticles();


    // CONSOLE("Chunk test\n");
    // Pos wp = {0, 0};
    // Chunk chunk;
    // CreateChunk(&chunk, 90, 90, wp);
    // // ChunkCreateManyParticlesCircle(&chunk, SAND, 50, 50, 20);
    // ChunkWallBox(&chunk);


    // Region region;
    // CreateRegion(&region, 6, 4, wp);
    // FillRegion(&region);
    // ClearRegion(&region);
    // CONSOLE("works\n");
    // // ChunkCreateManyParticlesCircle(
    // //     &(region.chunks[0]), SAND, 100, 100, 90
    // // );
    // ChunkWallBox(&(region.chunks[0]));
    // CONSOLE("works1\n");

    // StartRenderer(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    // int x = 0;
    // int y = 0;
    // CONSOLE("Chunk Render test\n");
    // // ChunkDraw(window, &chunk, x, y);

    // // long long sum = 0;
    // // long long count = 0;
    // int sum = 0;
    // int count = 0;

    // SDL_Event event;
    // while(window->isrunning){
    //     SDL_PollEvent(&event);
    //     if(event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) window->isrunning = FALSE;
    //     SimulateChunk(&(region.chunks[0]));
    //     // DrawChunkDirtyRects(window, &chunk);
    //     SDL_FillRect(
    //         SDL_GetWindowSurface(window->window), NULL, 
    //         SDL_MapRGB(
    //             SDL_GetWindowSurface(window->window)->format, 
    //             18, 18, 18
    //         )
    //     );
    //     clock_t start = GetTimeNano();
    //     RegionDraw(window, &region, x, y);
    //     clock_t end = GetTimeNano();
    //     printf("%ld\n", (end-start)/1000);
    //     SDL_UpdateWindowSurface(window->window);
    //     // SDL_RenderPresent(window->renderer);
    //     // Clear(window);
    //     SDL_Delay(200);
    // }

    // EndRenderer();
    // // printf("average: %dms", sum/count);
    // DeleteChunk(&chunk);
    // DestroyWindow(&window);
}


// Color getRandomColor3(){
//     Color c = {
//         .r = rand() % 256,
//         .g = rand() % 256,
//         .b = rand() % 256,
//         .a = 255
//     };

//     return c;
// }

void ChunkSpaceTest(){
    
    srand(time(NULL));
    Window* window;
    InitParticles();

    CONSOLE("Chunk render test started\n");

    CreateWindow(&window, 1080, 720, "ChunkRenderer test", FALSE);
    // Clear(window);

    //------------------------
    Pos wp = {0,0};
    Color c = {.rgba = 0xFF0000FF};

    Chunk chunk;
    CreateChunk(&chunk, DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, wp);
    ClearChunk(&chunk);
    ColorChunk(&chunk, c);

    Region region;
    CreateRegion(&region, 3, 2, wp);
    // FillRegion(&region);
    CONSOLE("region\n");
    ClearRegion(&region);
    CONSOLE("region1\n");
    ColorRegion(&region, c);

    ChunkSpace cs;
    CONSOLE("Chunk Space create test\n");
    CreateChunkSpace(&cs, 3, 3, DEFAULT_REGION_WIDTH, DEFAULT_REGION_HEIGHT, DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE);
    // ColorChunkSpace(&cs);
    CONSOLE("Chunk Space arrange test\n");
    ArrangeChunks(&cs);
    CONSOLE("Chunk Space particle create test\n");
    CreateParticlesRectCS(&cs, 40, 40, 50, 50, SAND);
    // int type = CS_GET_TYPE(&cs, 10, 10);
    SetChunkSpace(&cs);

    clock_t start = GetTimeNano();
    for(int i = 0; i < 100; i++)
        SimulateChunkSpace(&cs);
    clock_t end = GetTimeNano();
    printf("%ldms\n", (end-start)/1000000);
    int x = 100;
    int y = 100;
    //----------------------------
    CONSOLE("Chunk Space render test\n");

    StartRenderer(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNK_SIZE, DEFAULT_PARTICLE_SIZE);
    // ColorChunkSpace(&cs);
    CONSOLE("works\n");
    DrawChunkSpace(window, &cs, 0, 0);
    // RegionDraw(window, &region, 0, 0);
    // RegionDraw(window, &cs.regions[0], 0, 0);
    // ChunkDraw(window, &cs.regions[0].chunks[0], 0, 0);
    // ChunkDraw(window, &region.chunks[0], 0, 0);
    // ChunkDraw(window, &chunk, 0, 0);
    SDL_UpdateWindowSurface(window->window);
    // RegionDraw(window, &region, 0, 0);
    CONSOLE("works1\n");
    SDL_Delay(5000);
    EndRenderer();

    //----------------------
    
    CONSOLE("Chunk Space delete test\n");
    DeleteChunkSpace(&cs);
    DestroyWindow(&window);
    CONSOLE("Chunk Space end test\n");

}

int GameGuiTest(){
    return 0;
}


void ParticleInitTest(){
    // CONSOLE("Particle load test started\n");
    
    // CONSOLE("Particle init test\n");
    // InitParticles();
    
    // Color c;
    // ChangeColor(&c, typeColorList[SAND]);
    // CONSOLE("Checking sand color\n");
    // printf("r:%d g:%d b:%d a:%d\n", c.r, c.g, c.b, c.a);
    
    // CONSOLE("Particle load test success\n\n");
}

int ParticleGameTest(){

    // CONSOLE("ParticleGame test started\n");
    // ParticleGame* game;

    // CONSOLE("ParticleGame creation test\n");
    // CreateParticleGame(&game);


    // CONSOLE("Game draw test\n");
    // LoadChunks(game->w);
    // CONSOLE("works000\n");
    // DrawSceneWithoutTexture(game);
    // CONSOLE("works001\n");
    // SDL_RenderPresent(game->win->renderer);

    // CONSOLE("ParticleGame deletion test\n");
    // DeleteParticleGame(&game);

    // CONSOLE("ParticleGame test success\n\n");
    return 0;
}

void RunTests(){
    // WindowTest();
    // ChunkTest();
    // ParticleMapTest();
    // DirtyRectTest();
    // WorldTest();
    // DrawerTest();
    // ParticleInitTest();
    // ParticleGameTest();
    // ChunkRenderTest();
    ChunkSpaceTest();
}


#endif