#include "simulator.h"

int selectedType = SAND;

int PART_SIDE = 4;
int DELAY = 11;
int RADIUS = 1;
int CSIDE = 6;

// Implementations

// TODO
// int InitParticleSimulator(ParticleSimulator** sim, int w, int h, int ps){
//
//     *sim = (ParticleSimulator*)malloc(sizeof(ParticleSimulator));
//    
//     (*sim)->pSide = ps;
//     (*sim)->rows = h/ps;
//     (*sim)->cols = w/ps;
//     (*sim)->partCount = 0;
//
//     (*sim)->pMap = (Particle**)Malloc2D((*sim)->rows, (*sim)->cols, sizeof(Particle));
//     InitDirtyRectList(&rectList);
//
//     if((*sim)->pMap == NULL) return 1;
//
//     ClearMap((*sim));
//     return 0;
// }
//
// // TODO
// void DestroyParticleSimulator(ParticleSimulator** sim){
//     DestroyDirtyRectList(&rectList);
//
//     for(int i = 0; i < (*sim)->rows; i++){
//         free((*sim)->pMap[i]);
//     }
//     free((*sim)->pMap);
//
//     free(*sim);
// }

// void ChangeParticleSimulator(ParticleSimulator** sim, int w, int h, int ps){
//     DestroyParticleSimulator(sim);
//     InitParticleSimulator(sim, w, h, ps);
// }

// void ClearMap(ParticleMap* pmap){
//     for(int i = 0; i < pmap->rows; i++){
//         for(int j = 0; j < pmap->cols; j++){
//             pmap->map[i][j].id = -1;
//             pmap->map[i][j].heat = 0;
//             pmap->map[i][j].type = AIR;

//             pmap->partCount = 0;
//         }
//     }
// }


// TODO
// void SimulateHeatMap(ParticleSimulator* sim){
//     for(int i = 1; i < sim->rows - 1; i++){
//         for(int j = 1; j < sim->cols - 1; j++){ 
//
//             Particle* p = &sim->pMap[i][j];
//             if(p->type == AIR) continue;
//
//             Particle* neighbors[] = {
//                 &sim->pMap[i - 1][j],
//                 &sim->pMap[i + 1][j],
//                 &sim->pMap[i][j - 1],
//                 &sim->pMap[i][j + 1]
//             };
//
//             for(int i = 0; i < 4; i++){
//                 Particle* other = neighbors[i];
//                 if(other->type == AIR) continue;
//                 int h = (p->heat + other->heat) / 2;
//                 p->heat = h;
//                 other->heat = h;
//             }
//         }
//     }
// }

void SimulateChunkComplete(Chunk* chunk, int cs_x, int cs_y){
    int rows = chunk->h;
    int cols = chunk->w;

    int mov = 0;
    
    // CONSOLE("works10\n");
    for(int i = rows-1; i >= 0; i--){
        mov = mov ? 0 : 1;
        int nj = mov ? cols-1 : 0;
        int add = mov ? -1 : 1;
        for(int j = nj; mov ? j >= 0 : j < cols; j += add){
            int x = j;
            int y = i;

            if(CHUNK_GET_STATE(*chunk, j, i) <= P_USED) continue;
            CHUNK_GET_STATE(*chunk, j, i) = P_USED;
            
            // CONSOLE("works11\n");
            if(typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)] == NULL) {continue;}
            // printf("ctype:%d\n", CHUNK_GET_TYPE(*chunk, x, y));
            // printf("cx:%d cy:%d\n", x, y);
            // printf("csx:%d csy:%d\n", x + cs_x, y + cs_y);
            // CONSOLE("works1234\n");
            typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)](x + cs_x, y + cs_y);
            // CONSOLE("works13\n");
        }
    }

    for(int i = 0; i < chunk->size; i++){
        if(chunk->state[i] == P_USED){
            chunk->state[i] = P_FRESH;
        }
    }
}




