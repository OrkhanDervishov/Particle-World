#ifndef RENDERER_SOFTWARE_H
#define RENDERER_SOFTWARE_H

#include "chunk_renderer_sw.h"
#include "basic_text_renderer.h"

// Software rendering
void clear_window(Window *window, Color color);
void gen_light_chunk(Chunk* chunk);
// void gen_light_space(C)

#endif