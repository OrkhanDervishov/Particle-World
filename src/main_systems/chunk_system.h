#ifndef CHUNK_SYSTEM_H_
#define CHUNK_SYSTEM_H_

#include "core.h"

typedef uint16_t pw_layer_id_t;

typedef struct{
    pw_layer_id_t id;

    //data
} Layer;


/*********************************************/

typedef uint16_t pw_chunk_id_t;
typedef int32_t pw_chunk_coord_t;

typedef struct{
    pw_chunk_id_t id;
    pw_chunk_coord_t x, y;
    Layer* layers;
} Chunk;



/*********************************************/

typedef uint16_t pw_region_id_t;

typedef struct{
    pw_region_id_t id;

} Region;




/*********************************************/


typedef struct{
    Chunk* chunks
} ChunkSpace;



/*********************************************/

#endif //CHUNK_SYSTEM_H_