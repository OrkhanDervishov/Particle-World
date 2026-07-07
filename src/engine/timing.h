#ifndef TIMING_H
#define TIMING_H

#include "time.h"

typedef double pw_time_t;

typedef struct{
    pw_time_t *items;
    size_t count;
    size_t capacity;
} PWTimes;

typedef struct{
    pw_time_t delta_time;
    pw_time_t last_time;
    clock_t last_time_i;

    pw_time_t total_time;
    clock_t time_i;
} Time;

extern Time global_time;

#define CLOCKT_TO_PW_TIME_T(time) ((pw_time_t)(time)) / ((pw_time_t)CLOCKS_PER_SEC)
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

    global_time.delta_time = (pw_time_t)(current_time - global_time.last_time) / (pw_time_t)CLOCKS_PER_SEC;

    global_time.total_time += global_time.delta_time;
    global_time.last_time = current_time;
}

static inline pw_time_t get_current_time(){
    clock_t current_time = clock();
    return CLOCKT_TO_PW_TIME_T(current_time);
}

static inline pw_time_t get_last_time(){
    return global_time.last_time;
}


#define PW_DELTA_TIME get_global_delta()
static inline pw_time_t get_global_delta(){
    return global_time.delta_time;
}

#endif