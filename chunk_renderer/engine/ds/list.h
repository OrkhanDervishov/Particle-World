// C linked list library
#ifndef CWO_LINKED_LIST_H
#define CWO_LINKED_LIST_H

#include <stdio.h>
#include <malloc.h>

typedef struct ListNode ListNode;

struct ListNode{
    void* val;
    ListNode* pre;
    ListNode* next;
};

typedef struct 
{
    size_t size;
    ListNode* head;
    ListNode* tail;
} LinkedList;


void CreateList(LinkedList** list);
void DeleteList(LinkedList** list);
void ClearList(LinkedList* list);

void TailAdd(LinkedList* list, void* val);
void HeadAdd(LinkedList* list, void* val);
void DeleteNode(LinkedList* list, ListNode* node);

void PrintList(LinkedList* list, void (*printer)(void* val));

// void ListTests();

#endif //CWO_LINKED_LIST_H