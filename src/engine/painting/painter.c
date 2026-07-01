#include "painter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "geometry.h"



// Basics
//##################################################################

inline PNTColorf pnt_color_to_colorf(PNTColor color){
    return (PNTColorf){
        .r = ((float)color.r / 255.0f),
        .g = ((float)color.g / 255.0f),
        .b = ((float)color.b / 255.0f),
        .a = ((float)color.a / 255.0f)
    };
}

inline PNTColor pnt_colorf_to_color(PNTColorf colorf){
    return (PNTColor){
        .r = (uint16_t)(colorf.r * 255.0f) > 255 ? 255 : (uint8_t)(colorf.r * 255.0f),
        .g = (uint16_t)(colorf.g * 255.0f) > 255 ? 255 : (uint8_t)(colorf.g * 255.0f),
        .b = (uint16_t)(colorf.b * 255.0f) > 255 ? 255 : (uint8_t)(colorf.b * 255.0f),
        .a = (uint16_t)(colorf.a * 255.0f) > 255 ? 255 : (uint8_t)(colorf.a * 255.0f)
    };
}

inline PNTColorf pnt_add_colorf(PNTColorf a, PNTColorf b, float scale){
    return (PNTColorf){
        a.r + b.r*scale,
        a.g + b.g*scale,
        a.b + b.b*scale,
        a.a + b.a*scale// + b.a*scale
    };
}

inline PNTColor pnt_add_color(PNTColor a, PNTColor b){
    return (PNTColor){
        .r = ((uint16_t)a.r + (uint16_t)b.r) >= 255 ? 255 : a.r + b.r,
        .g = ((uint16_t)a.g + (uint16_t)b.g) >= 255 ? 255 : a.g + b.g,
        .b = ((uint16_t)a.b + (uint16_t)b.b) >= 255 ? 255 : a.b + b.b,
        // .a = ((uint16_t)a.a + (uint16_t)b.a) >= 255 ? 255 : a.a + b.a
        .a = a.a
    };
}

inline PNTColor pnt_get_random_color(){
    return (PNTColor){
        .r = rand() & 255,
        .g = rand() & 255,
        .b = rand() & 255,
        .a = 255
    };
}

inline PNTColor pnt_get_negative(PNTColor color){
    return (PNTColor){
        .r = 255 - color.r,
        .g = 255 - color.g,
        .b = 255 - color.b,
        .a = color.a,
    };
}

inline int pnt_get_negative_f(int fcolor, PNTPixelFormat format){

    int r = 0xFF - fcolor >> (format).r_shift;
    int b = 0xFF - fcolor >> (format).g_shift;
    int g = 0xFF - fcolor >> (format).b_shift;
    int a = fcolor >> (format).a_shift;

    return r<<(format).r_shift | g<<(format).g_shift | b<<(format).b_shift | a<<(format).a_shift;
}

inline PNTColor pnt_get_color(PNTImage image, int x, int y){
    if(
        x < 0 || x >= image.width ||
        y < 0 || y >= image.height
    ){
        return (PNTColor){.rgba=0x00000000};
    }
    return PNT_IMG_GET(image, x, y);
}

inline PNTColor pnt_colors_alpha_blend(PNTColor down, PNTColor up){
    return (PNTColor){
            .r = (((down).r * (255-(up).a)) + ((up).r * (up).a) >> 8),
            .g = (((down).g * (255-(up).a)) + ((up).g * (up).a) >> 8),
            .b = (((down).b * (255-(up).a)) + ((up).b * (up).a) >> 8),
            .a = (((down).a * (255-(up).a)) + ((up).a * (up).a) >> 8)
        };
}


PNTPixelFormat pnt_create_format(int r_mask, int g_mask, int b_mask, int a_mask){
    PNTPixelFormat format;
    format.r_mask = r_mask;
    format.g_mask = g_mask;
    format.b_mask = b_mask;
    format.a_mask = a_mask;

    format.r_shift = 0;
    format.g_shift = 0;
    format.b_shift = 0;
    format.a_shift = 0;


    while((r_mask & 1) == 0 && r_mask){
        r_mask >>= 1;
        format.r_shift++;
    }
    while((g_mask & 1) == 0 && g_mask){
        g_mask >>= 1;
        format.g_shift++;
    }
    while((b_mask & 1) == 0 && b_mask){
        b_mask >>= 1;
        format.b_shift++;
    }
    while((a_mask & 1) == 0 && a_mask){
        a_mask >>= 1;
        format.a_shift++;
    }

    return format;
}

pnt_color_t pnt_get_formatted_color(PNTColor color, PNTPixelFormat format){
    return  PNT_GET_FCOLOR(color, format);
}

PNTColor pnt_get_unformatted_color(int fcolor, PNTPixelFormat format){
    PNTColor color = PNT_GET_COLOR(fcolor, format);
    return color;
}

// Maybe will be moved to geometry.c
vec2 pnt_correct_line_end(int x, int y, int dx, int dy, int width, int height){
    if(x > width - 1){
        y -= dy*(x - (width - 1))/dx;
        x = width - 1;
    } else if(x < 0){
        y -= dy*x/dx;
        x = 0;
    }
    if(y > height - 1){
        x -= dx*(y - (height - 1))/dy;
        y = height - 1;
    } else if(y < 0){
        x -= dx*y/dy;
        y = 0;
    }

    vec2 p = {
        .x = x,
        .y = y,
    };

    return p;
}
//##################################################################



