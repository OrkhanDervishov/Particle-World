#ifndef PAINTER_H_
#define PAINTER_H_

#include "core.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define DEFAULT_IMAGE_WIDTH 720
#define DEFAULT_IMAGE_HEIGHT 480

typedef uint32_t    pnt_color_t;
typedef uint8_t     pnt_color_channel_t;

typedef enum{
    PNT_CUSTOM = 0,
    PNT_RGBA
} PNTImageFormats;

/*
    rgba uses native-endian layout
    0xAABBGGRR - little endian
    0xRRGGBBAA - big endian
*/
typedef union{
    struct{
        pnt_color_channel_t r, g, b, a;
    };
    pnt_color_t rgba;
} PNTColor;

typedef struct {
    float r, g, b, a;
} PNTColorf;

typedef struct{
    int r_mask;
    int g_mask;
    int b_mask;
    int a_mask;

    int r_shift;
    int g_shift;
    int b_shift;
    int a_shift;
} PNTPixelFormat;


typedef struct{
    int width;
    int height;
    int stride;
    PNTColor* buffer;
} PNTImage;

typedef struct{
    PNTImageFormats format_type;
    PNTPixelFormat format;

    int width;
    int height;
    int stride;
    pnt_color_t* buffer;
} PNTFormatImage;

typedef struct{
    vec2f translation;
    float rotation;
    vec2f scale;
} PNTTranforms2d;


typedef PNTColor        Color;
typedef PNTColorf       Colorf;
typedef PNTImage        Image;
typedef PNTFormatImage  FormatImage;
typedef PNTTranforms2d  Transforms2d;


#define PNT_IMGP_GET(img, x, y) (img)->buffer[(y)*(img)->stride + (x)]
#define PNT_IMG_GET(img, x, y)  (img).buffer[(y)*(img).stride + (x)]

#define PNT_GET_PIXEL(img, x, y) IMG_GET(img, x, y)

#define PNT_PUT_PIXEL(img, x, y, color)   \
do{\
    if((x) >= 0 && (x) < (img).width && (y) >= 0 && (y) < (img).height){ \
        PNT_IMG_GET((img), (x), (y)) = (color); \
    }\
} while(0)

// The most basic functions
#define  PNT_GET_FCOLOR(color, format)  (                                               \
                                        (uint32_t)(color).r << (format).r_shift |   \
                                        (uint32_t)(color).g << (format).g_shift |   \
                                        (uint32_t)(color).b << (format).b_shift |   \
                                        (uint32_t)(color).a << (format).a_shift     \
                                    )

#define  PNT_GET_COLOR(fcolor, format)  {                                        \
                                        .r = (fcolor) >> (format).r_shift,   \
                                        .g = (fcolor) >> (format).g_shift,   \
                                        .b = (fcolor) >> (format).b_shift,   \
                                        .a = (fcolor) >> (format).a_shift    \
                                    }
                                    
#define PNT_ALPHA_BLEND(color_up, color_down)   {                                                                                \
                                            .r = ((color_down).r * (255-(color_up).a) + ((color_up).r * (color_up).a) >> 8), \
                                            .g = ((color_down).g * (255-(color_up).a) + ((color_up).g * (color_up).a) >> 8), \
                                            .b = ((color_down).b * (255-(color_up).a) + ((color_up).b * (color_up).a) >> 8), \
                                            .a = ((color_down).a * (255-(color_up).a) + ((color_up).a * (color_up).a) >> 8), \
                                        }

#define PNT_GET_RANDOM_COLOR    (PNTColor){                \
                                .r = rand() & 255,  \
                                .g = rand() & 255,  \
                                .b = rand() & 255,  \
                                .a = 255            \
                            }

#define PNT_GET_NEGATIVE(color) (PNTColor){                \
                                .r = 255 - color.r, \
                                .g = 255 - color.g, \
                                .b = 255 - color.b, \
                                .a = color.a,       \
                            }