Rect SimulateChunkRect(Chunk* chunk, Rect rect){
    // printf("works\n");
    Rect res = {-1, -1, -1, -1};
    if(rect.w == 0 || rect.h == 0) return res;

    int rows = chunk->h;
    int cols = chunk->w;

    int minx = cols, miny = rows;
    int maxx = 0, maxy = 0;
    int mov = 0;


    for(int i = rect.y; i < rect.y + rect.h; i++){
        mov = mov ? 0 : 1;
        int nj = mov ? rect.x + rect.w : rect.x;
        int add = mov ? -1 : 1;
        for(int j = nj; mov ? j >= rect.x : j < rect.x + rect.w; j += add){
            int x = j;
            int y = i;

            if(CHUNK_GET_STATE(*chunk, j, i) <= P_USED) continue;
            CHUNK_GET_STATE(*chunk, j, i) = P_USED;
            
            // map[y][x].p.y >= rows-1 || 
            if(typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)] == NULL) {continue;}
            
            // Particle logic
            bool changed = typeFuncList[CHUNK_GET_TYPE(*chunk, j, i)](x, y);

            // Rectangle checking
            if(changed == TRUE){
                minx = minx < x ? minx : x;
                miny = miny < y ? miny : y;
                maxx = maxx > x ? maxx : x;
                maxy = maxy > y ? maxy : y;
            }
        }
    }

    int add = 2;
    res.x = minx - add;
    res.y = miny - add;
    res.w = maxx - res.x + add;
    res.h = maxy - res.y + add;
    
    CorrectDirtyRect(rows, cols, &res);

    return res;
}


// void SimulateAll(ParticleMap* pmap){
    // int mov = 0;
    // for(int i = sim->rows - 1; i >= 0; i--){
    //     mov = mov ? 0 : 1;
    //     int nj = mov ? sim->cols - 1 : 0;
    //     for(int j = nj; mov ? j >= 0 : j < sim->cols; mov ? j-- : j++){
    //        
    //         int oldx = j;
    //         int oldy = i;
    //
    //         if(sim->pMap[oldy][oldx].id > 0){
    //             if(sim->pMap[oldy][oldx].p.y >= sim->rows-1 || typeFuncList[sim->pMap[oldy][oldx].type] == NULL) {continue;}
    //            
    //             int x = oldx;
    //             int y = oldy;
    //
    //             // Particle logic
    //             typeFuncList[sim->pMap[oldy][oldx].type](sim, &x, &y);
    //
    //             // Particle swapping
    //             if(y == oldy && x == oldx){
    //                 if(sim->pMap[y][x].id == -2){
    //                     sim->pMap[y][x].type = AIR;
    //                     sim->pMap[y][x].id = -1;
    //                 }
    //                 continue;
    //             }
    //             if(x < 0 || x >= sim->cols) continue;
    //             if(y < 0 || y >= sim->rows) continue;
    //
    //             Particle temp = sim->pMap[y][x];
    //             sim->pMap[y][x] = sim->pMap[oldy][oldx];
    //             sim->pMap[oldy][oldx] = temp;
    //             sim->pMap[y][x].p.x = x;
    //             sim->pMap[y][x].p.y = y;
    //             sim->pMap[oldy][oldx].p.x = oldx;
    //             sim->pMap[oldy][oldx].p.y = oldy;
    //             sim->pMap[y][x].id = 0;
    //         }
    //     }
    // }
    // for(int i = 0; i < sim->rows; i++){
    //     for(int j = 0; j < sim->cols; j++){
    //         if(sim->pMap[i][j].id == 0) sim->pMap[i][j].id = 1;
    //     }
    // }
// }

