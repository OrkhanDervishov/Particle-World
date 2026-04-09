#ifndef BLUR_H
#define BLUR_H

#include "painter.h"

void gaussian_blur_3x3(Image dest, Image src);
void gaussian_blur_5x5(Image dest, Image src);
void gaussian_blur_7x7(Image dest, Image src);
void gaussian_blur_9x9(Image dest, Image src);

#endif