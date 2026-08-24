#ifndef PW_CHUNK_SYSTEM_H_
#define PW_CHUNK_SYSTEM_H_

#include "core.h"
#include "painter.h"
#include "camera.h"
#include "da.h"

typedef uint16_t pw_layer_id_t;

#define PW_LAYER_GET(Layer, index, Type) (Type)((Layer).data.items)[(index)]

typedef enum{
    PW_LAYER_GRID = 0,
    PW_LAYER_ENTITY = 1
} PWLayerType;

typedef struct{
    void* items;
    size_t count;
    size_t capacity;
} AnyData;

typedef struct{
    pw_layer_id_t id;
    PWLayerType type;

    //data
    size_t item_size;
    size_t size;
    AnyData data;
} PWLayer;

typedef struct{
    PWLayer *items;
    size_t count;
    size_t capacity;
} PWLayers;



/*********************************************/

typedef int64_t pw_chunk_coord_t;

typedef struct{
    pw_chunk_coord_t    x, y;
    /*
        I suppose (at least for now) chunk's width and height will be max value that a layer's width and height can take.
        However, this can be changed in the future, and layers' widths and heights can be bigger than chunk's. 
        So that, more complicated simulations can be created (maybe).
    */
    size_t    w, h;

    bool loaded;

    PWLayers            layers;
} PWChunk;



/*********************************************/

typedef uint16_t pw_region_id_t;

typedef struct{
    pw_region_id_t id;
    pw_chunk_coord_t    x, y;
    size_t    width, height;
    size_t    width_in_chunk, height_in_chunk;

    bool loaded;
    PWChunk *chunks;
} PWRegion;

typedef struct{
    PWRegion *items;
    size_t count;
    size_t capacity;
} PWRegions;

typedef struct{
    PWRegions elems;
    Slots slots;
    Indices free_indices;
} PWRegionPool;

/*********************************************/

typedef enum{
    PW_FIELD_LOAD_LAST = 0,
    PW_FIELD_LOAD_FAR,
    PW_FIELD_LOAD_LRU,
} PWFieldRegionLoadAlgo;

typedef struct{
    pw_chunk_coord_t    x, y;
    pw_chunk_coord_t    x_center, y_center;
   
    size_t      chunk_width,    chunk_height;
   
    size_t      region_width_in_chunks,   region_height_in_chunks;
    size_t      field_width_in_chunks,    field_height_in_chunks;

    /*
        Field first loads regions after it originizes chunks for simulation.
    */
    size_t          max_loaded_regions_count;
    Ll(PWRegion)    region_list;
    PWFieldRegionLoadAlgo loading_algo;

    PWRegionPool    regions;
    PWChunk*        chunks;
} PWField;

int pw_field_init(
    PWField *field, 
    size_t region_width_in_chunks, size_t region_height_in_chunks,
    size_t field_width_in_chunks, size_t field_height_in_chunks,
    size_t chunk_width, size_t chunk_height,
    size_t max_loaded_regions_count
);
void pw_field_destroy(PWField *field);
void pw_field_chunk_organize(PWField field);
void pw_field_update(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y);

void pw_field_region_unload_last(PWField *field);
void pw_field_region_unload_far(PWField *field);
void pw_field_region_unload_lru(PWField *field);

void pw_field_regions_load(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y);
void pw_field_region_load(PWField *field, pw_chunk_coord_t x, pw_chunk_coord_t y);
void pw_field_region_unload(PWField *field);

/*********************************************/

void pw_chunk_render(Image context, PWCamera2D camera, PWChunk* chunk, float x, float y, Color color);
void pw_region_render(Image context, PWCamera2D camera, PWRegion region, size_t chunk_width, size_t chunk_height, bool show_chunks);
void pw_field_chunks_render(Image context, PWCamera2D camera, PWField field, bool show_chunks);
void pw_field_regions_render(Image context, PWCamera2D camera, PWField field, bool show_chunks);


/*********************************************/

#endif //PW_CHUNK_SYSTEM_H_