// Rect SimulateRect(ParticleMap* pmap, Rect rect){
//     Rect z = {.w = -1, .h = -1, .x = -1, .y = -1};
//     if(rect.x == 0 && rect.y == 0 && rect.w == 0 && rect.h == 0) return z;
//     Rect newRect;
//     Particle** map = pmap->map;
//     int rows = pmap->rows;
//     int cols = pmap->cols;
//     int minx = cols, miny = rows;
//     int maxx = 0, maxy = 0;
//     int mov = 0;
//     for(int i = rect.y; i < rect.y + rect.h; i++){
//         mov = mov ? 0 : 1;
//         int nj = mov ? rect.x + rect.w : rect.x;
//         int add = mov ? -1 : 1;
//         for(int j = nj; mov ? j >= rect.x : j < rect.x + rect.w; j += add){
//             int x = j;
//             int y = i;
//             if(map[y][x].id > 0){
//                 if(map[y][x].p.y >= rows-1 || typeFuncList[map[y][x].type] == NULL) {continue;}
//                 // Particle logic
//                 map[y][x].id = 0;
//                 bool changed = typeFuncList[map[y][x].type](x, y);
//                 // Rectangle checking
//                 if(changed == TRUE){
//                     minx = minx < x ? minx : x;
//                     miny = miny < y ? miny : y;
//                     maxx = maxx > x ? maxx : x;
//                     maxy = maxy > y ? maxy : y;
//                 }
//             }
//         }
//     }
//     int add = 2;
//     newRect.x = minx - add;
//     newRect.y = miny - add;
//     newRect.w = maxx - newRect.x + add;
//     newRect.h = maxy - newRect.y + add;
//     CorrectDirtyRect(rows, cols, &newRect);
//     return newRect;
// }

// void SimulateRects(Chunk* chunk){
//     ListNode* r = chunk->dirty_rect_list->head;
//     while(r != NULL){
//         Rect newRect = SimulateChunkRect(chunk, (*(Rect*)r->val));
//         // Rect* rect;
//         // CreateRect(&rect, newRect.x, newRect.y, newRect.w, newRect.h);
//         // CONSOLE_RECT(newRect);
//         // if(newRect.h <= 2*add || newRect.w < 1){
//         if(newRect.h <= 1 || newRect.w <= 1){
//             // CONSOLE_RECT(newRect);
//             ListNode* nodeToDelete = r;
//             r = r->next;
//             DeleteDirtyRect(list, nodeToDelete);
//         }
//         else {
//             // Rect* rect = (Rect*)r->val;
//             *((Rect*)r->val) = newRect;
//             CONSOLE_RECT(*((Rect*)r->val));
//             r = r->next;
//         }
//     }
//     for(int i = 0; i < chunk->size; i++){
//         if(chunk->state[i] == 0){
//             chunk->state[i] = 1;
//         }
//     }
// }

void SimulateChunk(Chunk* chunk){
    ListNode* r = chunk->dirty_rect_list->head;

    while(r != NULL){
        Rect newRect = SimulateChunkRect(chunk, (*(Rect*)r->val));
        if(newRect.h <= 1 || newRect.w <= 1){
            ListNode* nodeToDelete = r;
            r = r->next;
            DeleteDirtyRect(chunk->dirty_rect_list, nodeToDelete);
        }
        else {
            *((Rect*)r->val) = newRect;
            CONSOLE_RECT(*((Rect*)r->val));
            r = r->next;
        }
    }

    for(int i = 0; i < chunk->size; i++){
        if(chunk->state[i] == P_USED){
            chunk->state[i] = P_FRESH;
        }
    }
}

void SimulateChunkSpace(ChunkSpace *cs){

    for(int i = cs->simEndY-1; i >= cs->simStartY; i--)
    for(int j = cs->simStartX; j < cs->simEndX; j++){
        SimulateChunkComplete(&CS_GET_ARRAY_CHUNK(cs, j, i), j*cs->chunk_width, i*cs->chunk_height);
    }
}


// Simulation control functions
void SlowDownParticleSimulator(){
    DELAY++;
}

void SpeedUpParticleSimulator(){
    if(DELAY == 0) return;
    DELAY--;
}

void IncreaseBrushRadius(){
    if(RADIUS > 10) return;
    RADIUS++;
}

void DecreaseBrushRadius(){
    if(RADIUS == 1) return;
    RADIUS--;
}

