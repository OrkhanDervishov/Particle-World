#ifndef PW_CHUNK_SYSTEM_H_
#define PW_CHUNK_SYSTEM_H_

#include "core.h"
#include "painter.h"
#include "camera.h"
#include "da.h"

/*
    Layer is a structure where every simulation's data will be stored.
    Every chunk will store its own layers' data.
    Layers allow new simulation types to be added easily.
    Layers also can interract with each other by getting and setting other layers' data.

    I decided to make like this beacause as every chunk is simulated one-by-one,
    it is comfortable to simulate all chunk's layers and finish simulation for this chunk's
    simulation completely.
    It is highly possible in future to this system change a lot.

    Each layer type will have its own simulation and rendering functions.
    Examples.
    Simulation: void simulate_layer(Da(PWlayer) layers);
    Rendering:  void render_layer(Da(PWlayer) layers);
*/

typedef int64_t pw_chunk_coord_t;

typedef uint16_t pw_layer_id_t;

#define PW_LAYER_GET(Layer, index, Type) (Type)((Layer).data.items)[(index)]

typedef enum{
    PW_LAYER_GRID = 0,
    PW_LAYER_ENTITY
} PWLayerType;


typedef struct{
    size_t item_size;

    DaAnyData data;
} PWSubLayer;
typedef Da(PWSubLayer) PWSubLayers;

typedef struct{
    pw_layer_id_t id;
    PWLayerType type;

    //Data
    // Size in bytes of type of items that this layer will store
    size_t item_size;
    // For both grid and entity data
    size_t size;
    // Only for grid data
    size_t width;
    size_t height;
    // Data itself
    // In the future data will be divided into sublayers for storing item's data with data oriented design
    PWSubLayers sublayers;
} PWLayer;
typedef Da(PWLayer) PWLayers;





typedef void (*PWLayerRenderer)(
    Image context, PWCamera2D camera, PWLayer layer, 
    pw_chunk_coord_t x, pw_chunk_coord_t y,
    size_t chunk_width, size_t chunk_height
);
typedef void (*PWLayerGenerator)(
    PWLayer *layer, pw_chunk_coord_t x, pw_chunk_coord_t y
);

typedef struct{
    size_t      registered_layer_count;
    PWLayers    layers;
    Da(PWLayerRenderer) layer_renderers;
    Da(PWLayerGenerator) layer_generators;
} PWLayerSystem;



PWLayer pw_layer_create(PWLayerType type, size_t item_size, size_t size, size_t width, size_t height);
void pw_layer_add_sublayer(PWLayer *layer, size_t item_size);
int pw_layer_sys_layer_add(PWLayerSystem *ls, PWLayer layer, PWLayerRenderer layer_renderer, PWLayerGenerator layer_generator);
void pw_layer_sys_info(PWLayerSystem *ls);

/*********************************************/

typedef struct{
    pw_chunk_coord_t    x, y;
    /*
        I suppose (at least for now) chunk's width and height will be max value that a layer's width and height can take.
        However, this can be changed in the future, and layers' widths and heights can be bigger than chunk's. 
        So that, more complicated simulations can be created (maybe).
    */
    size_t    w, h;

    bool loaded;

    PWLayers layers;
    // PWLayers layers
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
    PWFieldRegionLoadAlgo loading_algo;
    size_t          max_loaded_regions_count;
    Ll(PWRegion)    region_list;

    PWLayerSystem   ls;

    PWChunk**       chunks;
} PWField;

int pw_field_init(
    PWField *field, 
    size_t region_width_in_chunks, size_t region_height_in_chunks,
    size_t field_width_in_chunks, size_t field_height_in_chunks,
    size_t chunk_width, size_t chunk_height,
    size_t max_loaded_regions_count,
    PWLayerSystem ls
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

void pw_chunk_render(Image context, PWCamera2D camera, PWField field, PWChunk* chunk, float x, float y, Color color);
void pw_region_render(Image context, PWCamera2D camera, PWField field, PWRegion region, size_t chunk_width, size_t chunk_height, bool show_chunks);
void pw_field_chunks_render(Image context, PWCamera2D camera, PWField field, bool show_chunks);
void pw_field_regions_render(Image context, PWCamera2D camera, PWField field, bool show_chunks);


void pw_layer1_render(
    Image context, PWCamera2D camera, PWLayer layer, 
    pw_chunk_coord_t x, pw_chunk_coord_t y, 
    size_t chunk_width, size_t chunk_height
);
void pw_layer1_generate(PWLayer *layer, pw_chunk_coord_t x, pw_chunk_coord_t y);

/*********************************************/

#endif //PW_CHUNK_SYSTEM_H_