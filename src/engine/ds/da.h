#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAy_H_

#include <malloc.h>


// DYNAMIC ARRAY
//#################################################################################

/************************************************
Example of dynamic array struct.
items can be array of anything.

typedef struct{
    int *items;
    size_t count;
    size_t capacity;
} Numbers;
/*************************************************/

#define da_reserve(arr, size)\
do{\
    (arr).items = realloc((arr).items, ((arr).capacity + (size)) * sizeof(*(arr).items));\
    (arr).capacity += (size);\
}while(0)

#define da_free_n(arr, size)\
do{\
    if((size) <= (arr).capacity){\
        (arr).items = realloc((arr).items, ((arr).capacity - (size)) * sizeof(*(arr).items));\
        (arr).capacity -= (size);\
        (arr).count = (arr).count < (arr).capacity ? (arr).count : (arr).capacity;\
    }\
}while(0)

#define da_free(arr) da_free_n(arr, (arr).capacity)

#define da_append(arr, x) \
do{\
    if((arr).count >= (arr).capacity){\
        if((arr).capacity == 0) (arr).capacity = 64;\
        (arr).capacity *= 2;\
        (arr).items = realloc((arr).items, (arr).capacity * sizeof(*(arr).items));\
    }\
    (arr).items[arr.count++] = (x);\
}while(0)

#define da_insert(arr, x) da_append(arr, x)

#define da_push(arr, x) da_append(arr, x)

#define da_pop(arr)\
do{\
    if((arr).count != 0) (arr).count--;\
}while(0)

#define da_top(arr) (arr).items[(arr).count-1]
#define da_front(arr) (arr).items[0]
#define da_back(arr) da_top(arr)

#define da_fill(arr, x)\
do{\
    for(int i = 0; i < (arr).capacity; i++){\
        (arr).items[i] = x;\
    }\
    (arr).count = (arr).capacity;\
}while(0)

#define da_resize(arr, x, count)\
do{\
    if((arr).capacity < (count)){\
        da_reserve(arr, (count) - (arr).capacity);\
    }\
    da_fill(arr, x);\
}while(0)



#define da_copy(dest, src)\
do{\
    if((dest).capacity >= (src).count)\
    else{\
        size_t size = (src).count - (dest).capcity;\
        da_reserve(dest, size);\
    }\
    for(int i = 0; i < src.count; i++){\
        (dest).items[i] = (src).items[i];\
    }\
}while(0)


// Foreach for dynamic arrays
#define da_foreach(obj_p, arr) for(int i = 0; (i < (arr).count) && (((obj_p) = &(arr).items[i]), 1); i++)

// Foreach for normal arrays
#define foreach(obj_p, arr, count) for(int i = 0; (i < (count)) && (((obj_p) = &(arr)[i]), 1); i++)

//#################################################################################


#endif //DYNAMIC_ARRAY_H_