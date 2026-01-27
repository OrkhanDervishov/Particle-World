#include "list.h"


#define CREATE_NODE(node, v, p, n) (node) = (ListNode*)malloc(sizeof(ListNode)); (node)->val = (v), (node)->pre = (p), (node)->next = (n)


void CreateList(LinkedList** list){
    (*list) = (LinkedList*)malloc(sizeof(LinkedList));
    (*list)->size = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;
}

void DeleteList(LinkedList** list){
    ClearList(*list);
    free(*list);
}

void ClearList(LinkedList* list){

    ListNode* dummy = list->head;

    while(dummy != NULL){
        ListNode* nextNode = dummy->next;
        free(dummy);
        dummy = nextNode;
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}



void TailAdd(LinkedList* list, void* val){
    list->size++;
    if(list->head == NULL && list->tail == NULL){
        ListNode* node;
        CREATE_NODE(node, val, NULL, NULL);
        list->head = node, list->tail = node;
        return;
    }
    ListNode* node;
    CREATE_NODE(node, val, list->tail, NULL);
    list->tail->next = node;
    list->tail = node;
}

void HeadAdd(LinkedList* list, void* val){
    list->size++;
    if(list->head == NULL && list->tail == NULL){
        ListNode* node;
        CREATE_NODE(node, val, NULL, NULL);
        list->head = node, list->tail = node;
        return;
    }
    ListNode* node;
    CREATE_NODE(node, val, NULL, list->head);
    list->head->pre = node;
    list->head = node;
}

void DeleteNode(LinkedList* list, ListNode* node){
    if(node == NULL) return;

    list->size--;

    if(node == list->head && node == list->tail){
        free(node);
        list->head = NULL;
        list->tail = NULL;
        return;
    }
    if(node == list->head){
        if(node->next == NULL){
            free(node);
            list->head = NULL;
            list->tail = NULL;
        } else {
            node->next->pre = NULL;
            list->head = node->next;
            free(node);
        }
        return;
    }
    if(node == list->tail){
        if(node->pre == NULL){
            free(node);
            list->tail = NULL;
            list->head = NULL;
        } else {
            node->pre->next = NULL;
            list->tail = node->pre;
            free(node);
        }
        return;
    }
    node->pre->next = node->next;
    node->next->pre = node->pre;
    free(node);
}


void PrintList(LinkedList* list, void (*printer)(void* val)){
    ListNode* dummy = list->head;

    while(dummy != NULL){
        printer(dummy->val);
        dummy = dummy->next;
    }
}

// void RectPrinter(void* val){
//     Rect* r = (Rect*)val;
//     printf("x:%d y:%d w:%d h:%d\n", r->x, r->y, r->w, r->h);
// }


// void ListTests(){
//     LinkedList* list;
//     CreateList(&list);

//     printf("works1\n");
//     Rect r1 = {1, 1, 3, 3};
//     Rect r2 = {4, 1, 7, 9};
//     Rect r3 = {12, 34, 23, 12};
//     Rect r4 = {24, 23, 312, 14};
//     Rect r5 = {87, 65, 54, 321};

//     AddNodeToTail(list, &r1);
//     AddNodeToTail(list, &r5);

//     printf("works2\n");
//     PrintList(list, RectPrinter);

//     DeleteNode(list, list->head);

//     printf("\n\n\n");
//     PrintList(list, RectPrinter);

//     DeleteNode(list, list->tail);

//     printf("\n\n\n");
//     PrintList(list, RectPrinter);

//     ClearList(list);

//     printf("\n\n\n");
//     PrintList(list, RectPrinter);

//     DeleteList(&list);
// }
