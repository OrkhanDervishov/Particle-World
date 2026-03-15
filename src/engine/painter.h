#ifndef PAINTER_H
#define PAINTER_H

#include "core.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// #define SWAP(a, b, t) t = a; a = b; b = t;
#define IMGP_GET(img, x, y) (img)->buffer[(y)*(img)->width + (x)]
#define IMG_GET(img, x, y) (img).buffer[(y)*(img).width + (x)]
// #define COLOR_ALPHA

#define DEFAULT_IMAGE_WIDTH 720
#define DEFAULT_IMAGE_HEIGHT 480


typedef union{
    struct{
        uint8_t r, g, b, a;
    };
    uint32_t rgba;
} ColorRGBA;

typedef ColorRGBA Color;

typedef struct{
    int r_mask;
    int g_mask;
    int b_mask;
    int a_mask;

    int r_shift;
    int g_shift;
    int b_shift;
    int a_shift;
} MyPixelFormat;

typedef struct{
    int width;
    int height;
    Color* buffer;
} Image;

typedef struct{
    int width;
    int height;
    int* buffer;
    MyPixelFormat format;
} FormatImage;

// typedef struct{
//     int width;
//     int height;
//     ColorFormat cformat;
//     union{
//         ColorBGRA* bgra;
//         ColorRGBA* rgba;
//     };
// } SuperImage;




// The most basic functions
#define  GET_FCOLOR(color, format)  (                                               \
                                        (uint32_t)(color).r << (format).r_shift |   \
                                        (uint32_t)(color).g << (format).g_shift |   \
                                        (uint32_t)(color).b << (format).b_shift |   \
                                        (uint32_t)(color).a << (format).a_shift     \
                                    )

#define  GET_COLOR(fcolor, format)  {                                        \
                                        .r = (fcolor) >> (format).r_shift,   \
                                        .g = (fcolor) >> (format).g_shift,   \
                                        .b = (fcolor) >> (format).b_shift,   \
                                        .a = (fcolor) >> (format).a_shift    \
                                    }
                                    
#define ALPHA_BLEND(color_up, color_down)   {                                                                                   \
                                            .r = ((color_down).r * (255-(color_up).a) + (color_up).r * (color_up).a) >> 8,      \
                                            .g = ((color_down).g * (255-(color_up).a) + (color_up).g * (color_up).a) >> 8,      \
                                            .b = ((color_down).b * (255-(color_up).a) + (color_up).b * (color_up).a) >> 8,      \
                                            .a = 255                                                                             \
                                            }

#define GET_RANDOM_COLOR    (Color){                \
                                .r = rand() & 255,  \
                                .g = rand() & 255,  \
                                .b = rand() & 255,  \
                                .a = 255            \
                            }

#define GET_NEGATIVE(color) (Color){                \
                                .r = 255 - color.r, \
                                .g = 255 - color.g, \
                                .b = 255 - color.b, \
                                .a = color.a,       \
                            }

static inline Color get_random_color(){
    return (Color){
        .r = rand() & 255,
        .g = rand() & 255,
        .b = rand() & 255,
        .a = 255
    };
}

static inline Color get_negative(Color color){
    return (Color){
        .r = 255 - color.r,
        .g = 255 - color.g,
        .b = 255 - color.b,
        .a = color.a,
    };
}

MyPixelFormat create_format(int r_mask, int g_mask, int b_mask, int a_mask);
int get_formatted_color(Color color, MyPixelFormat format);
Color get_unformatted_color(int fcolor, MyPixelFormat format);

// Formatted image fucntions
void create_fimage(
    FormatImage* fimg, size_t w, size_t h, 
    int r_mask, int g_mask, int b_mask, int a_mask
);
void delete_fimage(FormatImage* fimg);
void fill_f(FormatImage fimg, Color color);
void put_pixel_f(FormatImage fimg, int x, int y, Color color);
void draw_circle_f(FormatImage fimg, int cx, int cy, int radius, Color color);
void draw_rect_f(FormatImage fimg, Rect rect, Color color, int tickness);
void draw_filled_circle_f(FormatImage fimg, int x, int y, int radius, Color color);
void draw_filled_rect_f(FormatImage fimg, Rect rect, Color color);
void draw_line_f(FormatImage fimg, Color c, int x0, int y0, int x1, int y1);

// RGBA image functions
int create_image(Image* img, size_t w, size_t h);
void delete_image(Image* img);
void draw_image_on_image(Image dest, Image src, int x, int y);
void draw_image_on_image_scaled(Image dest, Image src, int x, int y, int scaleX, int scaleY);
void fill_image(Image img, Color color);
void put_pixel(Image img, int x, int y, Color color);
// TODO: Add fill option
void draw_circle(Image img, int cx, int cy, int radius, Color color);
void draw_rect(Image img, Rect rect, Color color, int tickness);
void draw_filled_circle(Image img, int x, int y, int radius, Color color);
void draw_filled_rect(Image img, Rect rect, Color color);
void draw_triangle(Image img, Triangle t, Color color);
void draw_mesh(Image img, Triangle* triangles, int triangleCount, Color color);
// Bresenham
void draw_line(Image img, Color c, int x0, int y0, int x1, int y1);
// Xiaolin Wu
void draw_line_aa(Image img, Color c, int x0, int y0, int x1, int y1);
#define POINT_RADIUS 3
void draw_line_from_points(Image img, vec2* points, int count, Color color, int drawPoints);




// Conversion functions
int image_to_fimage(Image img, FormatImage fimg);
int fimage_to_image(FormatImage fimg, Image img);


void draw_image_on_fimage(FormatImage dest, Image src, int x, int y);
void draw_image_on_fimage_scaled(FormatImage dest, Image src, int x, int y, int scaleX, int scaleY);


// Image formats create and load
int save_image_ppm(Image* img, char* filename);
int save_image_png(Image* img, char* filename);
int load_png(Image* img, char* path);

#endif