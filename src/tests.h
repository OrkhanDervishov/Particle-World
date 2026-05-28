#ifndef TESTS_H
#define TESTS_H

#include "timing.h"
#include "particle_load.h"
#include "chunk.h"
#include "region.h"
#include "chunk_space.h"
// #include "world.h"
#include "game/particle_game.h"
#include "game.h"
#include "chunk_renderer_sw.h"
#include "dirtyrect.h"
#include "particle_data.h"
#include "simulator.h"


int ParticleGameTest(){

    ParticleGame* game;
    if(CreateParticleGame(&game)) printf("failed\n");
    BuildLabEnv(game);
    RunParticleGame(game);
    DeleteParticleGame(&game);
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
    ParticleGameTest();
    // GuiTest();
    // ChunkRenderTest();
    // ChunkSpaceTest();
}


#endif