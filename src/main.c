#include <stdio.h>
#include "tests.h"
// #include "engine_lib.h"
// #include "particle_data.h"
// #include "partsim/simulator.h"
// #include "partsim/particle.h"
// #include "game_gui.h"
// #include "particle_game.h"
// #include "rbsim/rigid.h"
// #include "chessgame/chess.h"

#define TESTS
// #define PART_TEST
// #define RIGID_TEST
// #define CHESS_TEST


int RigidTest(); 
int Chess();

int main(int argc, char* argv[]){

#ifdef RIGID_TEST
    RigidTest();
#endif
#ifdef CHESS_TEST
    if(Chess()) printf("chess failed\n");
#endif
#ifdef PART_TEST
    RunParticleGame();
#endif
#ifdef TESTS
    RunTests();
#endif

    return 0;
}


// int Chess(){
//     Window* cwin;
//     ChessSimulator* csim;
//     if(InitWindow(&cwin, SCR_WIDTH, SCR_HEIGHT, WIN_TITLE)) return 1;
//     if(InitChessSimulator(&csim, cwin, SimWidth, SimWidth, SimWidth/8)) return 1;

//     Color c1 = {20, 20, 20, 255};
//     Color c2 = {120, 120, 120, 255};
//     Color c3 = {255, 243, 128, 100};
//     AssignTileColors(c1, c2, c3);
//     ChessGame(csim);
//     DestroyChessSimulator(&csim);
//     DestroyWindow(&cwin);
//     return 0;
// }


// int RigidTest(){
//     // Inits
//     if(InitWindow(&win, SCR_WIDTH, SCR_HEIGHT, WIN_TITLE)) return 1;
//     if(InitRigidSimulator(&rsim, R_Sim_Width, R_Sim_Height)) return 1;

//     SDL_Surface* icon = SDL_LoadBMP(ICON_PATH);
//     if(icon == NULL) printf("fail\n");
//     SDL_SetWindowIcon(win->window, icon);

//     SDL_RendererInfo info;
//     SDL_GetRendererInfo(win->renderer, &info);
//     printf("%s\n", info.name);

//     clock_t start = 0, end = 1;
//     // Loop
//     SDL_Event event;

//     // CreateRect(rsim, 10, 10, 100, 100);
//     // CreateRect(rsim, 200, 90, 100, 100);
//     // CreateRect(rsim, 100, 90, 100, 100);
//     // CreateRect(rsim, 200, 480, 100, 100);
//     // CreateRect(rsim, 400, 800, 100, 100);
//     // CreateRect(rsim, 500, 600, 100, 100);
//     // RigidObj* rect1 = GetObjectById(rsim, 0);
//     // RigidObj* rect2 = GetObjectById(rsim, 1);

//     int xmove = -2;
//     int ymove = -2;

//     Polygon* p;
//     CreateTriangle(rsim, &p, 100, 100, 200, 200, 50, 200);

//     while(win->isrunning){
//         deltaTime = end - start;
//         sumTime += deltaTime;

//         if(sumTime > 500){
//             fps = 1000 / (end - start);
//             sumTime = 0;
//             sprintf(win->title, "FPS: %d", fps);
//             SDL_SetWindowTitle(win->window, win->title);
//         }
//         start = clock();

//         // Input handling
//         // ProcessInput(event, win, sim, &color);
//         ProcessRigidInput(event, win, rsim);

//         // MovePolygon(rsim, p, 1, 1);

//         // Render
//         // int mx, my;
//         // SDL_GetMouseState(&mx, &my);
//         DrawObjects(win, rsim);
//         DrawPolygon(win, rsim, p);

        
        
//         // End Frame
//         SDL_RenderPresent(win->renderer);
//         SDL_Delay(Delay);
//         end = clock();
//     }

//     DeletePolygon(rsim, &p);
//     // Destroy
//     SDL_FreeSurface(icon);
//     DestroyRigidSimulator(&rsim);
//     DestroyWindow(&win);
// }
