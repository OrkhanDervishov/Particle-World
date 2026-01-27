// #include "frame_painter.h"

// int DrawRegion(Window* window, Region* region, int x, int y){
//     if(window == NULL) return 1;
//     if(region == NULL) return 2;

//     for(int i = 0; i < region->h; i++)
//     for(int j = 0; j < region->w; j++){
//         DrawChunk2(
//             window, 
//             &REGION_GET_CHUNK(*region, j, i), 
//             x + j*DEFAULT_CHUNK_SIZE, 
//             y + i*DEFAULT_CHUNK_SIZE
//         );
//         printf("x:%d y:%d\n",x + j*DEFAULT_CHUNK_SIZE, y + i*DEFAULT_CHUNK_SIZE);
//     }

//     return 0;
// }


// int DrawBackground(Window* window, Image* image, int x, int y){
//     if(window == NULL) return 1;
//     if(image == NULL) return 2;

//     return 0;
// }


// int DrawBackground2(Window* window, SDL_Surface* image, int x, int y){
//     if(window == NULL) return 1;
//     if(image == NULL) return 2;

//     return 0;
// }

// void DrawChunk2(Window* window, Chunk* chunk, int x, int y){
//     // Particle** map = chunk->pmap.map;
//     // for(int i = 0; i < chunk->h; i++)
//     // for(int j = 0; j < chunk->w; j++)
//     // {
//     //     Particle p = map[i][j];
//     //     if(p.type == AIR) continue;
//     //     SDL_SetRenderDrawColor(
//     //         window->renderer, 
//     //         p.c.r,
//     //         p.c.g,
//     //         p.c.b,
//     //         p.c.a
//     //     );
//     //     SDL_Rect r = {
//     //         .x = p.p.x * PART_SIDE + 1 + 0,
//     //         .y = p.p.y * PART_SIDE + 1 + 0,
//     //         .w = PART_SIDE,
//     //         .h = PART_SIDE
//     //     };
//     //     SDL_RenderFillRect(window->renderer, &r);
//     // }
// }

// void DrawSceneWithoutTexture(ParticleGame* game){
//     Window* window = game->win;
//     World* world = game->w;
//     Color bg_color = game->bg_color;
//     CONSOLE("works1\n");
//     SDL_SetRenderDrawColor(
//         window->renderer,
//         bg_color.r,
//         bg_color.g,
//         bg_color.b,
//         bg_color.a);
//     SDL_RenderClear(window->renderer);
//     CONSOLE("works2\n");

//     SDL_Surface frame;
    
//     int frameParticleWidth = window->w / world->PARTICLE_SIZE;
//     int frameParticleHeight = window->h / world->PARTICLE_SIZE;
//     int frameParticleHalfWidth = frameParticleWidth/2;
//     int frameParticleHalfHeight = frameParticleHeight/2;

//     long worldX = world->chunks[0][0].pmap.map[world->CHUNK_SIZE/2][world->CHUNK_SIZE/2].p.x;
//     long worldY = world->chunks[0][0].pmap.map[world->CHUNK_SIZE/2][world->CHUNK_SIZE/2].p.y;
//     long startX = worldX - frameParticleHalfWidth;
//     long startY = worldY - frameParticleHalfHeight;
    
//     CONSOLE("works3\n");
//     for(int i = -frameParticleHalfHeight + 1; i < frameParticleHalfHeight - 1; i++)
//     for(int j = -frameParticleHalfWidth + 1; j < frameParticleHalfWidth - 1; j++){
//         //DrawParticle();
//         printf("i:%d j:%d\n", i, j);
//         printf("wi:%d wj:%d\n", worldY + i, worldX + j);
//         Particle* p = WORLD_GET_PARTICLE(world, worldX + j, worldY + i);
//         CONSOLE("works4\n");
//         SDL_SetRenderDrawColor(
//             window->renderer,
//             p->c.r,
//             p->c.g,
//             p->c.b,
//             p->c.a
//         );
//         SDL_Rect r = {
//             (p->p.x - startX) * world->PARTICLE_SIZE + 1, 
//             (p->p.y - startY) * world->PARTICLE_SIZE + 1, 
//             world->PARTICLE_SIZE, 
//             world->PARTICLE_SIZE};
//         CONSOLE_RECT(r);
//         SDL_RenderFillRect(window->renderer, &r);
//     }
// }

// // void DrawCage(Window* win, int r, int g, int b, int a)
// // {
// //     SDL_SetRenderDrawColor(win->renderer, r, g, b, a);

