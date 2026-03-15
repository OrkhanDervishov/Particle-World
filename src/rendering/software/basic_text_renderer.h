#ifndef BASIC_TEXT_RENDERER_H
#define BASIC_TEXT_RENDERER_H

/*
    Basic font renderer renders only basic fonts
*/

#include "engine_lib.h"
#include "basic_fonts.h"

#define INFO_TEXT_COLOR 0xFFBA9800
#define WARNING_TEXT_COLOR 0xFF00BEFE
#define ERROR_TEXT_COLOR 0xFF020A7C

int InitBasicTextRenderer();
void EndBasicTextRenderer();

void BasicSymbolRender(Window* window, char sym, int x, int y, int scale, Color Color);
void BasicTextRender(Window* window, const char* text, int x, int y, int scale, Color Color);

#endif