#ifndef BASIC_TEXT_RENDERER_H
#define BASIC_TEXT_RENDERER_H

/*
    Basic font renderer renders only basic fonts
*/

#include "../engine/engine_lib.h"
#include "basic_fonts.h"

int InitBasicTextRenderer();
void EndBasicTextRenderer();

void BasicSymbolRender(Window* window, char sym, int x, int y, int scale, Color Color);
void BasicTextRender(Window* window, const char* text, int x, int y, int scale, Color Color);

#endif