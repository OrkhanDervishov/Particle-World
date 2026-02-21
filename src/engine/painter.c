#include "painter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "geometry.h"


void CreateImage(Image* img, size_t w, size_t h){
    img->buffer = (Color*)malloc(w * h * sizeof(Color));
    img->width = w;
    img->height = h;
}

void DeleteImage(Image* img){
    free(img->buffer);
}




void FillImage(Image* img, Color color){
    for(int i = 0; i < img->height; i++)
    for(int j = 0; j < img->width; j++){
        img->buffer[i * img->width + j] = color;
    }
}

void PutPixel(Image* img, int x, int y, Color color){
    if(x >= img->width || x < 0 || y >= img->height || y < 0) return;
    IMG_GET(img, x, y) = color;
}

Color GetRandomColor(){
    Color c = {
        .r = rand() % 256,
        .g = rand() % 256,
        .b = rand() % 256,
        .a = 255
    };

    return c;
}


Rect CorrectRectImage(Image* img, Rect rect){
    // x, y correction
    if(rect.x < 0) rect.x = 0;
    else if(rect.x > img->width) rect.x = img->width;
    if(rect.y < 0) rect.y = 0;
    else if(rect.y > img->height) rect.y = img->height;

    // w, h correction
    if(rect.x + rect.w > img->width)
        rect.w = img->width - rect.x;
    if(rect.y + rect.h > img->height)
        rect.h = img->height - rect.y;

    return rect;
}

// TODO: Add fill option
void DrawCircle(Image* img, int x, int y, int radius, Color color){
    x = radius - 1;
    y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while(x >= y){
        int cx = x;
        int cy = y;
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);
        // PutPixel(img, x + rad, y + rad, color);

        if(err <= 0){
            y++;
            err += dy;
            dy += 2;  
        }
        if(err > 0){
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void DrawRect(Image* img, Rect rect, Color color){
    rect = CorrectRectImage(img, rect);

    for(int i = rect.y; i < rect.h; i++)
    for(int j = rect.x; j < rect.w; j++){
        IMG_GET(img, j, i) = color;
    }
}

void DrawFilledCircle(Image* img, int x, int y, int radius, Color color){
    Rect rect = {
        .x = x - radius,
        .y = y - radius,
        .w = radius*2,
        .h = radius*2
    };

    rect = CorrectRectImage(img, rect);

    for(int i = rect.y; i < rect.h + rect.y; i++)
    for(int j = rect.x; j < rect.w + rect.x; j++){
        int dx = j - x;
        int dy = i - y;
        if(dx*dx + dy*dy <= radius*radius)
            img->buffer[i * img->width + j] = color;
    }
}

void DrawFilledRect(Image* img, Rect rect, Color color){
    
    // rect = CorrectRectImage(img, rect);
    rect = CorrectRect(rect, img->width, img->height);

    for(int i = rect.y; i < rect.h + rect.y; i++)
    for(int j = rect.x; j < rect.w + rect.x; j++){
        img->buffer[i * img->width + j] = color;
    }
}

void DrawTriangle(Image* img, Triangle t, Color color){
    DrawLine(img, color, t.x0, t.y0, t.x1, t.y1);
    DrawLine(img, color, t.x1, t.y1, t.x2, t.y2);
    DrawLine(img, color, t.x0, t.y0, t.x2, t.y2);
}

void DrawMesh(Image* img, Triangle* triangles, int triangleCount, Color color){
    for(int i = 0; i < triangleCount; i++){
        DrawTriangle(img, triangles[i], color);
    }
}




// Correct line endpoints when they are out of frame size
vec2 CorrectLineEnd(int x, int y, int dx, int dy, int width, int height){
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


void BresenhamHorizontal(Image* img, Color c, int x0, int y0, int x1, int y1){
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
        img->buffer[y*img->width + x] = c;
    }
}

void BresenhamVertical(Image* img, Color c, int x0, int y0, int x1, int y1){
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
        img->buffer[y*img->width + x] = c;
    }
}

// Bresenham
void DrawLine(Image* img, Color c, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = CorrectLineEnd(x0, y0, dx, dy, img->width, img->height);
    vec2 p1 = CorrectLineEnd(x1, y1, dx, dy, img->width, img->height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        BresenhamHorizontal(img, c, x0, y0, x1, y1);
    } else {
        BresenhamVertical(img, c, x0, y0, x1, y1);
    }
}


void DrawLineAAHorizontal(Image* img, Color c, int x0, int y0, int x1, int y1){
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
        img->buffer[((int)y)*img->width + x] = c;
        img->buffer[((int)y+1)*img->width + x] = c;
        img->buffer[((int)y)*img->width + x].a = (uint8_t)(floor((1 - dist)*255.0f));
        img->buffer[((int)y+1)*img->width + x].a = (uint8_t)(floor(dist*255.0f));
    }
}

void DrawLineAAVertical(Image* img, Color c, int x0, int y0, int x1, int y1){
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
        img->buffer[(y)*img->width + (int)x] = c;
        img->buffer[(y+1)*img->width + (int)(x + 1)] = c;
        img->buffer[(y)*img->width + (int)x].a = (uint8_t)(floor((1 - dist)*255.0f));
        img->buffer[(y+1)*img->width + (int)(x + 1)].a = (uint8_t)(floor(dist*255.0f));
    }
}

// Xiaolin Wu
void DrawLineAntiAliased(Image* img, Color c, int x0, int y0, int x1, int y1){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    vec2 p0 = CorrectLineEnd(x0, y0, dx, dy, img->width, img->height);
    vec2 p1 = CorrectLineEnd(x1, y1, dx, dy, img->width, img->height);
    x0 = p0.x;
    y0 = p0.y;
    x1 = p1.x;
    y1 = p1.y;

    if(dx > dy){
        DrawLineAAHorizontal(img, c, x0, y0, x1, y1);
    } else {
        DrawLineAAVertical(img, c, x0, y0, x1, y1);
    }
}


#define POINT_RADIUS 3
void DrawLineFromPoints(Image* img, vec2* points, int count, Color color, int drawPoints){
    if(points == NULL){
        printf("line array is null\n");
        return;
    }

    if(!drawPoints)
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            DrawLine(img, color, start.x, start.y, end.x, end.y);
        }

    else
        for(int i = 0; i < count-1; i++){
            vec2 start = points[i];
            vec2 end = points[i+1];
            DrawLine(img, color, start.x, start.y, end.x, end.y);
            DrawCircle(img, start.x, start.y, POINT_RADIUS, color);
        }
}




int SaveImagePPM(Image* img, char* filename){

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

int SaveImagePNG(Image* img, char* filename){
    char path[64];
    sprintf(path, "%s", filename);
    return stbi_write_png(path, img->width, img->height, 4, &img->buffer[0], 4*img->width);
}


int LoadPNG(Image* img, char* filename){
    char path[64];
    sprintf(path, "images/%s", filename);

    int w, h, comp;
    unsigned char* imagedata = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);

    CreateImage(img, w, h);

    for(int i = 0; i < h; i++)
    for(int j = 0; j < w; j++){
        int index = (i*w + j) * 4;
        IMG_GET(img, j, i).r = imagedata[index];
        IMG_GET(img, j, i).g = imagedata[index+1];
        IMG_GET(img, j, i).b = imagedata[index+2];
        IMG_GET(img, j, i).a = imagedata[index+3];
    }

    stbi_image_free(imagedata);
    return 0;
}
