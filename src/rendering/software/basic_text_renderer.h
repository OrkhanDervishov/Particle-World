#ifndef BASIC_TEXT_RENDERER_H_
#define BASIC_TEXT_RENDERER_H_

/*
    Basic font renderer renders only basic fonts
*/

#include "basic_fonts.h"
#include "window.h"
#include "asset.h"
#include "ht.h"

/******************************************************/

#define INFO_TEXT_COLOR 0xFFBA9800
#define WARNING_TEXT_COLOR 0xFF00BEFE
#define ERROR_TEXT_COLOR 0xFF020A7C

int InitBasicTextRenderer();
void EndBasicTextRenderer();

void BasicSymbolRender(Window* window, char sym, int x, int y, float scale, Color Color);
void BasicTextRender(Window* window, const char* text, int x, int y, float scale, Color Color);

/******************************************************/



/******************************************************/

typedef struct{
    pw_asset_t fonts_sprite;
    Ht(char, size_t) char_map;
} PWSpriteFonts;


void pw_free_sprite_font(PWSpriteFonts *fonts);
PWSpriteFonts pw_load_sprite_fonts_file(pw_asset_t sprite, const char* path);
PWSpriteFonts pw_load_sprite_fonts_manual(pw_asset_t sprite, const char* chars);
int pw_load_sprite_font_manual(PWSpriteFonts *fonts, size_t index, char character);

void pw_draw_sprite_symbol(Image context, PWAssetManager *am, PWSpriteFonts *fonts, const char sym, Transforms2d transforms);
void pw_draw_sprite_text(Image context, PWAssetManager *am, PWSpriteFonts *fonts, const char* text, Transforms2d transforms);

/******************************************************/

#endif //BASIC_TEXT_RENDERER_H_