// Create, Delete
//##################################################################

int pnt_create_similar(PNTImage* dest, PNTImage src){
    if(dest->buffer != NULL){
        pnt_delete_image(dest);
    }
    return pnt_create_image(dest, src.width, src.height);
}

int pnt_copy_image_content(PNTImage dest, PNTImage src){
    if(dest.buffer == NULL || src.buffer == NULL) return 1;
    if(dest.width != src.width || dest.height != src.height) return 2;

    for(int i = 0; i < dest.height; i++)
    for(int j = 0; j < dest.width; j++)
        PNT_IMG_GET(dest, j, i) = PNT_IMG_GET(src, j, i);

    return 0;
}

int pnt_copy_image(PNTImage* dest, PNTImage src){
    if(dest->buffer != NULL){
        pnt_delete_image(dest);
    }
    int res = pnt_create_image(dest, src.width, src.height);
    if(!res) return res;

    for(int i = 0; i < src.height; i++)
    for(int j = 0; j < src.width; j++){
        PNT_IMGP_GET(dest, j, i) = PNT_IMG_GET(src, j, i);
    }

    return res;
}

int pnt_create_image(PNTImage* img, size_t w, size_t h){
    if(img->buffer != NULL){
        pnt_delete_image(img);
        return 1;
    }
    img->buffer = (PNTColor*)malloc(w * h * sizeof(PNTColor));
    img->width = w;
    img->height = h;
    img->stride = w;
    return 0;
}

void pnt_delete_image(PNTImage* img){
    free(img->buffer);
    img->buffer = NULL;
}


void pnt_create_fimage(
    PNTFormatImage* fimg, size_t w, size_t h, 
    int r_mask, int g_mask, int b_mask, int a_mask
){
    if(fimg->buffer != NULL) pnt_delete_fimage(fimg);
    fimg->buffer = (int*)malloc(w * h * sizeof(int));
    fimg->width = w;
    fimg->height = h;
    fimg->stride = w;
    fimg->format = pnt_create_format(r_mask, g_mask, b_mask, a_mask);
}

void pnt_delete_fimage(PNTFormatImage* fimg){
    free(fimg->buffer);
}

//##################################################################

// Conversion
//##################################################################

int pnt_image_to_fimage(PNTImage img, PNTFormatImage fimg){
    if(fimg.buffer == NULL && img.buffer == NULL) return 1;
    if(fimg.width != img.width || fimg.height != img.height) return 2;

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        PNTColor color = PNT_IMG_GET(img, j, i);
        int fcolor = (
            (uint32_t)color.r << fimg.format.r_shift |
            (uint32_t)color.g << fimg.format.g_shift |
            (uint32_t)color.b << fimg.format.b_shift |
            (uint32_t)color.a << fimg.format.a_shift
        );
        PNT_IMG_GET(fimg, j, i) = fcolor;
    }

    return 0;
}

int pnt_fimage_to_image(PNTFormatImage fimg, PNTImage img){
    if(fimg.buffer == NULL && img.buffer == NULL) return 1;
    if(fimg.width != img.width || fimg.height != img.height) return 2;

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        int fcolor = PNT_IMG_GET(fimg, j, i);
        PNT_IMG_GET(img, j, i) = (PNTColor)PNT_GET_COLOR(fcolor, fimg.format);
    }

    return 0;
}

//##################################################################

// Drawing on one another
//##################################################################

void pnt_blit(PNTImage dest, PNTImage src, int x, int y){
    Rect rect = {.x = x, .y = y, .w = src.width, .h = src.height};
    rect = CorrectRect(rect, dest.width, dest.height);

    int endX = rect.x + rect.w;
    int endY = rect.y + rect.h;
    for(int i = rect.y, k = 0; i < endY; i++, k++)
    for(int j = rect.x, t = 0; j < endX; j++, t++){
        PNT_IMG_GET(dest, j, i) = PNT_IMG_GET(src, t, k);
    }
}

