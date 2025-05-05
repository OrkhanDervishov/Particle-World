#include <stdio.h>
#include "quadtree.h"


int main(){
    QuadNode* root;
    CreateQuadTree(&root, 8, 8);
    Particle part;
    part.p.x = 2;
    part.p.y = 2;
    printf("worked1\n");
    InsertQuadNode(root, &part);
    printf("worked5\n");
    // if(root->lu == NULL) printf("worked\n");
    printf("x = %d, ", root->lu->rd->val->p.x);
    
    printf("y = %d\n", root->lu->rd->val->p.y);
}