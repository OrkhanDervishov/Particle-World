#include "particle_generation.h"


ParticleImage GenerateMagicFromImage(Image img, size_t part_size, int zeroColor){
    ParticleImage pimg;

    pimg.width = img.width/part_size;
    pimg.height = img.height/part_size;
    pimg.types = (part_type_t*)malloc(sizeof(part_type_t)*pimg.width*pimg.height);
    pimg.colors = (part_color_t*)malloc(sizeof(part_color_t)*pimg.width*pimg.height);
    
    for(int i = 0; i < pimg.height; i++)
    for(int j = 0; j < pimg.width; j++){
        part_color_t color = IMG_GET(img, j*part_size, i*part_size);
        bool add = (color.a != 0 ? TRUE : FALSE);
        pimg.types[i*pimg.width + j] = (add ? PHANTOM : EMPTY);
        if(add){
            pimg.colors[i*pimg.width + j] = color;
        }
    }

    return pimg;
}

void DestroyParticleImage(ParticleImage pimg){
    free(pimg.types);
    free(pimg.colors);
}