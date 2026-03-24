// C linked list library
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define INVALID_LL_VAL 0
#define LL_VAL_TYPE int


#include <stdio.h>
#include <malloc.h>

typedef struct ListNode{
    void* val;
    struct ListNode* next;
} ListNode;

typedef struct DoublyListNode{
    void* val;
    struct DoublyListNode* prev;
    struct DoublyListNode* next;
} DoublyListNode;

typedef struct 
{
    size_t size;
    ListNode* head;
    ListNode* tail;
} LinkedList;

typedef struct 
{
    size_t size;
    DoublyListNode* head;
    DoublyListNode* tail;
} DoublyLinkedList;

// Singly linked list functions
void llist_init(LinkedList* list);
size_t llist_size(LinkedList* list);
void llist_add_to_head(LinkedList* list, void* value);
void llist_add_to_tail(LinkedList* list, void* value);
void llist_delete_node_by_index(LinkedList* list, int index);
void llist_delete_node(LinkedList* list, ListNode* node);
void* llist_get_value(LinkedList* list, int index);
void llist_print(LinkedList* list, void (*print)(void* value));
void llist_change_node_value(LinkedList* list, int index, void* value);
void llist_delete(LinkedList* list);

// Doubly linked list functions
void dllist_init(DoublyLinkedList* list);
size_t dllist_size(DoublyLinkedList* list);
void dllist_add_to_head(DoublyLinkedList* list, void* value);
void dllist_add_to_tail(DoublyLinkedList* list, void* value);
void dllist_delete_node(DoublyLinkedList* list, int index);
void* dllist_get_value(DoublyLinkedList* list, int index);
void dllist_print(DoublyLinkedList* list, void (*print)(void* value));
void dllist_change_node_value(DoublyLinkedList* list, int index, void* value);
void dllist_delete(DoublyLinkedList* list);

#endif //LINKED_LIST_H