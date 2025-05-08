#ifndef DATA_H
#define DATA_H

#include "common.h"
#include "particle.h"
#include "quadtree.h"

int chunkX = 16;
int chunkY = 16;


typedef struct{
    Particle** data;
} Chunk;


typedef struct{
    QuadNode* root;
} Data;


void CreateChunk();
void InsertData();
Particle* GetData();



void InsertData(int x, int y, Particle* p){
    
}


Particle* GetData(int x, int y){
    return NULL;
}

#endif //DATA_H