void pnt_blit_scaled(PNTImage dest, PNTImage src, int x, int y, float scaleX, float scaleY){
    Rect rect = {.x = x, .y = y, .w = (int)((float)src.width*scaleX), .h = (int)((float)src.height*scaleY)};

    Rect rect2 = {.x = 0, .y = 0, .w = dest.width, .h = dest.height};
    vec2 start_pos = {.x = x, .y = y};
    Rect rect3 = get_intersection_rect2(rect, rect2);
    start_pos = correct_start_pos(rect2, start_pos);

    float addX = 1.0f/scaleX;
    float addY = 1.0f/scaleY;
    // float k = 0.0f, t = 0.0f;
    float base_t = (float)rect3.x*addX;
    float base_k = (float)rect3.y*addY;
    float t = base_t, k = base_k;
    int prev_k = -1, prev_t = -1;
    // int endX = rect.x + rect.w;
    // int endY = rect.y + rect.h;
    int endX = rect.x + rect2.w;
    int endY = rect.y + rect2.h;
    PNTColor src_color;
    PNTColor res_color;
    for(int i = start_pos.y; i < rect3.h; i++, k += addY){
        for(int j = start_pos.x; j < rect3.w; j++, t += addX){
            // if((int)k != prev_k || (int)t != prev_t){
            //     PNTColor dest_color = PNT_IMG_GET(dest, j, i);
            //     PNTColor src_color = PNT_IMG_GET(src, (int)t, (int)k);
            //     res_color = pnt_colors_alpha_blend(src_color, dest_color);
            //     prev_k = (int)k;
            //     prev_t = (int)t;
            // }
            // PNT_IMG_GET(dest, j, i) = res_color;
            if((int)k != prev_k || (int)t != prev_t){
                src_color = PNT_IMG_GET(src, (int)t, (int)k);
                prev_k = (int)k;
                prev_t = (int)t;
            }
            if(src_color.a == 0) continue;
            PNTColor dest_color = PNT_IMG_GET(dest, j, i);
            res_color = pnt_colors_alpha_blend(dest_color, src_color);
            PNT_IMG_GET(dest, j, i) = res_color;
        }
        t = base_t;
    }
}


void pnt_blit_transformed(PNTImage dest, PNTImage src, vec2f pos, float rotation, vec2f scale){
    float cx = (float)src.width/2;
    float cy = (float)src.height/2;
    float s = sin(rotation);
    float c = cos(rotation);

    vec2f v[4];
    
    // scaling
    v[0] = (vec2f){-cx * scale.x,  cy * scale.y};
    v[1] = (vec2f){ cx * scale.x,  cy * scale.y};
    v[2] = (vec2f){ cx * scale.x, -cy * scale.y};
    v[3] = (vec2f){-cx * scale.x, -cy * scale.y};
    // rotation
    float tempx;
    tempx = v[0].x, v[0].x = c*v[0].x + s*v[0].y, v[0].y = -s*tempx + c*v[0].y;
    tempx = v[1].x, v[1].x = c*v[1].x + s*v[1].y, v[1].y = -s*tempx + c*v[1].y;
    tempx = v[2].x, v[2].x = c*v[2].x + s*v[2].y, v[2].y = -s*tempx + c*v[2].y;
    tempx = v[3].x, v[3].x = c*v[3].x + s*v[3].y, v[3].y = -s*tempx + c*v[3].y;
    // translation
    v[0] = vec2_sum(v[0], pos);
    v[1] = vec2_sum(v[1], pos);
    v[2] = vec2_sum(v[2], pos);
    v[3] = vec2_sum(v[3], pos);

    int minx = (int)v[0].x;
    int miny = (int)v[0].y;
    int maxx = (int)v[0].x;
    int maxy = (int)v[0].y;
    for(int i = 0; i < 4; i++) if(minx > (int)v[i].x) minx = (int)v[i].x;
    for(int i = 0; i < 4; i++) if(miny > (int)v[i].y) miny = (int)v[i].y;
    for(int i = 0; i < 4; i++) if(maxx < (int)v[i].x) maxx = (int)v[i].x;
    for(int i = 0; i < 4; i++) if(maxy < (int)v[i].y) maxy = (int)v[i].y;
    


    float div_sin_scalex    = s/scale.x;
    float div_cos_scalex    = c/scale.x;
    float div_msin_scaley   = -s/scale.y;
    float div_cos_scaley    = c/scale.y;

    for(int v = miny; v < maxy; v++){
        if(v < 0 || v >= dest.height) continue;
        float ry = (float)v - pos.y;

        float pre_comp_y_sx = ry*div_sin_scalex + cx;
        float pre_comp_y_sy = ry*div_cos_scaley + cy;

        for(int u = minx; u < maxx; u++){
            if(u < 0 || u >= dest.width) continue;
            
            float rx = (float)u - pos.x;

            float sx = div_cos_scalex*rx + pre_comp_y_sx;
            float sy = div_msin_scaley*rx + pre_comp_y_sy;

            if(sx < 0.0f || sy < 0.0f || sx >= src.width || sy >= src.height) continue;


            PNTColor src_color = PNT_IMG_GET(src, (int)sx, (int)sy);
            if(src_color.a == 0) continue;

            PNTColor dest_color = PNT_IMG_GET(dest, u, v);
            PNT_IMG_GET(dest, u, v) = pnt_colors_alpha_blend(dest_color, src_color);
        }
    }
}


// void draw_image_on_fimage(PNTFormatImage dest, PNTImage src, int x, int y){
//     Rect rect = {.x = x, .y = y, .w = src.width, .h = src.height};
//     rect = CorrectRect(rect, dest.width, dest.height);

//     int endX = rect.x + rect.w;
//     int endY = rect.y + rect.h;
//     for(int i = rect.y, k = 0; i < endY; i++, k++)
//     for(int j = rect.x, t = 0; j < endX; j++, t++){
//         PNTColor dest_color = PNT_GET_COLOR(PNT_IMG_GET(dest, j, i), dest.format);
//         PNTColor src_color = PNT_IMG_GET(src, (int)t, (int)k);
//         int res_fcolor = PNT_GET_FCOLOR(colors_alpha_blend(src_color, dest_color), dest.format);
//         PNT_IMG_GET(dest, j, i) = res_fcolor;//get_formatted_color(PNT_IMG_GET(src, t, k), dest.format);
//     }
// }

