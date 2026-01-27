#include "world.h"


int CreateWorld(World** world){
    *world = (World*)malloc(sizeof(World));
    (*world)->chunk_load_distance = DEFAULT_CHUNK_LOAD_DISTANCE;
    (*world)->chunks = Malloc2D(MAX_WORLD_ROWS, MAX_WORLD_COLS, sizeof(Chunk));
    (*world)->CHUNK_SIZE = DEFAULT_CHUNK_SIZE;
    (*world)->PARTICLE_SIZE = DEFAULT_PARTICLE_SIZE;
    (*world)->seed = 10;

    Chunk chunk;
    for(int i = 0; i < MAX_WORLD_ROWS; i++)
    for(int j = 0; j < MAX_WORLD_COLS; j++){
        (*world)->chunks[i][j] = chunk;
        (*world)->chunks[i][j].active = FALSE;
    }

    return 0;
}

int DeleteWorld(World** world){

    Free2D((*world)->chunks, MAX_WORLD_ROWS);
    free(*world);
    return 0;
}

void LoadChunks(World* world){
    for(int i = CHUNK_START(world) - 1; i < CHUNK_END(world) + 1; i++)
    for(int j = CHUNK_START(world) - 1; j < CHUNK_END(world) + 1; j++){
        Pos p = {
            .x = i,
            .y = j
        };

        CreateChunk(&(world->chunks[i][j]), world->CHUNK_SIZE, world->CHUNK_SIZE, p);
        world->chunks[i][j].active = TRUE;
    }
}

void ClearWorld(World* world){
    for(int i = 0; i < MAX_WORLD_ROWS; i++)
    for(int j = 0; j < MAX_WORLD_COLS; j++)
        ClearChunk(&world->chunks[i][j]);
}

void SimulateWorld(World* world){
    for(int i = CHUNK_START(world); i < CHUNK_END(world); i++)
    for(int j = CHUNK_START(world); j < CHUNK_END(world); j++)
        SimulateChunk(&world->chunks[i][j]);
}




void SetChunkLoadDistance(World* world, int distance){
    if(distance <= 0 || distance > MAX_CHUNK_LOAD_DISTANCE) return;
    world->chunk_load_distance = distance;
}