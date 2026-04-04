#include "painter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "geometry.h"



// Basics
//##################################################################



MyPixelFormat create_format(int r_mask, int g_mask, int b_mask, int a_mask){
    MyPixelFormat format;
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

int get_formatted_color(Color color, MyPixelFormat format){
    return  GET_FCOLOR(color, format);
}

Color get_unformatted_color(int fcolor, MyPixelFormat format){
    Color color = GET_COLOR(fcolor, format);
    return color;
}

// Maybe will be moved to geometry.c
vec2 correct_line_end(int x, int y, int dx, int dy, int width, int height){
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

int create_image(Image* img, size_t w, size_t h){
    if(img->buffer != NULL){
        printf("buffer is not null\n");
        return 1;
    }
    img->buffer = (Color*)malloc(w * h * sizeof(Color));
    img->width = w;
    img->height = h;
    return 0;
}

void delete_image(Image* img){
    free(img->buffer);
}


void create_fimage(
    FormatImage* fimg, size_t w, size_t h, 
    int r_mask, int g_mask, int b_mask, int a_mask
){
    if(fimg->buffer != NULL) delete_fimage(fimg);
    fimg->buffer = (int*)malloc(w * h * sizeof(int));
    fimg->width = w;
    fimg->height = h;
    fimg->format = create_format(r_mask, g_mask, b_mask, a_mask);
}

void delete_fimage(FormatImage* fimg){
    free(fimg->buffer);
}

//##################################################################

// Conversion
//##################################################################

int image_to_fimage(Image img, FormatImage fimg){
    if(fimg.buffer == NULL && img.buffer == NULL) return 1;
    if(fimg.width != img.width || fimg.height != img.height) return 2;

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        Color color = img.buffer[i * img.width + j];
        int fcolor = (
            (uint32_t)color.r << fimg.format.r_shift |
            (uint32_t)color.g << fimg.format.g_shift |
            (uint32_t)color.b << fimg.format.b_shift |
            (uint32_t)color.a << fimg.format.a_shift
        );
        fimg.buffer[i * fimg.width + j] = fcolor;
    }

    return 0;
}

int fimage_to_image(FormatImage fimg, Image img){
    if(fimg.buffer == NULL && img.buffer == NULL) return 1;
    if(fimg.width != img.width || fimg.height != img.height) return 2;

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        int fcolor = fimg.buffer[i * fimg.width + j];
        img.buffer[i * img.width + j] = (Color)GET_COLOR(fcolor, fimg.format);
    }

    return 0;
}

//##################################################################

// Drawing on one another
//##################################################################

void draw_image_on_image(Image dest, Image src, int x, int y){
    Rect rect = {.x = x, .y = y, .w = src.width, .h = src.height};
    rect = CorrectRect(rect, dest.width, dest.height);

    for(int i = rect.y, k = 0; i < rect.h; i++, k++)
    for(int j = rect.x, t = 0; j < rect.w; j++, t++){
        dest.buffer[i*dest.width + j] = src.buffer[k*src.width + t];
    }
}

void draw_image_on_image_scaled(Image dest, Image src, int x, int y, int scaleX, int scaleY){
    Rect rect = {.x = x, .y = y, .w = src.width*scaleX, .h = src.height*scaleY};
    rect = CorrectRect(rect, dest.width, dest.height);

    float addX = 1.0f/(float)scaleX;
    float addY = 1.0f/(float)scaleY;
    float k = 0.0f, t = 0.0f;
    for(int i = rect.y; i < rect.h; i++)
    for(int j = rect.x; j < rect.w; j++){
        k += addY;
        t += addX;
        dest.buffer[i*dest.width + j] = src.buffer[(int)k*src.width + (int)t];
    }
}

void draw_image_on_fimage(FormatImage dest, Image src, int x, int y){
    Rect rect = {.x = x, .y = y, .w = src.width, .h = src.height};
    rect = CorrectRect(rect, dest.width, dest.height);

    for(int i = rect.y, k = 0; i < rect.h; i++, k++)
    for(int j = rect.x, t = 0; j < rect.w; j++, t++){
        IMG_GET(dest, j, i) = get_formatted_color(IMG_GET(src, t, k), dest.format);
    }
}