// void draw_image_on_fimage_scaled(PNTFormatImage dest, PNTImage src, int x, int y, int scaleX, int scaleY){
//     Rect rect = {.x = x, .y = y, .w = src.width*scaleX, .h = src.height*scaleY};
//     Rect rect2 = {.x = 0, .y = 0, .w = dest.width, .h = dest.height};
//     vec2 start_pos = {.x = x, .y = y};
//     Rect rect3 = get_intersection_rect2(rect, rect2);
//     start_pos = correct_start_pos(rect2, start_pos);
    
//     float addX = 1.0f/(float)scaleX;
//     float addY = 1.0f/(float)scaleY;
//     float base_t = (float)rect3.x*addX;
//     float base_k = (float)rect3.y*addY;
//     float t = base_t, k = base_k;
//     int prev_k = -1, prev_t = -1;
//     int endX = rect.x + rect2.w;
//     int endY = rect.y + rect2.h;
//     int res_fcolor;
//     PNTColor src_color;
//     for(int i = start_pos.y; i < rect3.h; i++, k += addY){
//         for(int j = start_pos.x; j < rect3.w; j++, t += addX){
//             // This condition breaks alpha blending
//             if((int)k != prev_k || (int)t != prev_t){
//                 src_color = PNT_IMG_GET(src, (int)t, (int)k);
//                 prev_k = (int)k;
//                 prev_t = (int)t;
//             }

//             if(src_color.a == 0) continue;
//             // PNT_IMG_GET(dest, j, i) = PNT_GET_FCOLOR(PNT_IMG_GET(src, (int)t, (int)k), dest.format);
//             PNTColor dest_color = PNT_GET_COLOR(PNT_IMG_GET(dest, j, i), dest.format);
//             res_fcolor = PNT_GET_FCOLOR(colors_alpha_blend(dest_color, src_color), dest.format);
        
//             PNT_IMG_GET(dest, j, i) = res_fcolor;
//         }
//         t = base_t;
//     }
// }

//##################################################################



// Main fucntions
//##################################################################

void pnt_change_color(PNTImage img, PNTColor dest, PNTColor src){
    int size = img.width * img.height;
    for(int i = 0; i < size; i++){
        if(img.buffer[i].rgba == dest.rgba) img.buffer[i] = src;
    }
}

void pnt_fill(PNTImage img, PNTColor color){
    for(int i = 0; i < img.height; i++)
    for(int j = 0; j < img.width; j++){
        PNT_IMG_GET(img, j, i) = color;
    }
}

void pnt_fill_f(PNTFormatImage fimg, PNTColor color){
    int fcolor = pnt_get_formatted_color(color, fimg.format);

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        PNT_IMG_GET(fimg, j, i) = fcolor;
    }
}

void pnt_put_pixel(PNTImage img, int x, int y, PNTColor color){
    if(x >= img.width || x < 0 || y >= img.height || y < 0) return;
    PNT_IMG_GET(img, x, y) = color;
}

void pnt_put_pixel_f(PNTFormatImage fimg, int x, int y, PNTColor color){
    if(x >= fimg.width || x < 0 || y >= fimg.height || y < 0) return;
    int fcolor = pnt_get_formatted_color(color, fimg.format);
    PNT_IMG_GET(fimg, x, y) = fcolor;
}




#define PUT_THICKNESS(img, x, y, cx, cy, color, thickness, side)   \
do{\
    int dx = 0;\
    int dy = 0;\
    for(int i = 0; i < (thickness)-1; i++){\
        if((side) == UP)       if(y+dy >= cy) dy += -1; \
        if((side) == DOWN)     if(y+dy <= cy) dy += 1;  \
        if((side) == LEFT)     if(x+dx >= cx) dx += -1; \
        if((side) == RIGHT)    if(x+dx <= cx) dx += 1;  \
        PNT_PUT_PIXEL(img, x+dx, y+dy, color);\
    }\
}while(0)

#define PUT_THICKNESS_NC(img, x, y, cx, cy, format, thickness, side)   \
do{\
    int dx = 0;\
    int dy = 0;\
    for(int i = 0; i < (thickness)-1; i++){\
        if((side) == UP)       if(y+dy >= cy) dy += -1; \
        if((side) == DOWN)     if(y+dy <= cy) dy += 1;  \
        if((side) == LEFT)     if(x+dx >= cx) dx += -1; \
        if((side) == RIGHT)    if(x+dx <= cx) dx += 1;  \
        int fcolor = pnt_get_negative_f(PNT_IMG_GET(img, x+dx, y+dy), format);\
        PNT_PUT_PIXEL(img, x+dx, y+dy, fcolor);\
    }\
}while(0)


