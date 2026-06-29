#ifndef BLUR_H
#define BLUR_H

#include "painter.h"

void pnt_gaussian_blur_3x3(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_5x5(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_7x7(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_9x9(PNTImage dest, PNTImage src);

void pnt_gaussian_blur_vertical_3x3(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_horizontal_3x3(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_vertical_5x5(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_horizontal_5x5(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_vertical_7x7(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_horizontal_7x7(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_vertical_9x9(PNTImage dest, PNTImage src);
void pnt_gaussian_blur_horizontal_9x9(PNTImage dest, PNTImage src);

void pnt_gaussian_blur_vertical(PNTImage dest, PNTImage src, int kernel_size);
void pnt_gaussian_blur_horizontal(PNTImage dest, PNTImage src, int kernel_size);

#endif