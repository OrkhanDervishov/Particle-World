#ifndef BASIC_FONTS_H
#define BASIC_FONTS_H

// abcdefghijklmnopqrstuvwxyz1234567890.,-   /\|?[]{}():;'"<>!@#$%^&*+=_

// Basic font system can use only ascii characters
#define MAX_SYMBOL_COUNT 256
#define DEFAULT_FONT_WIDTH 5
#define DEFAULT_FONT_HEIGHT 6
#define DEFAULT_FONT_OFFSET 1

extern char fontList[MAX_SYMBOL_COUNT][DEFAULT_FONT_HEIGHT][DEFAULT_FONT_WIDTH];

// void InitBasicFonts();

#endif