// TODO: Add fill option
void pnt_draw_circle(PNTImage img, int cx, int cy, int radius, PNTColor color, int thickness){
    int x = 0;
    int y = -radius;
    int p = -radius;

    while(x >= y){
        if(p > 0){
            y++;
            p += 2*(x + y) + 1;
        } else {
            p += 2*x + 1;
        }

        PUT_THICKNESS(img, cx + x, cy + y, cx, cy, color, thickness, DOWN);
        PUT_THICKNESS(img, cx - x, cy + y, cx, cy, color, thickness, DOWN);
        PUT_THICKNESS(img, cx + x, cy - y, cx, cy, color, thickness, UP);
        PUT_THICKNESS(img, cx - x, cy - y, cx, cy, color, thickness, UP);
        PUT_THICKNESS(img, cx + y, cy + x, cx, cy, color, thickness, RIGHT);
        PUT_THICKNESS(img, cx + y, cy - x, cx, cy, color, thickness, RIGHT);
        PUT_THICKNESS(img, cx - y, cy + x, cx, cy, color, thickness, LEFT);
        PUT_THICKNESS(img, cx - y, cy - x, cx, cy, color, thickness, LEFT);

        x++;
    }
}


// void draw_circle_f(PNTFormatImage fimg, int cx, int cy, int radius, PNTColor color, int thickness){
    
//     // for(int i = 0; i < thickness; i++){
//         int x = 0;
//         int y = -radius;
//         int p = -radius;
//         int fcolor = PNT_GET_FCOLOR(color, fimg.format);

//         while(x < -y){
//             if(p > 0){
//                 y++;
//                 p += 2*(x + y) + 1;
//             } else {
//                 p += 2*x + 1;
//             }

//             PUT_THICKNESS(fimg, cx + x, cy + y, cx, cy, fcolor, thickness, DOWN);
//             PUT_THICKNESS(fimg, cx - x, cy + y, cx, cy, fcolor, thickness, DOWN);
//             PUT_THICKNESS(fimg, cx + x, cy - y, cx, cy, fcolor, thickness, UP);
//             PUT_THICKNESS(fimg, cx - x, cy - y, cx, cy, fcolor, thickness, UP);
//             PUT_THICKNESS(fimg, cx + y, cy + x, cx, cy, fcolor, thickness, RIGHT);
//             PUT_THICKNESS(fimg, cx + y, cy - x, cx, cy, fcolor, thickness, RIGHT);
//             PUT_THICKNESS(fimg, cx - y, cy + x, cx, cy, fcolor, thickness, LEFT);
//             PUT_THICKNESS(fimg, cx - y, cy - x, cx, cy, fcolor, thickness, LEFT);
            
//             // Negative circle creation trials

//             // PNT_PUT_PIXEL(fimg, cx + x, cy + y, get_negative_f(PNT_IMG_GET(fimg, cx + x, cy + y), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx - x, cy + y, get_negative_f(PNT_IMG_GET(fimg, cx - x, cy + y), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx + x, cy - y, get_negative_f(PNT_IMG_GET(fimg, cx + x, cy - y), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx - x, cy - y, get_negative_f(PNT_IMG_GET(fimg, cx - x, cy - y), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx + y, cy + x, get_negative_f(PNT_IMG_GET(fimg, cx + y, cy + x), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx + y, cy - x, get_negative_f(PNT_IMG_GET(fimg, cx + y, cy - x), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx - y, cy + x, get_negative_f(PNT_IMG_GET(fimg, cx - y, cy + x), fimg.format));
//             // PNT_PUT_PIXEL(fimg, cx - y, cy - x, get_negative_f(PNT_IMG_GET(fimg, cx - y, cy - x), fimg.format));
            
//             // PUT_THICKNESS_NC(fimg, cx + x, cy + y, cx, cy, fimg.format, thickness, DOWN);
//             // PUT_THICKNESS_NC(fimg, cx - x, cy + y, cx, cy, fimg.format, thickness, DOWN);
//             // PUT_THICKNESS_NC(fimg, cx + x, cy - y, cx, cy, fimg.format, thickness, UP);
//             // PUT_THICKNESS_NC(fimg, cx - x, cy - y, cx, cy, fimg.format, thickness, UP);
//             // PUT_THICKNESS_NC(fimg, cx + y, cy + x, cx, cy, fimg.format, thickness, RIGHT);
//             // PUT_THICKNESS_NC(fimg, cx + y, cy - x, cx, cy, fimg.format, thickness, RIGHT);
//             // PUT_THICKNESS_NC(fimg, cx - y, cy + x, cx, cy, fimg.format, thickness, LEFT);
//             // PUT_THICKNESS_NC(fimg, cx - y, cy - x, cx, cy, fimg.format, thickness, LEFT);
            
//             x++;
//         }
//     // }
// }
#undef PUT_THICKNESS
#undef PUT_THICKNESS_NC


void pnt_draw_rect(PNTImage img, Rect rect, PNTColor color, int tickness){
    rect = CorrectRect(rect, img.width, img.height);

    Rect leftSide =     {.x = rect.x, .y = rect.y, .w = tickness, .h = rect.h};
    Rect rightSide =    {.x = rect.x + rect.w, .y = rect.y, .w = tickness, .h = rect.h};
    Rect upSide =       {.x = rect.x, .y = rect.y, .w = rect.w, .h = tickness};
    Rect bottomSide =   {.x = rect.x, .y = rect.y + rect.h, .w = rect.w, .h = tickness};

    pnt_draw_filled_rect(img, leftSide, color);
    pnt_draw_filled_rect(img, rightSide, color);
    pnt_draw_filled_rect(img, upSide, color);
    pnt_draw_filled_rect(img, bottomSide, color);
}

