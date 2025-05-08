#include "common.h"


void** Malloc2D(int rows, int cols, int elemSize){
    void** arr = malloc(rows * sizeof(void*));
    for(int i = 0; i < rows; i++){
        arr[i] = malloc(cols * elemSize);
    }
    return arr;
}


clock_t FindDelta(clock_t old){
    return 0;
}