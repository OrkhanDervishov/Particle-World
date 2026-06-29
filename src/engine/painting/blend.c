#include "blend.h"

void pnt_additive_blend(PNTImage dest, PNTImage src){
    if(dest.buffer == NULL || src.buffer == NULL) return;
    if(dest.width != src.width || dest.height != src.height) return;

    for(int i = 0; i < dest.height; i++)
    for(int j = 0; j < dest.width; j++){
        PNT_IMG_GET(dest, j, i) = pnt_add_color(PNT_IMG_GET(dest, j, i), PNT_IMG_GET(src, j, i));
    }
}

void pnt_alpha_blend(PNTImage dest, PNTImage src){
    if(dest.buffer == NULL || src.buffer == NULL) return;
    if(dest.width != src.width || dest.height != src.height) return;
    
    for(int i = 0; i < dest.height; i++)
    for(int j = 0; j < dest.width; j++){
        PNT_IMG_GET(dest, j, i) = pnt_colors_alpha_blend(PNT_IMG_GET(dest, j, i), PNT_IMG_GET(src, j, i));
    }
}