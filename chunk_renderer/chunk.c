#include "chunk.h"


int CreateChunk(Chunk* chunk, int w, int h, Pos worldPos){

    chunk->w = w;
    chunk->h = h;
    chunk->active = TRUE;
    chunk->chunk_world_pos = worldPos;
    chunk->chunk_pos.x = 0;
    chunk->chunk_pos.y = 0;

    CreateParticleMap(&chunk->pmap, h, w);
    ClearMap(&chunk->pmap);

    InitDirtyRectList(&chunk->dirty_rect_list);
    return 0;
}

void DeleteChunk(Chunk* chunk){
    DeleteParticleMap(&chunk->pmap);
    DeleteList(&chunk->dirty_rect_list);
}

void ClearChunk(Chunk* chunk){
    ClearMap(&chunk->pmap);
    ClearList(chunk->dirty_rect_list);
}

void ActivateChunk(Chunk* chunk){
    chunk->active = TRUE;
}

void SimulateChunk(Chunk* c){
    // printf("%s\n", typeNameList[CHUNK_GET_PARTICLE(c, 100, 32).type]);
    SimulateRects(&c->pmap, c->dirty_rect_list);
}

void ChunkFillWithAir(Chunk* chunk){
    for(int i = 0; i < chunk->h; i++)
    for(int j = 0; j < chunk->w; j++)
    {
        chunk->pmap.map[i][j].c.r = 255;
        chunk->pmap.map[i][j].c.b = 0;
        chunk->pmap.map[i][j].c.g = 0;
        chunk->pmap.map[i][j].c.a = 255;
        
        chunk->pmap.map[i][j].id = -1; 
        chunk->pmap.map[i][j].p.x = j;
        chunk->pmap.map[i][j].p.y = i;
        chunk->pmap.map[i][j].type = AIR;
    }
}


// Chunk data manipulation

// Particles

void ChunkCreateParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y){
    if(chunk_x >= 0 && chunk_y >= 0 && chunk_x < chunk->w && chunk_y < chunk->h){
        if(CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y).id != -1) return;

        Particle part = {
            .p = {
                .x          = chunk_x,
                .y          = chunk_y
            },
            .id             = 1,
            .xvel           = 2,
            .yvel           = 1,
            .life_t         = rand() % 30,
            .effect_t       = rand() % 30,
            .heat           = (CHECK_FLAG(typeIFlagsList[type], BURNING) ? 5000 : 0),
            .type           = type,
            .iflags         = typeIFlagsList[type],
            .durability     = typeDurabilityList[type]
        };
        ChangeColor(&part.c, typeColorList[type]);
        
        CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y) = part;
        chunk->pmap.partCount++;
    }
}

void ChunkCreateReplaceParticle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y){
    if(CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y).id < 0) chunk->pmap.partCount++;

    Particle part = {
        .p = {
            .x          = chunk_x,
            .y          = chunk_y
        },
        .id             = 1,
        .xvel           = 2,
        .yvel           = 1,
        .life_t         = rand() % 30,
        .effect_t       = rand() % 30,
        .heat           = (CHECK_FLAG(typeIFlagsList[type], BURNING) ? 5000 : 0),
        .type           = type,
        .iflags         = typeIFlagsList[type],
        .durability     = typeDurabilityList[type]
    };
    ChangeColor(&part.c, typeColorList[type]);
    
    CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y) = part;
}

void ChunkReplaceParticle(Chunk* chunk, int sx, int sy, int dx, int dy){
    Particle temp;
    SWAP(CHUNK_GET_PARTICLE(chunk, sx, sy), CHUNK_GET_PARTICLE(chunk, dx, dy), temp);
}

void ChunkDeleteParticle(Chunk* chunk, int chunk_x, int chunk_y){
    // if(sim->pMap[py][px].id > 0){
        CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y).id = -1;
        CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y).heat = 0;
        CHUNK_GET_PARTICLE(chunk, chunk_x, chunk_y).type = AIR;

        chunk->pmap.partCount--;
    // }
}

void ChunkSwapParticles(Chunk* chunk, Particle* p1, Particle* p2){
    Particle tp;
    int t;

    SWAP(*p1, *p2, tp);
    SWAP(p1->p.x, p2->p.x, t);
    SWAP(p1->p.y, p2->p.y, t);
}

void ChunkCreateManyParticlesCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad){
    int begy = chunk_y - rad;
    int begx = chunk_x - rad;
    int endy = chunk_y + rad;
    int endx = chunk_x + rad;
    int srad = rad * rad;

    for(int i = begy; i < endy; i++){
        int di = abs(chunk_y - i);
        for(int j = begx; j < endx; j++){
            int dj = abs(chunk_x - j);
            if(di*di + dj*dj < srad)
                if(j >= 0 && i >= 0 && j < chunk->w && i < chunk->h)
                    if(CHUNK_GET_PARTICLE(chunk, j, i).id < 0)
                        ChunkCreateReplaceParticle(chunk, type, j, i);
        }
    }
}

