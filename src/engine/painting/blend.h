#ifndef BLEND_H
#define BLEND_H

#include "painter.h"

static inline Color add_color(Color a, Color b);

void additive_blend(Image dest, Image src);

#endif