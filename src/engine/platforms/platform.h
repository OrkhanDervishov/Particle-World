#ifndef PLATFORM_H
#define PLATFORM_H

typedef enum{
    SDL2 = 0
} PlatformType;

typedef struct{
    PlatformType type;
} Platform;

#endif