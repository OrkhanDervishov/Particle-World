#ifndef QUADTREE_H
#define QUADTREE_H
#include <stdio.h>
#include <malloc.h>
#include <math.h>


// #define QuadNode struct QuadNode
typedef struct QuadNode QuadNode;
#define PART_SIDE 2

typedef struct
{
    int x, y;
} Pos;


typedef struct{
    Pos p;
} Particle;

struct QuadNode{
    Pos tl;
    Pos br;
    Particle* val;
    QuadNode* p;
    QuadNode* lu;
    QuadNode* ld;
    QuadNode* ru;
    QuadNode* rd;
};

#define CREATE_QUAD (QuadNode*)malloc(sizeof(QuadNode))
#define INIT_QUAD(node) (node)->val = NULL;(node)->p = NULL;(node)->lu = NULL;(node)->ld = NULL;(node)->ru = NULL;(node)->rd = NULL;

void CreateQuadTree(QuadNode** root, int w, int h);
void InsertQuadNode(QuadNode* root, Particle* part);



void CreateQuadTree(QuadNode** root, int w, int h){
    *root = CREATE_QUAD;
    (*root)->tl.x = 0;
    (*root)->tl.y = 0;
    (*root)->br.x = w;
    (*root)->br.y = h;
    (*root)->val = NULL;
    (*root)->p = NULL;
    (*root)->lu = NULL;
    (*root)->ld = NULL;
    (*root)->ru = NULL;
    (*root)->rd = NULL;
}

void InsertQuadNode(QuadNode* tree, Particle* part){
    if(tree == NULL) return;
    if(part == NULL) return;
    // printf("worked2\n");
    int x = part->p.x;
    int y = part->p.y;
    int w = abs(tree->tl.x - tree->br.x);
    int h = abs(tree->tl.y - tree->br.y);
    // printf("worked3\n");
    // Quad was found
    if(w == PART_SIDE && h == PART_SIDE)
    {
        tree->val = part;
        return;
    }
    // printf("worked4\n");

    int midx = tree->br.x/2;
    int midy = tree->br.y/2;

    // 0 - up, 1 - down
    int ud = (y >= h/2) ? 1 : 0;
    // 0 - left, 2 - right
    int lr = (x >= w/2) ? 2 : 0;
    // Create a new child quad
    printf("%d\n", lr + ud);
    
    // lr + ud = {0, 1, 2, 3}
    switch (lr + ud)
    {
    // up left
    case 0:
        tree->lu = CREATE_QUAD;
        tree->lu->p = tree;
        tree->lu->tl.x = tree->tl.x;
        tree->lu->tl.y = tree->tl.y;
        tree->lu->br.x = midx;
        tree->lu->br.y = midy;
        INIT_QUAD(tree->lu)
        InsertQuadNode(tree->lu, part);
        break;
    // down left
    case 1:
        tree->ld = CREATE_QUAD;
        tree->ld->p = tree;
        tree->ld->tl.x = tree->tl.x;
        tree->ld->tl.y = midy;
        tree->ld->br.x = midx;
        tree->ld->br.y = tree->br.y;
        INIT_QUAD(tree->ld)
        InsertQuadNode(tree->ld, part);
        break;
    // up right
    case 2:
        tree->ru = CREATE_QUAD;
        tree->ru->p = tree;
        tree->ru->tl.x = midx;
        tree->ru->tl.y = tree->tl.y;
        tree->ru->br.x = tree->br.x;
        tree->ru->br.y = midy;
        INIT_QUAD(tree->ru)
        InsertQuadNode(tree->ru, part);
        break;
    // down right
    case 3:
        tree->rd = CREATE_QUAD;
        tree->rd->p = tree;
        tree->rd->tl.x = midx;
        tree->rd->tl.y = midy;
        tree->rd->br.x = tree->br.x;
        tree->rd->br.y = tree->br.y;
        INIT_QUAD(tree->rd)
        InsertQuadNode(tree->rd, part);
        break;
    }

}



#endif // QUADTREE_H