void draw_image_on_fimage_scaled(FormatImage dest, Image src, int x, int y, int scaleX, int scaleY){
    Rect rect = {.x = x, .y = y, .w = src.width*scaleX, .h = src.height*scaleY};
    rect = CorrectRect(rect, dest.width, dest.height);

    float addX = 1.0f/(float)scaleX;
    float addY = 1.0f/(float)scaleY;
    float k = 0.0f, t = 0.0f;
    int prev_k = -1, prev_t = -1;
    int endX = rect.x + rect.w;
    int endY = rect.y + rect.h;
    int res_fcolor;
    for(int i = rect.y; i < endY; i++, k += addY){
        // k += addY;
        for(int j = rect.x; j < endX; j++, t += addX){
            if((int)k != prev_k || (int)t != prev_t){
                Color dest_color = GET_COLOR(IMG_GET(dest, j, i), dest.format);
                Color src_color = IMG_GET(src, (int)t, (int)k);
                res_fcolor = GET_FCOLOR((Color)ALPHA_BLEND(src_color, dest_color), dest.format);
                prev_k = (int)k;
                prev_t = (int)t;
            }
            IMG_GET(dest, j, i) = res_fcolor;
        }
        t = 0.0f;
    }
}

//##################################################################



// Main fucntions
//##################################################################

void fill_image(Image img, Color color){
    for(int i = 0; i < img.height; i++)
    for(int j = 0; j < img.width; j++){
        IMG_GET(img, j, i) = color;
    }
}

void fill_f(FormatImage fimg, Color color){
    int fcolor = get_formatted_color(color, fimg.format);

    for(int i = 0; i < fimg.height; i++)
    for(int j = 0; j < fimg.width; j++){
        IMG_GET(fimg, j, i) = fcolor;
    }
}

void put_pixel(Image img, int x, int y, Color color){
    if(x >= img.width || x < 0 || y >= img.height || y < 0) return;
    IMG_GET(img, x, y) = color;
}

void put_pixel_f(FormatImage fimg, int x, int y, Color color){
    if(x >= fimg.width || x < 0 || y >= fimg.height || y < 0) return;
    int fcolor = get_formatted_color(color, fimg.format);
    IMG_GET(fimg, x, y) = fcolor;
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
        PUT_PIXEL(img, x+dx, y+dy, color);\
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
        int fcolor = get_negative_f(IMG_GET(img, x+dx, y+dy), format);\
        PUT_PIXEL(img, x+dx, y+dy, fcolor);\
    }\
}while(0)


