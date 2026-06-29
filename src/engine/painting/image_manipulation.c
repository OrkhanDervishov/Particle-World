#include "image_manipulation.h"


PNTImage pnt_minimize_resolution(PNTImage image, int scale_x, int scale_y){
    PNTImage result;
    result.buffer = NULL;
    int result_x = image.width/scale_x;
    int result_y = image.height/scale_y;
    pnt_create_image(&result, result_x, result_y);
    int divider = scale_x*scale_y;
    
    for(int i = 0; i < result_y; i++)
    for(int j = 0; j < result_x; j++){
        uint16_t r_sum=0, g_sum=0, b_sum=0, a_sum=0;
        for(int k = i*scale_y; k < i*scale_y+scale_y; k++)
        for(int t = j*scale_x; t < j*scale_x+scale_x; t++){
            PNTColor color = PNT_IMG_GET(image, t, k);
            r_sum += color.r;
            g_sum += color.g;
            b_sum += color.b;
            a_sum += color.a;
        }
        PNT_IMG_GET(result, j, i) = (PNTColor){
            .r = r_sum/divider,
            .g = g_sum/divider,
            .b = b_sum/divider,
            .a = a_sum/divider
        };
    }
    return result;
}