// //     for(int i = 0; i < win->w; i += PART_SIDE){
// //         SDL_RenderDrawLine(win->renderer, i, 0, i, win->h);
// //     }
    
// //     for(int i = 0; i < win->h; i += PART_SIDE){
// //         SDL_RenderDrawLine(win->renderer, 0, i, win->w, i);
// //     }
// // }

// // void DrawCircle(Window* win, int px, int py, int rad){
// //     int x = rad - 1;
// //     int y = 0;
// //     int dx = 1;
// //     int dy = 1;
// //     int err = dx - (rad << 1);

// //     SDL_SetRenderDrawColor(win->renderer, MWHITE);
// //     while(x >= y){
// //         int cx = x * CSIDE;
// //         int cy = y * CSIDE;
// //         SDL_Rect r0 = {px + cx, py + cy, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r0);
// //         SDL_Rect r1 = {px + cy, py + cx, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r1);
// //         SDL_Rect r2 = {px - cy, py + cx, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r2);
// //         SDL_Rect r3 = {px - cx, py + cy, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r3);
// //         SDL_Rect r4 = {px - cx, py - cy, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r4);
// //         SDL_Rect r5 = {px - cy, py - cx, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r5);
// //         SDL_Rect r6 = {px + cy, py - cx, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r6);
// //         SDL_Rect r7 = {px + cx, py - cy, CSIDE, CSIDE};
// //         SDL_RenderFillRect(win->renderer, &r7);

// //         if(err <= 0){
// //             y++;
// //             err += dy;
// //             dy += 2;  
// //         }
// //         if(err > 0){
// //             x--;
// //             dx += 2;
// //             err += dx - (rad << 1);
// //         }
// //     }
// // }

// void DrawChunkDirtyRects(Window* win, Chunk* chunk){
//     SDL_SetRenderDrawColor(win->renderer, MYELLOW);
//     ListNode* r = chunk->dirty_rect_list->head;
//     while(r != NULL){
//         SDL_Rect r1;
//         Rect r2 = *(Rect*)(r->val);
//         r1.x = r2.x * PART_SIDE;
//         r1.y = r2.y * PART_SIDE;
//         r1.w = r2.w * PART_SIDE;
//         r1.h = r2.h * PART_SIDE;
//         SDL_RenderDrawRect(win->renderer, &r1);
//         r = r->next;
//     }
// }







// // // void DrawScene(Window* win, ParticleSimulator* sim, SDL_Texture* bg, SDL_Rect* src, SDL_Rect* dest){

// // //     SDL_SetRenderDrawColor(win->renderer, MGOOD_GRAY);
// // //     SDL_RenderClear(win->renderer);

// // //     if(hm_mode){
// // //         SDL_SetRenderDrawColor(win->renderer, MBLACK);
// // //         SDL_RenderFillRect(win->renderer, dest);

// // //         for(int i = 0; i < sim->rows; i++){
// // //             for(int j = 0; j < sim->cols; j++){
// // //                 Particle p = sim->pMap[i][j];
// // //                 if(p.type == AIR) continue;

// // //                 int h = p.heat;
// // //                 SDL_SetRenderDrawColor(win->renderer,
// // //                      h > 255 ? 255 : h, 
// // //                      h / 50, 
// // //                      (h == 0 ? 30 : 0), 
// // //                      255);
// // //                 SDL_Rect r = {
// // //                     p.p.x * sim->pSide + 1, 
// // //                     p.p.y * sim->pSide + 1, 
// // //                     sim->pSide, 
// // //                     sim->pSide};
// // //                 SDL_RenderFillRect(win->renderer, &r);
// // //             }
// // //         }
// // //     } else {
// // //         SDL_RenderCopy(win->renderer, bg, src, dest);

// // //         for(int i = 0; i < sim->rows; i++){
// // //             for(int j = 0; j < sim->cols; j++){
// // //                 if(sim->pMap[i][j].id < 0) continue;
// // //                 SDL_SetRenderDrawColor(
// // //                     win->renderer,
// // //                     sim->pMap[i][j].c.r,
// // //                     sim->pMap[i][j].c.g,
// // //                     sim->pMap[i][j].c.b,
// // //                     sim->pMap[i][j].c.a
// // //                 );
// // //                 SDL_Rect r = {
// // //                     sim->pMap[i][j].p.x * sim->pSide + 1, 
// // //                     sim->pMap[i][j].p.y * sim->pSide + 1, 
// // //                     sim->pSide, 
// // //                     sim->pSide};
// // //                 SDL_RenderFillRect(win->renderer, &r);
// // //             }
// // //         }
// // //     }
// // // }