// TODO: Add fill option
void draw_circle(Image img, int cx, int cy, int radius, Color color, int thickness){
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


void draw_circle_f(FormatImage fimg, int cx, int cy, int radius, Color color, int thickness){
    
    // for(int i = 0; i < thickness; i++){
        int x = 0;
        int y = -radius;
        int p = -radius;
        int fcolor = GET_FCOLOR(color, fimg.format);

        while(x < -y){
            if(p > 0){
                y++;
                p += 2*(x + y) + 1;
            } else {
                p += 2*x + 1;
            }

            PUT_THICKNESS(fimg, cx + x, cy + y, cx, cy, fcolor, thickness, DOWN);
            PUT_THICKNESS(fimg, cx - x, cy + y, cx, cy, fcolor, thickness, DOWN);
            PUT_THICKNESS(fimg, cx + x, cy - y, cx, cy, fcolor, thickness, UP);
            PUT_THICKNESS(fimg, cx - x, cy - y, cx, cy, fcolor, thickness, UP);
            PUT_THICKNESS(fimg, cx + y, cy + x, cx, cy, fcolor, thickness, RIGHT);
            PUT_THICKNESS(fimg, cx + y, cy - x, cx, cy, fcolor, thickness, RIGHT);
            PUT_THICKNESS(fimg, cx - y, cy + x, cx, cy, fcolor, thickness, LEFT);
            PUT_THICKNESS(fimg, cx - y, cy - x, cx, cy, fcolor, thickness, LEFT);
            
            // Negative circle creation trials

            // PUT_PIXEL(fimg, cx + x, cy + y, get_negative_f(IMG_GET(fimg, cx + x, cy + y), fimg.format));
            // PUT_PIXEL(fimg, cx - x, cy + y, get_negative_f(IMG_GET(fimg, cx - x, cy + y), fimg.format));
            // PUT_PIXEL(fimg, cx + x, cy - y, get_negative_f(IMG_GET(fimg, cx + x, cy - y), fimg.format));
            // PUT_PIXEL(fimg, cx - x, cy - y, get_negative_f(IMG_GET(fimg, cx - x, cy - y), fimg.format));
            // PUT_PIXEL(fimg, cx + y, cy + x, get_negative_f(IMG_GET(fimg, cx + y, cy + x), fimg.format));
            // PUT_PIXEL(fimg, cx + y, cy - x, get_negative_f(IMG_GET(fimg, cx + y, cy - x), fimg.format));
            // PUT_PIXEL(fimg, cx - y, cy + x, get_negative_f(IMG_GET(fimg, cx - y, cy + x), fimg.format));
            // PUT_PIXEL(fimg, cx - y, cy - x, get_negative_f(IMG_GET(fimg, cx - y, cy - x), fimg.format));
            
            // PUT_THICKNESS_NC(fimg, cx + x, cy + y, cx, cy, fimg.format, thickness, DOWN);
            // PUT_THICKNESS_NC(fimg, cx - x, cy + y, cx, cy, fimg.format, thickness, DOWN);
            // PUT_THICKNESS_NC(fimg, cx + x, cy - y, cx, cy, fimg.format, thickness, UP);
            // PUT_THICKNESS_NC(fimg, cx - x, cy - y, cx, cy, fimg.format, thickness, UP);
            // PUT_THICKNESS_NC(fimg, cx + y, cy + x, cx, cy, fimg.format, thickness, RIGHT);
            // PUT_THICKNESS_NC(fimg, cx + y, cy - x, cx, cy, fimg.format, thickness, RIGHT);
            // PUT_THICKNESS_NC(fimg, cx - y, cy + x, cx, cy, fimg.format, thickness, LEFT);
            // PUT_THICKNESS_NC(fimg, cx - y, cy - x, cx, cy, fimg.format, thickness, LEFT);
            
            x++;
        }
    // }
}
#undef PUT_THICKNESS
#undef PUT_THICKNESS_NC


void draw_rect(Image img, Rect rect, Color color, int tickness){
    rect = CorrectRect(rect, img.width, img.height);

    Rect leftSide =     {.x = rect.x, .y = rect.y, .w = tickness, .h = rect.h};
    Rect rightSide =    {.x = rect.x + rect.w, .y = rect.y, .w = tickness, .h = rect.h};
    Rect upSide =       {.x = rect.x, .y = rect.y, .w = rect.w, .h = tickness};
    Rect bottomSide =   {.x = rect.x, .y = rect.y + rect.h, .w = rect.w, .h = tickness};

    draw_filled_rect(img, leftSide, color);
    draw_filled_rect(img, rightSide, color);
    draw_filled_rect(img, upSide, color);
    draw_filled_rect(img, bottomSide, color);
}

void draw_rect_f(FormatImage fimg, Rect rect, Color color, int tickness){
    rect = CorrectRect(rect, fimg.width, fimg.height);
    int fcolor = get_formatted_color(color, fimg.format);

    Rect leftSide =     {.x = rect.x, .y = rect.y, .w = tickness, .h = rect.h};
    Rect rightSide =    {.x = rect.x + rect.w - tickness, .y = rect.y, .w = tickness, .h = rect.h};
    Rect upSide =       {.x = rect.x, .y = rect.y, .w = rect.w, .h = tickness};
    Rect bottomSide =   {.x = rect.x, .y = rect.y + rect.h - tickness, .w = rect.w, .h = tickness};

    draw_filled_rect_f(fimg, leftSide, color);
    draw_filled_rect_f(fimg, rightSide, color);
    draw_filled_rect_f(fimg, upSide, color);
    draw_filled_rect_f(fimg, bottomSide, color);
}

void draw_filled_circle(Image img, int x, int y, int radius, Color color){
    Rect rect = {
        .x = x - radius,
        .y = y - radius,
        .w = radius*2,
        .h = radius*2
    };

    rect = CorrectRect(rect, img.width, img.height);

    for(int i = rect.y; i < rect.h + rect.y; i++)
    for(int j = rect.x; j < rect.w + rect.x; j++){
        int dx = j - x;
        int dy = i - y;
        if(dx*dx + dy*dy <= radius*radius)
            IMG_GET(img, dx, dy) = color;
    }
}

void draw_filled_circle_f(FormatImage fimg, int x, int y, int radius, Color color){
    Rect rect = {
        .x = x - radius,
        .y = y - radius,
        .w = radius*2,
        .h = radius*2
    };

    rect = CorrectRect(rect, fimg.width, fimg.height);
    int fcolor = get_formatted_color(color, fimg.format);

    for(int i = rect.y; i < rect.h + rect.y; i++)
    for(int j = rect.x; j < rect.w + rect.x; j++){
        int dx = j - x;
        int dy = i - y;
        if(dx*dx + dy*dy <= radius*radius)
            IMG_GET(fimg, dx, dy) = fcolor;
    }
}

void draw_filled_rect(Image img, Rect rect, Color color){
    rect = CorrectRect(rect, img.width, img.height);

    int endX = rect.x + rect.w;
    int endY = rect.y + rect.h;
    for(int i = rect.y; i < endY; i++)
    for(int j = rect.x; j < endX; j++){
        IMG_GET(img, j, i) = color;
    }
}

void draw_filled_rect_f(FormatImage fimg, Rect rect, Color color){
    rect = CorrectRect(rect, fimg.width, fimg.height);
    int fcolor = get_formatted_color(color, fimg.format);

    int endX = rect.x + rect.w;
    int endY = rect.y + rect.h;
    for(int i = rect.y; i < endY; i++)
    for(int j = rect.x; j < endX; j++){
        IMG_GET(fimg, j, i) = fcolor;
    }
}


void draw_triangle(Image img, Triangle t, Color color){
    draw_line(img, color, t.x0, t.y0, t.x1, t.y1);
    draw_line(img, color, t.x1, t.y1, t.x2, t.y2);
    draw_line(img, color, t.x0, t.y0, t.x2, t.y2);
}

void draw_mesh(Image img, Triangle* triangles, int triangleCount, Color color){
    for(int i = 0; i < triangleCount; i++){
        draw_triangle(img, triangles[i], color);
    }
}




// Line drawing
//##################################################################

void bresenham_horizontal(Image img, Color color, int x0, int y0, int x1, int y1){
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
        IMG_GET(img, x, y) = color;
    }
}

