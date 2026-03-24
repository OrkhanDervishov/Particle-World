#include "linked_list.h"
#define LN_SIZE sizeof(ListNode)
#define DLN_SIZE sizeof(DoublyListNode)


void llist_init(LinkedList* list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

size_t llist_size(LinkedList* list){
    return list->size;
}


void llist_add_to_head(LinkedList* list, void* value){
    ListNode* dummy = (ListNode*)malloc(LN_SIZE);
    if(dummy == NULL) return;
    dummy->val = value;

    if(list->size == 0){
        dummy->next = NULL;
        list->head = dummy;
        list->tail = dummy;
    }
    else {
        dummy->next = list->head;
        list->head = dummy;
    }

    list->size++;
}

void llist_add_to_tail(LinkedList* list, void* value){
    ListNode* dummy = (ListNode*)malloc(LN_SIZE);
    dummy->val = value;

    if(list->size == 0){
        dummy->next = NULL;
        list->head = dummy;
        list->tail = dummy;
    }
    else {
        dummy->next = NULL;
        list->tail->next = dummy;
        list->tail = dummy;
    }

    list->size++;
}

void llist_delete_node(LinkedList* list, ListNode* node){
    // if(list->size == 0) return;
    
    // ListNode* dummy = node;
    // if(dummy == list->head){
    //     dummy->next
    // }
}

void llist_delete_node_by_index(LinkedList* list, int index){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return;
    if(index > list->size - 1) return;

    ListNode* pre = NULL;
    ListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            if(pre == NULL){
                free(dummy);
                break;
            }
            pre->next = dummy->next;
            free(dummy);
            break;
        }
        pre = dummy;
        dummy = dummy->next;
        index--;
    }
    list->size--;
}


void* llist_get_value(LinkedList* list, int index){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return NULL;
    if(index > list->size - 1) return NULL;

    ListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            return dummy->val;
        }
        dummy = dummy->next;
        index--;
    }
}


void llist_print(LinkedList* list, void (*print)(void* value)){
    if(list->head == NULL) return;

    ListNode* dummy = list->head;
    while(dummy != NULL){
        print(dummy->val);
        dummy = dummy->next;
    }
}


void llist_change_node_value(LinkedList* list, int index, void* value){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return;
    if(index > list->size - 1) return;

    ListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            dummy->val = value;
        }
        dummy = dummy->next;
        index--;
    }
}


void llist_delete(LinkedList* list){
    if(list->head == NULL) return;
    ListNode* dummy = list->head;
    
    while(list->head != list->tail){
        list->head = list->head->next;
        free(dummy);
        dummy = list->head;
    }
    free(list->head);
}








// Include doubly linked list library implementations 

void dllist_init(DoublyLinkedList* list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

size_t dllist_size(DoublyLinkedList* list){
    return list->size;
}

void dllist_add_to_head(DoublyLinkedList* list, void* value){
    DoublyListNode* dummy = (DoublyListNode*)malloc(DLN_SIZE);
    if(dummy == NULL) return;
    dummy->val = value;

    if(list->size == 0){
        dummy->next = NULL;
        dummy->prev = NULL;
        list->head = dummy;
        list->tail = dummy;
    }
    else {
        list->head->prev = dummy;
        dummy->next = list->head;
        dummy->prev = NULL;
        list->head = dummy;
    }

    list->size++;
}

void dllist_add_to_tail(DoublyLinkedList* list, void* value){
    DoublyListNode* dummy = (DoublyListNode*)malloc(LN_SIZE);
    dummy->val = value;

    if(list->size == 0){
        dummy->next = NULL;
        dummy->prev = NULL;
        list->head = dummy;
        list->tail = dummy;
    }
    else {
        list->tail->next = dummy;
        dummy->next = NULL;
        dummy->prev = list->tail;
        list->tail = dummy;
    }

    list->size++;
}

void dllist_delete_node(DoublyLinkedList* list, int index){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return;
    if(index > list->size - 1) {
        printf("Invalid index %d: dllist_delete_node.\n", index);
        return;
    }

    DoublyListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            if(dummy == list->head && dummy == list->tail){
                printf("works\n");
                list->head = NULL;
                list->tail = NULL;
                free(dummy);
                break;
            }
            if(dummy == list->head){
                list->head = dummy->next;
                list->head->prev = NULL;
                free(dummy);
                break;
            }
            if(dummy == list->tail){
                list->tail = dummy->prev;
                list->tail->next = NULL;
                free(dummy);
                break;
            }
            dummy->prev->next = dummy->next;
            dummy->next->prev = dummy->prev;
            free(dummy);
            break;
        }
        dummy = dummy->next;
        index--;
    }
    list->size--;
}

void* dllist_get_value(DoublyLinkedList* list, int index){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return NULL;
    if(index > list->size - 1) return NULL;

    DoublyListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            return dummy->val;
        }
        dummy = dummy->next;
        index--;
    }
}

void dllist_print(DoublyLinkedList* list, void (*print)(void* value)){
    if(list->head == NULL) return;

    DoublyListNode* dummy = list->head;
    while(dummy != NULL){
        print(dummy->val);
        dummy = dummy->next;
    }
}

void dllist_change_node_value(DoublyLinkedList* list, int index, void* value){
    if(index < 0) index = list->size + index - 1;
    if(list->size == 0) return;
    if(index > list->size - 1) return;

    DoublyListNode* dummy = list->head;
    while(index >= 0){
        if(index == 0){
            dummy->val = value;
        }
        dummy = dummy->next;
        index--;
    }
}

void dllist_delete(DoublyLinkedList* list){
    if(list->head == NULL) return;
    DoublyListNode* dummy = list->head;
    
    while(list->head != list->tail){
        list->head = list->head->next;
        free(dummy);
        dummy = list->head;
    }
    free(list->head);
}