// void draw_rect_f(PNTFormatImage fimg, Rect rect, PNTColor color, int tickness){
//     rect = CorrectRect(rect, fimg.width, fimg.height);
//     int fcolor = get_formatted_color(color, fimg.format);

//     Rect leftSide =     {.x = rect.x, .y = rect.y, .w = tickness, .h = rect.h};
//     Rect rightSide =    {.x = rect.x + rect.w - tickness, .y = rect.y, .w = tickness, .h = rect.h};
//     Rect upSide =       {.x = rect.x, .y = rect.y, .w = rect.w, .h = tickness};
//     Rect bottomSide =   {.x = rect.x, .y = rect.y + rect.h - tickness, .w = rect.w, .h = tickness};

//     draw_filled_rect_f(fimg, leftSide, color);
//     draw_filled_rect_f(fimg, rightSide, color);
//     draw_filled_rect_f(fimg, upSide, color);
//     draw_filled_rect_f(fimg, bottomSide, color);
// }

void pnt_draw_filled_circle(PNTImage img, int x, int y, int radius, PNTColor color){
    Rect rect = {
        .x = x - radius,
        .y = y - radius,
        .w = radius*2,
        .h = radius*2
    };

    rect = CorrectRect(rect, img.width, img.height);

    for(int i = rect.y; i < rect.h + rect.y; i++){
        int dy = i - y;
        for(int j = rect.x; j < rect.w + rect.x; j++){
            int dx = j - x;
            if(dx*dx + dy*dy <= radius*radius)
                PNT_IMG_GET(img, j, i) = color;
        }
    }
}

// void draw_filled_circle_f(PNTFormatImage fimg, int x, int y, int radius, PNTColor color){
//     Rect rect = {
//         .x = x - radius,
//         .y = y - radius,
//         .w = radius*2,
//         .h = radius*2
//     };
    
//     rect = CorrectRect(rect, fimg.width, fimg.height);
//     int fcolor = get_formatted_color(color, fimg.format);
    
//     for(int i = rect.y; i < rect.h + rect.y; i++){
//         int dy = y - i;
//         for(int j = rect.x; j < rect.w + rect.x; j++){
//             int dx = x - j;
//             if(dx*dx + dy*dy <= radius*radius)
//             PNT_IMG_GET(fimg, j, i) = fcolor;
//         }
//     }
// }

void pnt_draw_filled_rect(PNTImage img, Rect rect, PNTColor color){
    rect = CorrectRect(rect, img.width, img.height);

    int endX = rect.x + rect.w;
    int endY = rect.y + rect.h;
    for(int i = rect.y; i < endY; i++)
    for(int j = rect.x; j < endX; j++){
        PNT_IMG_GET(img, j, i) = color;
        // printf("worked\n");
    }
    // printf("---------\n");

}

// void draw_filled_rect_f(PNTFormatImage fimg, Rect rect, PNTColor color){
//     rect = CorrectRect(rect, fimg.width, fimg.height);
//     int fcolor = get_formatted_color(color, fimg.format);

//     int endX = rect.x + rect.w;
//     int endY = rect.y + rect.h;
//     for(int i = rect.y; i < endY; i++)
//     for(int j = rect.x; j < endX; j++){
//         PNT_IMG_GET(fimg, j, i) = fcolor;
//     }
// }


void pnt_draw_triangle(PNTImage img, Triangle t, PNTColor color){
    pnt_draw_line(img, color, t.x0, t.y0, t.x1, t.y1);
    pnt_draw_line(img, color, t.x1, t.y1, t.x2, t.y2);
    pnt_draw_line(img, color, t.x0, t.y0, t.x2, t.y2);
}

void pnt_draw_mesh(PNTImage img, Triangle* triangles, int triangleCount, PNTColor color){
    for(int i = 0; i < triangleCount; i++){
        pnt_draw_triangle(img, triangles[i], color);
    }
}




// Line drawing
//##################################################################

void pnt_bresenham_horizontal(PNTImage img, PNTColor color, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    if(x0 > x1){
        int temp;
        SWAP(x0, x1, temp);
        SWAP(y0, y1, temp);
    }
    
    int D = dx;
    int move = -1;
    if(y0 < y1){
        move = 1;
    }
    int k = 2 * dy;
    int y = y0;
    
    for(int x = x0; x < x1; x++){
        D += k;
        if(D > 2 * dx){
            y += move;
            D -= 2 * dx;
        }
        PNT_IMG_GET(img, x, y) = color;
    }
}

// void bresenham_horizontal_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);
    
//     if(x0 > x1){
//         int temp;
//         SWAP(x0, x1, temp);
//         SWAP(y0, y1, temp);
//     }
    
//     int D = dx;
//     int move = -1;
//     if(y0 < y1){
//         move = 1;
//     }
//     int k = 2 * dy;
//     int y = y0;

