#include "dirtyrect.h"


void InitDirtyRectList(LinkedList** list){
    CreateList(list);
}

void DestroyDirtyRectList(LinkedList** list){
    ListNode* dummy = (*list)->head;
    while(dummy != NULL){
        free((Rect*)(dummy->val));
    }
    DeleteList(list);
}

void CreateRect(Rect** r, int x, int y, int w, int h){
    (*r) = (Rect*)malloc(sizeof(Rect));
    (*r)->x = x;
    (*r)->y = y;
    (*r)->w = w;
    (*r)->h = h;
}

void CorrectDirtyRect(int rows, int cols, Rect* r){
    if(r->x < 0) r->x = 0;
    if(r->x >= cols) r->x = cols - 1;

    if(r->y < 0) r->y = 0;
    if(r->y >= rows) r->y = rows - 1;

    if(r->x + r->w < 0) r->w = r->x;
    if(r->x + r->w >= cols) r->w = cols - r->x - 1;

    if(r->y + r->h < 0) r->h = r->y;
    if(r->y + r->h >= cols) r->h = cols - r->y - 1;
}

void AddDirtyRect(LinkedList* list, int rows, int cols, int px, int py, int rad){
    Rect* rect;
    CreateRect(&rect, px - rad, py - rad, 2*rad, 2*rad);
    CorrectDirtyRect(rows, cols, rect);
    TailAdd(list, rect);
}

void DeleteDirtyRect(LinkedList* list, ListNode* node){
    free((Rect*)(node->val));
    DeleteNode(list, node);
}