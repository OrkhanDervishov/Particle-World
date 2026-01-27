#ifndef RIGID_H
#define RIGID_H

#include "../core.h"

#define QUIT_BUTTON (event.key.keysym.sym == SDLK_ESCAPE)

#define MAXGON 12

typedef struct{
    int angle;
    int sides;
    Vec2d vertices[MAXGON];
} Polygon;


typedef struct{
    int id;
    SDL_Rect rect;
} RigidObj;


typedef struct
{
    int last_id;
    int h, w;
    int countObj;
    RigidObj* objList;
} RigidSimulator;

extern int R_Sim_Height;
extern int R_Sim_Width;


int InitRigidSimulator(RigidSimulator** rsim, int h, int w);
void DestroyRigidSimulator(RigidSimulator** rsim);

void CreateRect(RigidSimulator* rsim, int x, int y, int w, int h);
void DrawObjects(Window* win, RigidSimulator* rsim);

void ProcessRigidInput(SDL_Event event, Window* win, RigidSimulator* rsim);

// By Id
RigidObj* GetObjectById(RigidSimulator* rsim, int id);
int CheckCollisionById(RigidSimulator* rsim, int id1, int id2);
void MoveObjectById(RigidSimulator* rsim, int id, int x, int y);

// By Object
int CheckCollision(RigidSimulator* rsim, RigidObj* obj1, RigidObj* obj2);
void MoveObject(RigidSimulator* rsim, RigidObj* obj, int x, int y);
int CheckBorderCollision(RigidSimulator* rsim, RigidObj* obj);

// Global Collision
void CheckAllCollisions(RigidSimulator* rsim);

// Polygons
void CreateTriangle(
RigidSimulator* rsim,
Polygon** p,
int x1, int y1,
int x2, int y2,
int x3, int y3);

void DeletePolygon(RigidSimulator* rsim, Polygon** p);
void DrawPolygon(Window* win, RigidSimulator* rsim, Polygon* p);
void MovePolygon(RigidSimulator* rsim, Polygon* p, int x, int y);

#endif