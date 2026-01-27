#ifndef DIRTYRECT_H
#define DIRTYRECT_H

#include "../engine/engine_api.h"

void InitDirtyRectList(LinkedList** list);
void DestroyDirtyRectList(LinkedList** list);

void CreateRect(Rect** r, int x, int y, int w, int h);
void AddDirtyRect(LinkedList* list, int rows, int cols, int px, int py, int rad);
void DeleteDirtyRect(LinkedList* list, ListNode* node);
void CorrectDirtyRect(int rows, int cols, Rect* r);

#endif