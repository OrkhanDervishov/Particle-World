#include "blend.h"

void additive_blend(Image dest, Image src){
    if(dest.width != src.width || dest.height != src.height) return;

    for(int i = 0; i < dest.height; i++)
    for(int j = 0; j < dest.width; j++){
        IMG_GET(dest, j, i) = add_color(IMG_GET(dest, j, i), IMG_GET(src, j, i));
    }
}