void bresenham_horizontal_f(FormatImage fimg, Color color, int x0, int y0, int x1, int y1){
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

    int fcolor = get_formatted_color(color, fimg.format);
    for(int x = x0; x < x1; x++){
        D += k;
        if(D > 2 * dx){
            y += move;
            D -= 2 * dx;
        }
        IMG_GET(fimg, x, y) = fcolor;
    }
}

void bresenham_vertical(Image img, Color color, int x0, int y0, int x1, int y1){
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
        IMG_GET(img, x, y) = color;
    }
}

void bresenham_vertical_f(FormatImage fimg, Color color, int x0, int y0, int x1, int y1){
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
    
    int fcolor = get_formatted_color(color, fimg.format);
    for(int y = y0; y < y1; y++){
        D += k;
        if(D > 2 * dy){
            x += move;
            D -= 2 * dy;
        }
        IMG_GET(fimg, x, y) = fcolor;
    }
}

// Bresenham
void draw_line(Image img, Color color, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = correct_line_end(x0, y0, dx, dy, img.width, img.height);
    vec2 p1 = correct_line_end(x1, y1, dx, dy, img.width, img.height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        bresenham_horizontal(img, color, x0, y0, x1, y1);
    } else {
        bresenham_vertical(img, color, x0, y0, x1, y1);
    }
}