//     int fcolor = get_formatted_color(color, fimg.format);
//     for(int x = x0; x < x1; x++){
//         D += k;
//         if(D > 2 * dx){
//             y += move;
//             D -= 2 * dx;
//         }
//         PNT_IMG_GET(fimg, x, y) = fcolor;
//     }
// }

void pnt_bresenham_vertical(PNTImage img, PNTColor color, int x0, int y0, int x1, int y1){
    if(y0 > y1){
        int temp;
        SWAP(x0, x1, temp);
        SWAP(y0, y1, temp);
    }
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    
    int D = dy;
    int move = -1;
    if(x0 < x1){
        move = 1;
    }
    int k = 2 * dx;
    int x = x0;
    
    for(int y = y0; y < y1; y++){
        D += k;
        if(D > 2 * dy){
            x += move;
            D -= 2 * dy;
        }
        PNT_IMG_GET(img, x, y) = color;
    }
}

// void bresenham_vertical_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     if(y0 > y1){
//         int temp;
//         SWAP(x0, x1, temp);
//         SWAP(y0, y1, temp);
//     }
    
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);
    
    
//     int D = dy;
//     int move = -1;
//     if(x0 < x1){
//         move = 1;
//     }
//     int k = 2 * dx;
//     int x = x0;
    
//     int fcolor = get_formatted_color(color, fimg.format);
//     for(int y = y0; y < y1; y++){
//         D += k;
//         if(D > 2 * dy){
//             x += move;
//             D -= 2 * dy;
//         }
//         PNT_IMG_GET(fimg, x, y) = fcolor;
//     }
// }

// Bresenham
void pnt_draw_line(PNTImage img, PNTColor color, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = pnt_correct_line_end(x0, y0, dx, dy, img.width, img.height);
    vec2 p1 = pnt_correct_line_end(x1, y1, dx, dy, img.width, img.height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        pnt_bresenham_horizontal(img, color, x0, y0, x1, y1);
    } else {
        pnt_bresenham_vertical(img, color, x0, y0, x1, y1);
    }
}

// void draw_line_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);

//     vec2 p0 = correct_line_end(x0, y0, dx, dy, fimg.width, fimg.height);
//     vec2 p1 = correct_line_end(x1, y1, dx, dy, fimg.width, fimg.height);
//     x0 = p0.x;
//     y0 = p0.y;
//     x1 = p1.x;
//     y1 = p1.y;

//     if(dx > dy){
//         bresenham_horizontal_f(fimg, color, x0, y0, x1, y1);
//     } else {
//         bresenham_vertical_f(fimg, color, x0, y0, x1, y1);
//     }
// }

//##################################################################


// Anti-aliased line drawing
//##################################################################

void pnt_draw_line_aa_horizontal(PNTImage img, PNTColor color, int x0, int y0, int x1, int y1){
    if(x0 > x1){
        int temp;
        SWAP(x0, x1, temp);
        SWAP(y0, y1, temp);
    }
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    
    float k = (float)dy / (float)dx;
    if(y0 < y1) k = -k;
    
    float y = (float)y0;
    
    for(int x = x0; x < x1; x++){
        y -= k;
        
        float ry = floorf(y);
        float dist = y - ry;
        // img.buffer[((int)y)*img.width + x] = color;
        // img.buffer[((int)y+1)*img.width + x] = color;
        // img.buffer[((int)y)*img.width + x].a = (uint8_t)(floor((1 - dist)*255.0f));
        // img.buffer[((int)y+1)*img.width + x].a = (uint8_t)(floor(dist*255.0f));
        PNT_IMG_GET(img, x, (int)y) = color;
        PNT_IMG_GET(img, x, (int)y+1) = color;
        PNT_IMG_GET(img, x, (int)y).a = (uint8_t)(floor((1 - dist)*255.0f));
        PNT_IMG_GET(img, x, (int)y+1).a = (uint8_t)(floor(dist*255.0f));
    }
}

void pnt_draw_line_aa_vertical(PNTImage img, PNTColor color, int x0, int y0, int x1, int y1){
    if(y0 > y1){
        int temp;
        SWAP(x0, x1, temp);
        SWAP(y0, y1, temp);
    }
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    
    float k = (float)dx / (float)dy;
    if(x0 < x1) k = -k;
    
    float x = (float)x0;
    
    for(int y = y0; y < y1; y++){
        x -= k;
        
        float rx = floorf(x);
        float dist = x - rx;
        // PNT_IMG_GET(img, (int)x, y) = color;
        // img.buffer[(y)*img.width + (int)x] = color;
        // img.buffer[(y+1)*img.width + (int)(x + 1)] = color;
        // img.buffer[(y)*img.width + (int)x].a = (uint8_t)(floor((1 - dist)*255.0f));
        // img.buffer[(y+1)*img.width + (int)(x + 1)].a = (uint8_t)(floor(dist*255.0f));
        PNT_IMG_GET(img, (int)x, y) = color;
        PNT_IMG_GET(img, (int)x+1, y+1) = color;
        PNT_IMG_GET(img, (int)x, y).a = (uint8_t)(floor((1 - dist)*255.0f));
        PNT_IMG_GET(img, (int)x+1, y+1).a = (uint8_t)(floor(dist*255.0f));
    }
}

