#ifndef TIMING_H
#define TIMING_H

#include "time.h"

typedef struct{
    float delta_time;
    float last_time;
    clock_t last_time_i;

    float total_time;
    clock_t time_i;
} Time;
extern Time global_time;

#define CLOCKT_TO_FLOAT(time) ((float)(time)) / ((float)CLOCKS_PER_SEC)
#define GLOBAL_DELTA global_time.delta_time

static inline void init_global_time(){
    global_time = (Time){
        .delta_time = 0.0f,
        .last_time = 0.0f,
        .total_time = 0.0f,
        .last_time_i = 0,
        .time_i = 0
    };
}

static inline void update_global_time(){
    clock_t current_time = clock();

    if (global_time.last_time == 0) {
        global_time.last_time = current_time;
        return;
    }

    global_time.delta_time =
        (float)(current_time - global_time.last_time) / CLOCKS_PER_SEC;

    global_time.total_time += global_time.delta_time;
    global_time.last_time = current_time;
}

static inline float get_current_time(){
    clock_t current_time = clock();
    return CLOCKT_TO_FLOAT(current_time);
}

static inline float get_last_time(){
    return global_time.last_time;
}

static inline float get_global_delta(){
    return global_time.delta_time;
}

#endif