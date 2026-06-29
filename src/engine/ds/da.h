#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <malloc.h>
#include <stdint.h>

// MOSTLY USED
//************************************************
 
typedef struct{
    float *items;
    size_t count;
    size_t capacity;    
} Integers;

typedef struct{
    float *items;
    size_t count;
    size_t capacity;    
} Floats;

//************************************************

typedef struct{
    size_t *items;
    size_t count;
    size_t capacity;
} Indices;

typedef struct{
    int8_t *items;
    size_t count;
    size_t capacity;
} Slots;


// DYNAMIC ARRAY
//#################################################################################

/************************************************
Example of a dynamic array struct.
items can be array of anything.

typedef struct{
    int *items;
    size_t count;
    size_t capacity;
} Numbers;
/*************************************************/

#define da_get(arr, index) (arr).items[(index)]

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

#define da_set(arr, index, x) (arr).items[(index)] = (x)

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
    if((dest).capacity <= (src).count){\
        size_t size = (src).count - (dest).capacity;\
        da_reserve(dest, size);\
    }\
    for(int i = 0; i < (src).count; i++){\
        (dest).items[i] = (src).items[i];\
    }\
    (dest).count = (src).count;\
}while(0)


// Foreach for dynamic arrays
#define da_foreach(obj_p, arr) for(int i = 0; (i < (arr).count) && (((obj_p) = &(arr).items[i]), 1); i++)

// Foreach for normal arrays
#define foreach(obj_p, arr, count) for(int i = 0; (i < (count)) && (((obj_p) = &(arr)[i]), 1); i++)

//#################################################################################


// POOL
//#################################################################################
/************************************************
Example of a pool struct.
items can be array of anything.

typedef struct{
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

typedef struct{
    Numbers elems;
    Indices free_indices;
    Slots slots;
} NumberPool;

/*************************************************/


#define pool_append(pool, x, res_index)\
do{\
    if((pool).free_indices.count > 0){\
        size_t index = da_back((pool).free_indices);\
        (pool).elems.items[index] = (x);\
        (pool).slots.items[index] = TRUE;\
        (pool).free_indices.count--;\
        (res_index) = index;\
    }\
    da_append((pool).elems, (x));\
    da_append((pool).slots, TRUE);\
    (res_index) = (pool).elems.count-1;\
}while(0)


#define pool_delete(pool, index)\
do{\
    if((index) >= (pool).elems.count || (index) < 0 || (pool).slots.items[(index)] == FALSE) break;\
    (pool).slots.items[(index)] = FALSE;\
    da_append((pool).free_indices, (index));\
}while(0)


#define pool_get(pool, index) (pool).elems.items[(index)]

#define pool_set(pool, x, index) (pool).elems.items[(index)] = (x)

#define pool_is_deleted(pool, index) (pool).slots.items[(index)]

#define pool_get_count(pool) (pool).elems.count - (pool).free_indices.count

#define pool_print(pool)\
do{\
    for(size_t i = 0; i < (pool).elems.count; i++){\
        if((pool).slots.items[i] == TRUE)\
            printf("%d, ", (pool).elems.items[i]);\
        else\
            printf("[deleted], ");\
    }\
    printf("\n");\
}while(0)

//#################################################################################



#endif //DYNAMIC_ARRAY_H_