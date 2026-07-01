#include "basic_text_renderer.h"
#include "asset_renderer.h"


#define HT_IMPLEMENTATION
#include "ht.h"


/******************************************************/
// Surface* fontSurface;

Image font_image;

int fontWidth;
int fontHeight;

int InitBasicTextRenderer(){

    fontWidth = DEFAULT_FONT_WIDTH;
    fontHeight = DEFAULT_FONT_HEIGHT;
    pnt_create_image(&font_image, fontWidth, fontHeight);
}

void EndBasicTextRenderer(){
    pnt_delete_image(&font_image);
}

int SymbolToImage(char sym, Color color){

    for(size_t i = 0; i < fontHeight; i++)
    for(size_t j = 0; j < fontWidth; j++){
        font_image.buffer[i*fontWidth + j].rgba = (int)fontList[sym][i][j]*color.rgba;
    }

    return 0;
}

void BasicSymbolRender(Window* window, char sym, int x, int y, float scale, Color color){
    // return;
    SymbolToImage(sym, color);
    pnt_blit_scaled(window->context, font_image, x, y, scale, scale);
}

void BasicTextRender(Window* window, const char* text, int x, int y, float scale, Color color){
    int len = strlen(text);

    for(int i = 0, k = 0; i < len; i++){
        if(text[i] == '\n'){
            y += fontHeight*scale + 4*DEFAULT_FONT_OFFSET;
            k = 0;
            continue;
        }
        BasicSymbolRender(
            window, text[i], 
            x + k*fontWidth*scale+DEFAULT_FONT_OFFSET,
            y, scale, color
        );
        k++;
    }
}

/******************************************************/




/******************************************************/

void pw_free_sprite_font(PWSpriteFonts *fonts){
    ht_free(&fonts->char_map);
}

PWSpriteFonts pw_load_sprite_fonts_file(pw_asset_t sprite, const char* path){
    PWSpriteFonts fonts;

    fonts.fonts_sprite = sprite;
    fonts.char_map.hasheq = ht_cstr_hasheq;

    // TODO: File loading and parser

    return fonts;
}

PWSpriteFonts pw_load_sprite_fonts_manual(pw_asset_t sprite, const char* chars){
    PWSpriteFonts fonts = {
        .fonts_sprite = sprite,
        .char_map = {
            .hasheq = ht_mem_hasheq
        }
    };

    size_t chars_len = strlen(chars);

    for(size_t i = 0; i < chars_len; i++){
        *ht_put(&fonts.char_map, chars[i]) = i;
    }

    return fonts;
}

int pw_load_sprite_font_manual(PWSpriteFonts *fonts, size_t index, char character){
    *ht_put(&fonts->char_map, character) = index;
    return 0;
}

void pw_draw_sprite_symbol(Image context, PWAssetManager *am, PWSpriteFonts *fonts, const char sym, Transforms2d transforms){
    size_t *indexp;
    size_t index;

    indexp = ht_find(&fonts->char_map, sym);
    if(indexp == NULL){
        return;
    }
    index = *indexp;

    pw_draw_sprite_multiple(context, am, fonts->fonts_sprite, transforms, index);
}

void pw_draw_sprite_text(Image context, PWAssetManager *am, PWSpriteFonts *fonts, const char* text, Transforms2d transforms){
    size_t text_len = strlen(text);

    float font_width = pw_asset_manager_get_sprite_offsetx(am, fonts->fonts_sprite);
    float font_offset = font_width*transforms.scale.x;

    for(size_t i = 0; i < text_len; i++){
        transforms.translation.x += font_offset;
        pw_draw_sprite_symbol(context, am, fonts, text[i], transforms);
    }
}

/******************************************************/