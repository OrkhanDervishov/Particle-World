#include "basic_text_renderer.h"


// Surface* fontSurface;

Image font_image;

int fontWidth;
int fontHeight;

int InitBasicTextRenderer(){

    fontWidth = DEFAULT_FONT_WIDTH;
    fontHeight = DEFAULT_FONT_HEIGHT;
    create_image(&font_image, fontWidth, fontHeight);
}

void EndBasicTextRenderer(){
    delete_image(&font_image);
}

int SymbolToImage(char sym, Color color){

    for(size_t i = 0; i < fontHeight; i++)
    for(size_t j = 0; j < fontWidth; j++){
        font_image.buffer[i*fontWidth + j].rgba = (int)fontList[sym][i][j]*color.rgba;
    }

    return 0;
}

void BasicSymbolRender(Window* window, char sym, int x, int y, int scale, Color color){
    SymbolToImage(sym, color);
    draw_image_on_fimage_scaled(window->context, font_image, x, y, scale, scale);
}

void BasicTextRender(Window* window, const char* text, int x, int y, int scale, Color color){
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
