#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <malloc.h>
#include <stdint.h>

// MOSTLY USED
//************************************************
 
typedef struct{
    int *items;
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

#define Da(Type)\
struct{\
    Type *items;\
    size_t count;\
    size_t capacity;\
    size_t item_size;\
}

#define da_typeof(arr) __typeof__((arr))

#define da_typeof_value(arr) __typeof__(*(arr).items)

#define da_zero(arr) (da_typeof((arr))){0}

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



//-------------------------------------
/*
    DaAnyData is a special dynamic array that can hold any type of value.
    It has additional item_size member.
*/
typedef struct{
    void* items;
    size_t count;
    size_t capacity;
    size_t item_size;
} DaAnyData;



//-------------------------------------

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

#define Pool(Type)\
struct{\
    struct{\
        Type *items;\
        size_t count;\
        size_t capacity;\
    } elems;\
    Indices free_indices;\
    Slots slots;\
}

#define pool_typeof(pool) __typeof__((pool))

#define pool_typeof_value(pool) __typeof__(*(pool).elems.items)

#define pool_zero(pool) (pool_typeof((pool))){0}

#define pool_free(pool)\
do{\
    free((pool).elems.items);\
    free((pool).slots.items);\
    free((pool).free_indices.items);\
}while(0)

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

// LINKED LIST
//#################################################################################

#define LL_ALLOCATOR(size) malloc((size))
#define LL_FREE(mem) free((mem))
#define LL_TYPEOF(ll) __typeof__((ll).head)
#define LL_TYPEOF_VALUE(ll) __typeof__((ll).head->value)

#define Ll(Type)                \
    struct{                     \
        struct LLNode{          \
            Type value;         \
            struct LLNode *next;\
            struct LLNode *prev;\
        } *head, *tail;         \
                                \
        size_t count;           \
    }

#define LL_INITIALIZATION {\
    .head = NULL,\
    .tail = NULL,\
    .count = 0\
}

#define ll_init(ll)\
do{\
    (ll).head = NULL;\
    (ll).tail = NULL;\
    (ll).count = 0;\
}while(0)

#define ll_create_node(ll, node_return, next_node, prev_node, new_value)\
do{\
    (node_return) = LL_ALLOCATOR(sizeof(*node));\
    (node_return)->prev = (prev_node);\
    (node_return)->next = (next_node);\
    (node_return)->value = (new_value);\
}while(0)

// for(size_t i = 0; i < index && curr; i++){
#define ll_get_node_by_index(ll, node_return, index)\
do{\
    LL_TYPEOF(ll) ll_curr = (ll).head;\
    (node_return) = NULL;\
    for(size_t ll_i = 0; ll_i != (index) && ll_curr; ll_i++)\
        ll_curr = ll_curr->next;\
\
    if(ll_curr)\
        (node_return) = ll_curr;\
}while(0)


#define ll_push_head(ll, value)\
do{\
    LL_TYPEOF(ll) node;\
    ll_create_node(ll, node, (ll).head, NULL, value);\
\
    if((ll).head){\
        (ll).head->prev = node;\
    }\
    else{\
        (ll).tail = node;\
    }\
\
    (ll).head = node;\
    (ll).count++;\
} while(0)

#define ll_push_tail(ll, value)\
do{\
    LL_TYPEOF(ll) node;\
    ll_create_node(ll, node, NULL, (ll).tail, value);\
\
    if((ll).tail){\
        (ll).tail->next = node;\
    }\
    else{\
        (ll).head = node;\
    }\
\
    (ll).tail = node;\
    (ll).count++;\
} while(0)

#define ll_pop_head(ll)\
do{\
    LL_TYPEOF(ll) node = (ll).head;\
    \
    if(node == NULL) break;\
    (ll).head = node->next;\
\
    if(node->next){\
        (ll).head->prev = NULL;\
    }\
    else{\
        (ll).tail = NULL;\
    }\
\
    LL_FREE(node);\
    (ll).count--;\
} while(0)

#define ll_pop_tail(ll)\
do{\
    LL_TYPEOF(ll) node = (ll).tail;\
    \
    if(node == NULL) break;\
    (ll).tail = node->prev;\
\
    if(node->prev){\
        (ll).tail->next = NULL;\
    }\
    else{\
        (ll).head = NULL;\
    }\
\
    LL_FREE(node);\
    (ll).count--;\
} while(0)

#define ll_push_index(ll, index, value)\
do{\
    LL_TYPEOF(ll) node;\
    LL_TYPEOF(ll) new_node;\
    ll_get_node_by_index(ll, node, index);\
    if(node == NULL){\
        ll_create_node(ll, new_node, NULL, NULL, value);\
        (ll).head = (ll).tail = new_node;\
    }\
\
    LL_TYPEOF(ll) prev = node->prev;\
\
    ll_create_node(ll, new_node, node, prev, value);\
    node->prev = new_node;\
    if(prev) prev->next = new_node;\
    (ll).count++;\
} while(0)

#define ll_pop_index(ll, index)\
do{\
    LL_TYPEOF(ll) node;\
    ll_get_node_by_index(ll, node, index);\
    if(node == NULL){\
        break;\
    }\
    LL_TYPEOF(ll) next = node->next, prev = node->prev;\
    if(prev) prev->next = next;\
    if(next) next->prev = prev;\
    LL_FREE(node);\
    (ll).count--;\
} while(0)

#define ll_pop_node(ll, node)\
do{\
    if((node) == NULL){\
        break;\
    }\
    LL_TYPEOF(ll) next = (node)->next, prev = (node)->prev;\
    if((node) == (ll).head) (ll).head = next;\
    if((node) == (ll).tail) (ll).tail = prev;\
    if(prev) prev->next = next;\
    if(next) next->prev = prev;\
    LL_FREE((node));\
    (ll).count--;\
} while(0)

#define ll_get_head_val_direct(ll) (ll).head->value

#define ll_get_tail_val_direct(ll) (ll).tail->value

#define ll_get_head_val(ll, var)\
do{\
    var = (ll).head->value;\
} while(0)

#define ll_get_tail_val(ll, var)\
do{\
    var = (ll).tail->value;\
} while(0)

#define ll_get_val(ll, var, index)\
do{\
    LL_TYPEOF(ll) node = NULL;\
    ll_get_node_by_index(ll, node, index);\
    if(node) var = node->value;\
} while(0)

#define ll_free(ll)\
do{\
    LL_TYPEOF(ll) curr = (ll).head;\
    while(curr){\
        LL_FREE(curr);\
        curr = curr->next;\
    }\
    (ll).count = 0;\
} while(0)

#define ll_cycle(ll)\
do{\
    (ll).tail->next = (ll).head;\
    (ll).head->prev = (ll).tail;\
} while(0)

#define ll_get_count(ll) (ll).count

//#################################################################################

#endif //DYNAMIC_ARRAY_H_