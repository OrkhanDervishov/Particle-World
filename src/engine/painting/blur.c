#include "blur.h"


void gaussian_blur_3x3(Image dest, Image src){

    /*   ___________
        |___|___|___|
        |___|_*_|___|
        |___|___|___|
    */
    int divisor = 16.0f;
    int mult = 1.0f;
    float kernel[3][3] = {
        {mult*1.0f/divisor, mult*2.0f/divisor, mult*1.0f/divisor},
        {mult*2.0f/divisor, 4.0f/divisor, mult*2.0f/divisor},
        {mult*1.0f/divisor, mult*2.0f/divisor, mult*1.0f/divisor}
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 3; k++)
        for(int t = 0; t < 3; t++){
            Color c = get_color(src, j+t-1, i+k-1);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k][t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_5x5(Image dest, Image src){

    /*   ___________
        |___|___|___|
        |___|_*_|___|
        |___|___|___|
    */
    float kernel[5][5] = {
        {1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256},
        {4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256},
        {6.0f/256, 24.0f/256, 36.0f/256, 24.0f/256, 6.0f/256},
        {4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256},
        {1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256}
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 5; k++)
        for(int t = 0; t < 5; t++){
            Color c = get_color(src, j+t-1, i+k-1);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k][t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_7x7(Image dest, Image src){

    /*   ___________
        |___|___|___|
        |___|_*_|___|
        |___|___|___|
    */
    float kernel[7][7] = {
        {1.0f/4096,  6.0f/4096, 15.0f/4096, 20.0f/4096, 15.0f/4096,  6.0f/4096, 1.0f/4096},
        {6.0f/4096, 36.0f/4096, 90.0f/4096,120.0f/4096, 90.0f/4096, 36.0f/4096, 6.0f/4096},
        {15.0f/4096,90.0f/4096,225.0f/4096,300.0f/4096,225.0f/4096, 90.0f/4096,15.0f/4096},
        {20.0f/4096,120.0f/4096,300.0f/4096,400.0f/4096,300.0f/4096,120.0f/4096,20.0f/4096},
        {15.0f/4096,90.0f/4096,225.0f/4096,300.0f/4096,225.0f/4096, 90.0f/4096,15.0f/4096},
        {6.0f/4096,36.0f/4096, 90.0f/4096,120.0f/4096, 90.0f/4096, 36.0f/4096, 6.0f/4096},
        {1.0f/4096,  6.0f/4096, 15.0f/4096, 20.0f/4096, 15.0f/4096,  6.0f/4096, 1.0f/4096}
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 7; k++)
        for(int t = 0; t < 7; t++){
            Color c = get_color(src, j+t-1, i+k-1);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k][t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_9x9(Image dest, Image src){

    /*   ___________
        |___|___|___|
        |___|_*_|___|
        |___|___|___|
    */
    float kernel[9][9] = {
        {1/65536.0f,  8/65536.0f,  28/65536.0f,  56/65536.0f,  70/65536.0f,  56/65536.0f,  28/65536.0f,  8/65536.0f, 1/65536.0f},
        {8/65536.0f, 64/65536.0f, 224/65536.0f, 448/65536.0f, 560/65536.0f, 448/65536.0f, 224/65536.0f, 64/65536.0f, 8/65536.0f},
        {28/65536.0f,224/65536.0f,784/65536.0f,1568/65536.0f,1960/65536.0f,1568/65536.0f,784/65536.0f,224/65536.0f,28/65536.0f},
        {56/65536.0f,448/65536.0f,1568/65536.0f,3136/65536.0f,3920/65536.0f,3136/65536.0f,1568/65536.0f,448/65536.0f,56/65536.0f},
        {70/65536.0f,560/65536.0f,1960/65536.0f,3920/65536.0f,4900/65536.0f,3920/65536.0f,1960/65536.0f,560/65536.0f,70/65536.0f},
        {56/65536.0f,448/65536.0f,1568/65536.0f,3136/65536.0f,3920/65536.0f,3136/65536.0f,1568/65536.0f,448/65536.0f,56/65536.0f},
        {28/65536.0f,224/65536.0f,784/65536.0f,1568/65536.0f,1960/65536.0f,1568/65536.0f,784/65536.0f,224/65536.0f,28/65536.0f},
        {1/65536.0f,  8/65536.0f,  28/65536.0f,  56/65536.0f,  70/65536.0f,  56/65536.0f,  28/65536.0f,  8/65536.0f, 1/65536.0f},
        {8/65536.0f, 64/65536.0f, 224/65536.0f, 448/65536.0f, 560/65536.0f, 448/65536.0f, 224/65536.0f, 64/65536.0f, 8/65536.0f},
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 9; k++)
        for(int t = 0; t < 9; t++){
            Color c = get_color(src, j+t-1, i+k-1);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k][t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}