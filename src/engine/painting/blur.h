#ifndef BLUR_H
#define BLUR_H

#include "painter.h"

void gaussian_blur_3x3(Image dest, Image src);
void gaussian_blur_5x5(Image dest, Image src);
void gaussian_blur_7x7(Image dest, Image src);
void gaussian_blur_9x9(Image dest, Image src);

void gaussian_blur_vertical_3x3(Image dest, Image src);
void gaussian_blur_horizontal_3x3(Image dest, Image src);
void gaussian_blur_vertical_5x5(Image dest, Image src);
void gaussian_blur_horizontal_5x5(Image dest, Image src);
void gaussian_blur_vertical_7x7(Image dest, Image src);
void gaussian_blur_horizontal_7x7(Image dest, Image src);
void gaussian_blur_vertical_9x9(Image dest, Image src);
void gaussian_blur_horizontal_9x9(Image dest, Image src);

void gaussian_blur_vertical(Image dest, Image src, int kernel_size);
void gaussian_blur_horizontal(Image dest, Image src, int kernel_size);

#endif