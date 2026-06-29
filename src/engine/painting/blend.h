#ifndef BLEND_H
#define BLEND_H

#include "painter.h"

// static inline Color add_color(Color a, Color b);

void pnt_additive_blend(PNTImage dest, PNTImage src);
void pnt_alpha_blend(PNTImage dest, PNTImage src);

#endif