extern PNTColorf    pnt_color_to_colorf(PNTColor color);
extern PNTColor     pnt_colorf_to_color(PNTColorf colorf);
extern PNTColorf    pnt_add_colorf(PNTColorf a, PNTColorf b, float scale);
extern PNTColor     pnt_add_color(PNTColor a, PNTColor b);
extern PNTColor     pnt_get_random_color();
extern PNTColor     pnt_get_negative(PNTColor color);
extern int          pnt_get_negative_f(int fcolor, PNTPixelFormat format);
extern PNTColor     pnt_get_color(PNTImage image, int x, int y);
extern PNTColor     pnt_colors_alpha_blend(PNTColor down, PNTColor up);

PNTPixelFormat  pnt_create_format(int r_mask, int g_mask, int b_mask, int a_mask);
pnt_color_t     pnt_get_formatted_color(PNTColor color, PNTPixelFormat format);
PNTColor        pnt_get_unformatted_color(int fcolor, PNTPixelFormat format);

// Formatted image fucntions
void pnt_create_fimage(
    PNTFormatImage* fimg, size_t w, size_t h, 
    int r_mask, int g_mask, int b_mask, int a_mask
);
void pnt_delete_fimage(PNTFormatImage* fimg);
void pnt_fill_f(PNTFormatImage fimg, PNTColor color);
void pnt_put_pixel_f(PNTFormatImage fimg, int x, int y, PNTColor color);
// void draw_circle_f(PNTFormatImage fimg, int cx, int cy, int radius, PNTColor color, int thickness);
// void draw_rect_f(PNTFormatImage fimg, Rect rect, PNTColor color, int tickness);
// void draw_filled_circle_f(PNTFormatImage fimg, int x, int y, int radius, PNTColor color);
// void draw_filled_rect_f(PNTFormatImage fimg, Rect rect, PNTColor color);
// void draw_line_f(PNTFormatImage fimg, PNTColor c, int x0, int y0, int x1, int y1);

// RGBA image functions
int  pnt_create_similar(PNTImage* dest, PNTImage src);
int  pnt_copy_image_content(PNTImage dest, PNTImage src);
int  pnt_copy_image(PNTImage* dest, PNTImage src);
int  pnt_create_image(PNTImage* img, size_t w, size_t h);
void pnt_delete_image(PNTImage* img);
void pnt_change_color(PNTImage img, PNTColor dest, PNTColor src);
void pnt_fill(PNTImage img, PNTColor color);
void pnt_put_pixel(PNTImage img, int x, int y, PNTColor color);
// TODO: Add fill option
void pnt_draw_circle(PNTImage img, int cx, int cy, int radius, PNTColor color, int thickness);
void pnt_draw_rect(PNTImage img, Rect rect, PNTColor color, int tickness);
void pnt_draw_filled_circle(PNTImage img, int x, int y, int radius, PNTColor color);
void pnt_draw_filled_rect(PNTImage img, Rect rect, PNTColor color);
void pnt_draw_triangle(PNTImage img, Triangle t, PNTColor color);
void pnt_draw_mesh(PNTImage img, Triangle* triangles, int triangleCount, PNTColor color);
// Bresenham
void pnt_draw_line(PNTImage img, PNTColor c, int x0, int y0, int x1, int y1);
// Xiaolin Wu
void pnt_draw_line_aa(PNTImage img, PNTColor c, int x0, int y0, int x1, int y1);
#define POINT_RADIUS 3
void pnt_draw_line_from_points(PNTImage img, vec2* points, int count, PNTColor color, int drawPoints);



// Conversion functions
int pnt_image_to_fimage(PNTImage img, PNTFormatImage fimg);
int pnt_fimage_to_image(PNTFormatImage fimg, PNTImage img);


void pnt_blit(PNTImage dest, PNTImage src, int x, int y);
void pnt_blit_scaled(PNTImage dest, PNTImage src, int x, int y, float scaleX, float scaleY);
void pnt_blit_transformed(PNTImage dest, PNTImage src, vec2f pos, float rotation, vec2f scale);
// void draw_image_on_fimage(PNTFormatImage dest, PNTImage src, int x, int y);
// void draw_image_on_fimage_scaled(PNTFormatImage dest, PNTImage src, int x, int y, int scaleX, int scaleY);


// PNTImage formats create and load
int pnt_save_image_ppm(PNTImage* img, char* filename);
int pnt_save_image_png(PNTImage* img, char* filename);
int pnt_load_png(PNTImage* img, const char* path);

#endif