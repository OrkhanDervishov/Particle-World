#include "asset_loader.h"

/************************************************************/
// Storing entities as objects

asset_id_t asset_add(AssetPool* pool, Asset item){
    if(pool->free_indices.count > 0){
        asset_id_t index = da_back(pool->free_indices);
        pool->items.items[index] = item;
        pool->slots.items[index] = TRUE;
        pool->free_indices.count--;
        return index;
    }
    da_append(pool->items, item);
    da_append(pool->slots, TRUE);
    return pool->items.count-1;
}

void asset_delete(AssetPool* pool, asset_id_t index){
    if(index >= pool->items.count || pool->slots.items[index] == FALSE) return;
    pool->slots.items[index] = FALSE;
    da_append(pool->free_indices, index);
}

void asset_pool_print_stats(AssetPool* pool){
    printf("Asset count: %zu\n", pool->items.count - pool->free_indices.count);
}   

/************************************************************/