void draw_line_f(FormatImage fimg, Color color, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = correct_line_end(x0, y0, dx, dy, fimg.width, fimg.height);
    vec2 p1 = correct_line_end(x1, y1, dx, dy, fimg.width, fimg.height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        bresenham_horizontal_f(fimg, color, x0, y0, x1, y1);
    } else {
        bresenham_vertical_f(fimg, color, x0, y0, x1, y1);
    }
}

//##################################################################


// Anti-aliased line drawing
//##################################################################

void draw_line_aa_horizontal(Image img, Color color, int x0, int y0, int x1, int y1){
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
        img.buffer[((int)y)*img.width + x] = color;
        img.buffer[((int)y+1)*img.width + x] = color;
        img.buffer[((int)y)*img.width + x].a = (uint8_t)(floor((1 - dist)*255.0f));
        img.buffer[((int)y+1)*img.width + x].a = (uint8_t)(floor(dist*255.0f));
    }
}

void draw_line_aa_vertical(Image img, Color color, int x0, int y0, int x1, int y1){
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
        // IMG_GET(img, (int)x, y) = color;
        img.buffer[(y)*img.width + (int)x] = color;
        img.buffer[(y+1)*img.width + (int)(x + 1)] = color;
        img.buffer[(y)*img.width + (int)x].a = (uint8_t)(floor((1 - dist)*255.0f));
        img.buffer[(y+1)*img.width + (int)(x + 1)].a = (uint8_t)(floor(dist*255.0f));
    }
}

// Xiaolin Wu
void draw_line_aa(Image img, Color c, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = correct_line_end(x0, y0, dx, dy, img.width, img.height);
    vec2 p1 = correct_line_end(x1, y1, dx, dy, img.width, img.height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        draw_line_aa_horizontal(img, c, x0, y0, x1, y1);
    } else {
        draw_line_aa_vertical(img, c, x0, y0, x1, y1);
    }
}

//##################################################################


#define POINT_RADIUS 3
void draw_line_from_points(Image img, vec2* points, int count, Color color, int drawPoints){
    if(points == NULL){
        printf("line array is null\n");
        return;
    }

    if(!drawPoints)
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            draw_line(img, color, start.x, start.y, end.x, end.y);
        }

    else
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            draw_line(img, color, start.x, start.y, end.x, end.y);
            draw_filled_circle(img, start.x, start.y, POINT_RADIUS, color);
        }
}


// Image save, load
//##################################################################

int save_image_ppm(Image* img, char* filename){

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
            (img->buffer[i * img->width + j].b),
            (img->buffer[i * img->width + j].g),
            (img->buffer[i * img->width + j].r)
        };

        fwrite(bytes, sizeof(bytes), 1, file);
    }

    fclose(file);
    return 0;
}

int save_image_png(Image* img, char* filename){
    char path[64];
    sprintf(path, "%s", filename);
    return stbi_write_png(path, img->width, img->height, 4, &img->buffer[0], 4*img->width);
}


/* 
This function creates image itself.
If you pass already created image it will be deleted.
NOTE: This will be changed in the future
*/
int load_png(Image* img, char* path){

    int w, h, comp;
    unsigned char* imagedata = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);

    if(img->buffer != NULL){
        free(img->buffer);
    }
    img->width = w;
    img->height = h;
    img->buffer = (Color*)imagedata;
    
    // If will be needed
    // create_image(img, w, h);
    // for(int i = 0; i < h; i++)
    // for(int j = 0; j < w; j++){
    //     int index = (i*w + j) * 4;
    //     IMGP_GET(img, j, i).r = imagedata[index];
    //     IMGP_GET(img, j, i).g = imagedata[index+1];
    //     IMGP_GET(img, j, i).b = imagedata[index+2];
    //     IMGP_GET(img, j, i).a = imagedata[index+3];
    // }
    // stbi_image_free(imagedata);
    return 0;
}

//##################################################################