// Xiaolin Wu
void pnt_draw_line_aa(PNTImage img, PNTColor c, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = pnt_correct_line_end(x0, y0, dx, dy, img.width, img.height);
    vec2 p1 = pnt_correct_line_end(x1, y1, dx, dy, img.width, img.height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        pnt_draw_line_aa_horizontal(img, c, x0, y0, x1, y1);
    } else {
        pnt_draw_line_aa_vertical(img, c, x0, y0, x1, y1);
    }
}

//##################################################################


#define POINT_RADIUS 3
void pnt_draw_line_from_points(PNTImage img, vec2* points, int count, PNTColor color, int drawPoints){
    if(points == NULL){
        printf("line array is null\n");
        return;
    }

    if(!drawPoints)
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            pnt_draw_line(img, color, start.x, start.y, end.x, end.y);
        }

    else
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            pnt_draw_line(img, color, start.x, start.y, end.x, end.y);
            pnt_draw_filled_circle(img, start.x, start.y, POINT_RADIUS, color);
        }
}

//##################################################################
// void bresenham_horizontal_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);
    
//     if(x0 > x1){
//         int temp;
//         SWAP(x0, x1, temp);
//         SWAP(y0, y1, temp);
//     }
    
//     int D = dx;
//     int move = -1;
//     if(y0 < y1){
//         move = 1;
//     }
//     int k = 2 * dy;
//     int y = y0;

//     int fcolor = get_formatted_color(color, fimg.format);
//     for(int x = x0; x < x1; x++){
//         D += k;
//         if(D > 2 * dx){
//             y += move;
//             D -= 2 * dx;
//         }
//         PNT_IMG_GET(fimg, x, y) = fcolor;
//     }
// }



// void bresenham_vertical_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     if(y0 > y1){
//         int temp;
//         SWAP(x0, x1, temp);
//         SWAP(y0, y1, temp);
//     }
    
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);
    
    
//     int D = dy;
//     int move = -1;
//     if(x0 < x1){
//         move = 1;
//     }
//     int k = 2 * dx;
//     int x = x0;
    
//     int fcolor = get_formatted_color(color, fimg.format);
//     for(int y = y0; y < y1; y++){
//         D += k;
//         if(D > 2 * dy){
//             x += move;
//             D -= 2 * dy;
//         }
//         PNT_IMG_GET(fimg, x, y) = fcolor;
//     }
// }


// void draw_line_f(PNTFormatImage fimg, PNTColor color, int x0, int y0, int x1, int y1){
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);

//     vec2 p0 = correct_line_end(x0, y0, dx, dy, fimg.width, fimg.height);
//     vec2 p1 = correct_line_end(x1, y1, dx, dy, fimg.width, fimg.height);
//     x0 = p0.x;
//     y0 = p0.y;
//     x1 = p1.x;
//     y1 = p1.y;

//     if(dx > dy){
//         bresenham_horizontal_f(fimg, color, x0, y0, x1, y1);
//     } else {
//         bresenham_vertical_f(fimg, color, x0, y0, x1, y1);
//     }
// }

//##################################################################



// PNTImage save, load
//##################################################################

int pnt_save_image_ppm(PNTImage* img, char* filename){

    char path[64];
    sprintf(path, "images/%s", filename);

    FILE* file = fopen(path, "wb");

    if(file == NULL){
        perror("File opening failed\n");
        return 1;
    }

    // File configs
    fprintf(file, "P6\n%d %d\n255\n", img->width, img->height);

    // Save image data
    for(int i = 0; i < img->height; i++)
    for(int j = 0; j < img->width; j++){
        uint8_t bytes[3] = {
            (PNT_IMGP_GET(img, j, i).b),
            (PNT_IMGP_GET(img, j, i).g),
            (PNT_IMGP_GET(img, j, i).r)
        };

        fwrite(bytes, sizeof(bytes), 1, file);
    }

    fclose(file);
    return 0;
}

int pnt_save_image_png(PNTImage* img, char* filename){
    char path[64];
    sprintf(path, "%s", filename);
    return stbi_write_png(path, img->width, img->height, 4, &img->buffer[0], 4*img->width);
}


/* 
This function creates image itself.
If you pass already created image it will be deleted.
NOTE: This will be changed in the future
*/
int pnt_load_png(PNTImage* img, const char* path){
    if(img->buffer != NULL){
        // free(img->buffer);
        pnt_delete_image(img);
    }

    int w, h, comp;
    unsigned char* imagedata = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);

    img->width = w;
    img->height = h;
    img->stride = w;
    img->buffer = (PNTColor*)imagedata;
    
    // If will be needed
    // create_image(img, w, h);
    // for(int i = 0; i < h; i++)
    // for(int j = 0; j < w; j++){
    //     int index = (i*w + j) * 4;
    //     PNT_IMGP_GET(img, j, i).r = imagedata[index];
    //     PNT_IMGP_GET(img, j, i).g = imagedata[index+1];
    //     PNT_IMGP_GET(img, j, i).b = imagedata[index+2];
    //     PNT_IMGP_GET(img, j, i).a = imagedata[index+3];
    // }
    // stbi_image_free(imagedata);
    return 0;
}

//##################################################################