#include "blur.h"

static inline Colorf scale_colorf(Colorf colorf, float scalar){
    return (Colorf){
        colorf.r * scalar,
        colorf.g * scalar,
        colorf.b * scalar,
        colorf.a
    };
}


void gaussian_blur_3x3(Image dest, Image src){

    /*   ___________
        |___|___|___|
        |___|_*_|___|
        |___|___|___|
    */
    float divisor = 16.0f;
    float mult = 1.0f;
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
    float divisor = 256.0f;
    float kernel[5][5] = {
        {1.0f/divisor,  4.0f/divisor,  6.0f/divisor,  4.0f/divisor, 1.0f/divisor},
        {4.0f/divisor, 16.0f/divisor, 24.0f/divisor, 16.0f/divisor, 4.0f/divisor},
        {6.0f/divisor, 24.0f/divisor, 36.0f/divisor, 24.0f/divisor, 6.0f/divisor},
        {4.0f/divisor, 16.0f/divisor, 24.0f/divisor, 16.0f/divisor, 4.0f/divisor},
        {1.0f/divisor,  4.0f/divisor,  6.0f/divisor,  4.0f/divisor, 1.0f/divisor}
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 5; k++)
        for(int t = 0; t < 5; t++){
            Color c = get_color(src, j+t-2, i+k-2);
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
    float divisor = 4096.0f;
    float kernel[7][7] = {
        {1.0f/divisor,  6.0f/divisor, 15.0f/divisor, 20.0f/divisor, 15.0f/divisor,  6.0f/divisor, 1.0f/divisor},
        {6.0f/divisor, 36.0f/divisor, 90.0f/divisor,120.0f/divisor, 90.0f/divisor, 36.0f/divisor, 6.0f/divisor},
        {15.0f/divisor,90.0f/divisor,225.0f/divisor,300.0f/divisor,225.0f/divisor, 90.0f/divisor,15.0f/divisor},
        {20.0f/divisor,120.0f/divisor,300.0f/divisor,400.0f/divisor,300.0f/divisor,120.0f/divisor,20.0f/divisor},
        {15.0f/divisor,90.0f/divisor,225.0f/divisor,300.0f/divisor,225.0f/divisor, 90.0f/divisor,15.0f/divisor},
        {6.0f/divisor,36.0f/divisor, 90.0f/divisor,120.0f/divisor, 90.0f/divisor, 36.0f/divisor, 6.0f/divisor},
        {1.0f/divisor,  6.0f/divisor, 15.0f/divisor, 20.0f/divisor, 15.0f/divisor,  6.0f/divisor, 1.0f/divisor}
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 7; k++)
        for(int t = 0; t < 7; t++){
            Color c = get_color(src, j+t-3, i+k-3);
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
            Color c = get_color(src, j+t-4, i+k-4);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k][t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}


void gaussian_blur_horizontal_3x3(Image dest, Image src){

    /*   ___________
        |___|_*_|___|
    */
    float divisor = 4.0f;
    float mult = 1.0f;
    float kernel[3] = {
        1.0f/divisor,
        2.0f/divisor,
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int t = 0; t < 3; t++){
            Color c = get_color(src, j+t-1, i);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
void gaussian_blur_vertical_3x3(Image dest, Image src){

    /*   ___
        |___|
        |_*_|
        |___|
    */
    float divisor = 4.0f;
    float mult = 1.0f;
    float kernel[3] = {
        1.0f/divisor,
        2.0f/divisor,
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 3; k++){
            Color c = get_color(src, j, i+k-1);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_horizontal_5x5(Image dest, Image src){
    float divisor = 19.0f;
    float kernel[5] = {
        1.0f/divisor, 
        4.0f/divisor, 
        9.0f/divisor, 
        4.0f/divisor, 
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int t = 0; t < 5; t++){
            Color c = get_color(src, j+t-4, i);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
void gaussian_blur_vertical_5x5(Image dest, Image src){
    float divisor = 19.0f;
    float kernel[5] = {
        1.0f/divisor, 
        4.0f/divisor, 
        9.0f/divisor, 
        4.0f/divisor, 
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 5; k++){
            Color c = get_color(src, j, i+k-4);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_horizontal_7x7(Image dest, Image src){
    float divisor = 64.0f;
    float kernel[7] = {
        1.0f/divisor,
        6.0f/divisor,
        15.0f/divisor,
        20.0f/divisor,
        15.0f/divisor,
        6.0f/divisor,
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int t = 0; t < 7; t++){
            Color c = get_color(src, j+t-4, i);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
void gaussian_blur_vertical_7x7(Image dest, Image src){
    float divisor = 64.0f;
    float kernel[7] = {
        1.0f/divisor,
        6.0f/divisor,
        15.0f/divisor,
        20.0f/divisor,
        15.0f/divisor,
        6.0f/divisor,
        1.0f/divisor
    };
    
    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 7; k++){
            Color c = get_color(src, j, i+k-4);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}

void gaussian_blur_horizontal_9x9(Image dest, Image src){
    float divisor = 256.0f;
    float kernel[9] = {
        1/divisor,
        8/divisor,
        28/divisor,
        56/divisor,
        70/divisor,
        56/divisor,
        28/divisor,
        8/divisor,
        1/divisor
    };

    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int t = 0; t < 9; t++){
            Color c = get_color(src, j+t-4, i);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
void gaussian_blur_vertical_9x9(Image dest, Image src){
    float divisor = 256.0f;
    float kernel[9] = {
        1/divisor,
        8/divisor,
        28/divisor,
        56/divisor,
        70/divisor,
        56/divisor,
        28/divisor,
        8/divisor,
        1/divisor
    };

    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < 9; k++){
            Color c = get_color(src, j, i+k-4);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}


// Kernel for 3x3
const float DIVISOR_3 = 4.0f;
float kernel_3[3] = {
    1.0f/DIVISOR_3,
    2.0f/DIVISOR_3,
    1.0f/DIVISOR_3
};

// Kernel for 5x5
const float DIVISOR_5 = 19.0f;
float kernel_5[5] = {
    1.0f/DIVISOR_5, 
    4.0f/DIVISOR_5, 
    9.0f/DIVISOR_5, 
    4.0f/DIVISOR_5, 
    1.0f/DIVISOR_5
};

// Kernel for 7x7
const float DIVISOR_7 = 64.0f;
float kernel_7[7] = {
    1.0f/DIVISOR_7,
    6.0f/DIVISOR_7,
    15.0f/DIVISOR_7,
    20.0f/DIVISOR_7,
    15.0f/DIVISOR_7,
    6.0f/DIVISOR_7,
    1.0f/DIVISOR_7
};

// Kernel for 9x9
const float DIVISOR_9 = 256.0f;
float kernel_9[9] = {
    1 /DIVISOR_9 ,
    8 /DIVISOR_9,
    28/DIVISOR_9,
    56/DIVISOR_9,
    70/DIVISOR_9,
    56/DIVISOR_9,
    28/DIVISOR_9,
    8 /DIVISOR_9,
    1 /DIVISOR_9
};


void gaussian_blur_horizontal(Image dest, Image src, int kernel_size){
    
    float* kernel;
    switch (kernel_size)
    {
    case 3:
        kernel = kernel_3;
        break;
    case 5:
        kernel = kernel_5;
        break;
    case 7:
        kernel = kernel_7;
        break;
    case 9:
        kernel = kernel_9;
        break;
    default:
        printf("no horizontal kernel for %dx%d\n", kernel_size);
        return;
    }

    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int t = 0; t < kernel_size; t++){
            Color c = get_color(src, j+t-kernel_size/2, i);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[t]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
void gaussian_blur_vertical(Image dest, Image src, int kernel_size){

    float* kernel;
    switch (kernel_size)
    {
    case 3:
        kernel = kernel_3;
        break;
    case 5:
        kernel = kernel_5;
        break;
    case 7:
        kernel = kernel_7;
        break;
    case 9:
        kernel = kernel_9;
        break;
    default:
        printf("no vertical kernel for %d\n", kernel_size);
        return;
    }

    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        Colorf sum_color = {0.0f, 0.0f, 0.0f, 1.0f};
        for(int k = 0; k < kernel_size; k++){
            Color c = get_color(src, j, i+k-kernel_size/2);
            Colorf cf = color_to_colorf(c);
            sum_color = add_colorf(sum_color, cf, kernel[k]);
        }
        Color color = colorf_to_color(sum_color);
        IMG_GET(dest, j, i) = color;
    }
}
