#include "basic_text_renderer.h"


Surface* fontSurface;
int fontWidth;
int fontHeight;

int InitBasicTextRenderer(){
    if(fontSurface != NULL){
        SDL_FreeSurface(fontSurface);
    }

    fontWidth = DEFAULT_FONT_WIDTH;
    fontHeight = DEFAULT_FONT_HEIGHT;
    fontSurface = SDL_CreateRGBSurface(
        0, DEFAULT_FONT_WIDTH, DEFAULT_FONT_HEIGHT,
        32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
    );
}

void EndBasicTextRenderer(){
    if(fontSurface == NULL) return;
    SDL_FreeSurface(fontSurface);
}

int SymbolToSurface(char sym, Color color){
        
    uint32_t *pixels = (uint32_t *)fontSurface->pixels;
    for(size_t i = 0; i < fontHeight; i++)
    for(size_t j = 0; j < fontWidth; j++){
        pixels[i*fontWidth + j] = (int)fontList[sym][i][j]*color.rgba;
    }

    return 0;
}

void BasicSymbolRender(Window* window, char sym, int x, int y, int scale, Color color){
    SymbolToSurface(sym, color);
    Surface* winSurface = SDL_GetWindowSurface(window->window);
    
    SDL_Rect srcRect = {0, 0, fontWidth, fontHeight};
    SDL_Rect dstRect = {x, y, fontWidth*scale, fontHeight*scale};
    SDL_BlitScaled(fontSurface, &srcRect, winSurface, &dstRect);
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
