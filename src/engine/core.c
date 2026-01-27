#include "core.h"

void* Malloc2D(int rows, int cols, int elemSize){

    void** arr = malloc(rows * sizeof(void*));
    for(int i = 0; i < rows; i++){
        arr[i] = malloc(cols * elemSize);
    }
    return arr;
}

void Free2D(void* arr, int rows){
    void** mat = arr;
    for(int i = 0; i < rows; i++){
        free(mat[i]);
    }
    free(mat);
}

long GetTimeNano(){
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return res.tv_sec * 1000000000L + res.tv_nsec;
}