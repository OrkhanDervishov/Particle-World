#include "rigid.h"

int R_Sim_Height = 720;
int R_Sim_Width = 1080;


int InitRigidSimulator(RigidSimulator** rsim, int w, int h){
    *rsim = (RigidSimulator*)malloc(sizeof(RigidSimulator));
    if(rsim == NULL) return 1;

    (*rsim)->objList = (RigidObj*)malloc(sizeof(RigidObj));
    if((*rsim)->objList == NULL) return 1;

    (*rsim)->h = h;
    (*rsim)->w = w;
    (*rsim)->countObj = 0;
    (*rsim)->last_id = 0;

    return 0;
}


void DestroyRigidSimulator(RigidSimulator** rsim){  
    free((*rsim)->objList);
    free(*rsim);
}


void CreateRect(RigidSimulator* rsim, int x, int y, int w, int h){
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    RigidObj obj;
    obj.id = rsim->last_id++;
    obj.rect = rect;
    rsim->objList[rsim->countObj++] = obj;
}


void DrawObjects(Window* win, RigidSimulator* rsim){
    SDL_SetRenderDrawColor(win->renderer, 100, 100, 100, 255);
    SDL_RenderClear(win->renderer);
    for(int i = 0; i < rsim->countObj; i++){
        SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(win->renderer, &rsim->objList[i].rect);
    }
}


void ProcessRigidInput(SDL_Event event, Window* win, RigidSimulator* rsim)
{
    int mx, my;
    int state = SDL_GetMouseState(&mx, &my);
    // Other events
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) win->isrunning = 0;

        if(event.type == SDL_KEYDOWN){
            if(QUIT_BUTTON) win->isrunning = 0;
        }
    }
}

RigidObj* GetObjectById(RigidSimulator* rsim, int id){
    for(int i = 0; i < rsim->countObj; i++){
        if(rsim->objList[i].id == id){
            return &rsim->objList[i];
        }
    }
    return NULL;
}


int CheckCollisionById(RigidSimulator* rsim, int id1, int id2){
    RigidObj* obj1 = GetObjectById(rsim, id1);
    RigidObj* obj2 = GetObjectById(rsim, id2);

    int x11 = obj1->rect.x;
    int x12 = obj1->rect.x + obj1->rect.w;
    int x21 = obj2->rect.x;
    int x22 = obj2->rect.x + obj2->rect.w;

    int y11 = obj1->rect.y;
    int y12 = obj1->rect.y + obj1->rect.h;
    int y21 = obj2->rect.y;
    int y22 = obj2->rect.y + obj2->rect.h;

    if((x11 > x22 && x12 < x21) || (x11 < x22 && x12 > x21)){
        if((y11 > y22 && y12 < y21) || (y11 < y22 && y12 > y21)){
            return 1;
        }
    }

    return 0;
}

void MoveObjectById(RigidSimulator* rsim, int id, int x, int y){
    RigidObj* obj = GetObjectById(rsim, id);
    obj->rect.x += x;
    obj->rect.y += y;
}


int CheckBorderCollision(RigidSimulator* rsim, RigidObj* obj){
    if(obj->rect.x < 0 || obj->rect.x + obj->rect.w > rsim->w){
        return 1;
    }
    if(obj->rect.y < 0 || obj->rect.y + obj->rect.h > rsim->h){
        return 2;
    }
    return 0;
}

int CheckCollision(RigidSimulator* rsim, RigidObj* obj1, RigidObj* obj2){
    int x11 = obj1->rect.x;
    int x12 = obj1->rect.x + obj1->rect.w;
    int x21 = obj2->rect.x;
    int x22 = obj2->rect.x + obj2->rect.w;

    int y11 = obj1->rect.y;
    int y12 = obj1->rect.y + obj1->rect.h;
    int y21 = obj2->rect.y;
    int y22 = obj2->rect.y + obj2->rect.h;

    if((x11 > x22 && x12 < x21) || (x11 < x22 && x12 > x21)){
        if((y11 > y22 && y12 < y21) || (y11 < y22 && y12 > y21)){
            return 1;
        }
    }

    return 0;
}

void MoveObject(RigidSimulator* rsim, RigidObj* obj, int x, int y){
    obj->rect.x += x;
    obj->rect.y += y;
}



void CheckAllCollisions(RigidSimulator* rsim){

    for(int i = 0; i < rsim->countObj - 1; i++){
        for(int j = i + 1; j < rsim->countObj; j++){
            if(CheckCollision(rsim, &rsim->objList[i], &rsim->objList[j])){
                printf("collision detected %d %d\n", i, j);
            }
        }
    }
}


void CreateTriangle(
    RigidSimulator* rsim,
    Polygon** p,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3)
{
    *p = (Polygon*)malloc(sizeof(Polygon));
    (*p)->sides = 3;
    (*p)->angle = 0;
    (*p)->vertices[0].x = x1;
    (*p)->vertices[0].y = y1;
    (*p)->vertices[1].x = x2;
    (*p)->vertices[1].y = y2;
    (*p)->vertices[2].x = x3;
    (*p)->vertices[2].y = y3;
}


void DeletePolygon(RigidSimulator* rsim, Polygon** p){
    free(*p);
}


void DrawPolygon(Window* win, RigidSimulator* rsim, Polygon* p){

    SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
    for(int i = 1; i < p->sides; i++){
        SDL_RenderDrawLine(
            win->renderer, 
            p->vertices[i - 1].x,
            p->vertices[i - 1].y,
            p->vertices[i].x,
            p->vertices[i].y
        );
    }
    SDL_RenderDrawLine(
        win->renderer, 
        p->vertices[0].x,
        p->vertices[0].y,
        p->vertices[p->sides - 1].x,
        p->vertices[p->sides - 1].y
    );
}


void MovePolygon(RigidSimulator* rsim, Polygon* p, int x, int y){
    p->vertices[0].x += x;
    p->vertices[0].y += y;
    p->vertices[1].x += x;
    p->vertices[1].y += y;
    p->vertices[2].x += x;
    p->vertices[2].y += y;
}