void ChunkCreateManyParticlesWithRarityCircle(Chunk* chunk, ParticleType type, int chunk_x, int chunk_y, int rad, int rarity){
    int begy = chunk_y - rad;
    int begx = chunk_x - rad;
    int endy = chunk_y + rad;
    int endx = chunk_x + rad;
    int srad = rad * rad;

    for(int i = begy; i < endy; i++){
        int di = abs(chunk_y - i);
        for(int j = begx; j < endx; j++){
            int dj = abs(chunk_x - j);
            if(di*di + dj*dj < srad)
            if(rand() % rarity != 1) continue;
                if(j >= 0 && i >= 0 && j < chunk->w && i < chunk->h)
                    if(CHUNK_GET_PARTICLE(chunk, j, i).id < 0)
                        ChunkCreateReplaceParticle(chunk, type, j, i);
        }
    }
}

void ChunkDeleteManyParticlesCircle(Chunk* chunk, int chunk_x, int chunk_y, int rad){
    int begy = chunk_y - rad;
    int begx = chunk_x - rad;
    int endy = chunk_y + rad;
    int endx = chunk_x + rad;
    int srad = rad * rad;

    for(int i = begy; i < endy; i++){
        if(i <= 0 || i >= chunk->h - 1) continue;
        int di = abs(chunk_y - i);
        for(int j = begx; j < endx; j++){
            if(j <= 0 || j >= chunk->w - 1) continue;
            int dj = abs(chunk_x - j);
            if(di*di + dj*dj < srad)
                ChunkDeleteParticle(chunk, j, i);
        }
    }
}


// ParticleSimulator
void ChunkWallBox(Chunk* chunk){
    
    for(int j = 0; j < chunk->w; j++){
        ChunkCreateParticle(chunk, WALL, j, 0);
        ChunkCreateParticle(chunk, WALL, j, chunk->h - 1);
    }
    for(int i = 1; i < chunk->h - 1; i++){
        ChunkCreateParticle(chunk, WALL, 0, i);
        ChunkCreateParticle(chunk, WALL, chunk->w - 1, i);
    }

}

// ParticleSimulator
void ChunkExplosion(Chunk* chunk, int chunk_x, int chunk_y, int rad, int power, int replaceWith){
    int x = rad - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (rad << 1);

    while(x >= y){
        int cx = x;
        int cy = y;
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x + cx, chunk_y + cy, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x + cy, chunk_y + cx, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x - cy, chunk_y + cx, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x - cx, chunk_y + cy, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x - cx, chunk_y - cy, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x - cy, chunk_y - cx, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x + cy, chunk_y - cx, power, replaceWith);
        ChunkDestructionLine(chunk, chunk_x, chunk_y, chunk_x + cx, chunk_y - cy, power, replaceWith);

        if(err <= 0){
            y++;
            err += dy;
            dy += 2;  
        }
        if(err > 0){
            x--;
            dx += 2;
            err += dx - (rad << 1);
        }
    }
}

// ParticleSimulator
void ChunkDestructionLine(Chunk* chunk, int cx0, int cy0, int cx1, int cy1, int power, int replaceWith){
    int dx = abs(cx1 - cx0);
    int sx = cx0 < cx1 ? 1 : -1;
    int dy = -abs(cy1 - cy0);
    int sy = cy0 < cy1 ? 1 : -1;
    int error = dx + dy;

    while(1){
        if(cx0 >= 0 && cy0 >= 0 && cx0 < chunk->w && cy0 < chunk->h){
            if(CHUNK_GET_PARTICLE(chunk, cx0, cy0).type == WALL) break;
            ChunkCreateReplaceParticle(chunk, replaceWith, cx0, cy0);
            CHUNK_GET_PARTICLE(chunk, cx0, cy0).heat = power * 10;
            if (cx0 == cx1 && cy0 == cy1) break;
            int e2 = 2 * error;
            if(e2 >= dy){
                if(cx0 == cx1) break;
                error += dy;
                cx0 += sx;
            }
            if(e2 <= dx){
                if(cy0 == cy1) break;
                error += dx;
                cy0 += sy;
            }
        }
        else{
            break;
        }
    }

    // // int dx = abs(px1 - px0);
    // // int sx = px0 < px1 ? 1 : -1;
    // // int dy = -abs(py1 - py0);
    // // int sy = py0 < py1 ? 1 : -1;
    // // int error = dx + dy;

    // // Color c;
    // // ChangeColor(&c, FIRE_COLORS);

    // // while (1) {
    // //     CreateReplaceParticle(sim, px0, py0, &c, FIRE);

    // //     if (px0 == px1 && py0 == py1) break;

    // //     int e2 = 2 * error;

    // //     if (e2 >= dy) {
    // //         error += dy;
    // //         px0 += sx;
    // //     }

    // //     if (e2 <= dx) {
    // //         error += dx;
    // //         py0 += sy;
    // //     }
    // // }
}