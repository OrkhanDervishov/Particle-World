#include <stdio.h>
#include <time.h>
#include "engine/engine_api.h"
#include "perlin.h"

#define WINDOW_TITLE "Test window"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define PART_SIZE 10
#define CHUNK_SIZE 16
#define WORLD_SIZE 3
#define FIELD_WIDTH 1 + 2*WORLD_SIZE
#define FIELD_HEIGHT 1 + 2*WORLD_SIZE

#define FIELD_DRAW_WIDTH_OFFSET WINDOW_WIDTH/2 - PART_SIZE*CHUNK_SIZE/2 - (WORLD_SIZE)*PART_SIZE*CHUNK_SIZE/2
#define FIELD_DRAW_HEIGHT_OFFSET WINDOW_HEIGHT/2 - PART_SIZE*CHUNK_SIZE/2 - (WORLD_SIZE)*PART_SIZE*CHUNK_SIZE/2

#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 60

#define RAND_FLOAT (float)rand() / (float)RAND_MAX
#define RAND_DOUBLE (double)rand() / (double)RAND_MAX

#define CHUNK_SAVE_ENABLED FALSE

typedef struct{
    Color color;
} Part;

typedef struct{
    int chunk_x, chunk_y;
    Part parts[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

typedef struct{
    int field_x, field_y;
    int center_chunk_x, center_chunk_y;
    int rows, cols;
    Chunk chunks[FIELD_WIDTH][FIELD_HEIGHT];
} Field;


#define FIELD_GET_PART(f, x, y) (f).chunks[]


typedef struct{
    Color color;
    double x, y;
    int chunk_x, chunk_y;
    int camera_x, camera_y;
} Player;

int permutation[PERM_TOTAL];


int RandomGenerator(int x, int y){
    switch (rand() % 5)
    {
        case 0:
        return 0xFF777777;
        case 1:
        return 0xFF333333;
        case 2:
        return 0xFFBBBBBB;
        case 3:
        return 0xFFFFFFFF;
        case 4:
        return 0xFF000000;
    }    
}

int PerlinGenerator(int x, int y){
    double val = noise2D((double)(x) * 0.05, (double)(y) * 0.05, permutation) * 100.0;
    
    Color c;
    c.r = (int)(val) & 255;
    c.g = (int)(val) & 255;
    c.b = (int)(val) & 255;
    c.a = 255;
    return c.rgba;
}

int FunctionGenerator(int x, int y){
    int val = (int)(tan(x*x + y*y));
    Color c;
    c.r = val & 255;
    c.g = val & 255;
    c.b = val & 255;
    c.a = 255;
    return c.rgba;
}

// int (*MainGenerator)(int x, int y) = RandomGenerator;
// int (*MainGenerator)(int x, int y) = FunctionGenerator;
int (*MainGenerator)(int x, int y) = PerlinGenerator;

void CreateChunk(Chunk *chunk, int chunk_x, int chunk_y){
    for(int i = 0; i < CHUNK_SIZE; i++)
    for(int j = 0; j < CHUNK_SIZE; j++){
        chunk->parts[i][j].color.rgba = MainGenerator(chunk_x + j, chunk_y + i);
    }
    chunk->chunk_x = chunk_x;
    chunk->chunk_y = chunk_y;
}

void SaveChunkDataToFile(Chunk* chunk, FILE* chunkdata){
    for(int i = 0; i < CHUNK_SIZE; i++)
    for(int j = 0; j < CHUNK_SIZE; j++){
        fprintf(chunkdata, "%d ", chunk->parts[i][j].color.rgba);
    }
}

int SaveChunk(Chunk* chunk){
    char filepath[64];
    snprintf(filepath, sizeof(filepath), "./chunk_test/chunk_data/cnk%d_%d.dat", chunk->chunk_x, chunk->chunk_y);
    
    FILE* chunkdata = fopen(filepath, "w");

    if(chunkdata == NULL){
        CONSOLE("Error opening chunk file for saving chunk\n");
        return 1;
    }

    SaveChunkDataToFile(chunk, chunkdata);

    fclose(chunkdata);
    return 0;
}

void LoadChunkDataFromFile(Chunk* chunk, FILE* chunkdata){
    for(int i = 0; i < CHUNK_SIZE; i++)
    for(int j = 0; j < CHUNK_SIZE; j++){
        fscanf(chunkdata, "%d ", &chunk->parts[i][j].color.rgba);
    }
}

int LoadChunk(Chunk* chunk, int chunk_x, int chunk_y){
    char filepath[64];
    snprintf(filepath, sizeof(filepath), "./chunk_test/chunk_data/cnk%d_%d.dat", chunk_x, chunk_y);

    if(access(filepath, F_OK) == 0){
        // If file exists
        FILE* chunkdata = fopen(filepath, "r");
        if(chunkdata == NULL){
            CONSOLE("Error opening chunk file for loading chunk\n");
            return 1;
        }
        LoadChunkDataFromFile(chunk, chunkdata);
        chunk->chunk_x = chunk_x;
        chunk->chunk_y = chunk_y;
        fclose(chunkdata);
    } else {
        // If file doesn't exist
        CreateChunk(chunk, chunk_x, chunk_y);
        if(CHUNK_SAVE_ENABLED)
            SaveChunk(chunk);
    }

    return 0;
}

void LoadChunks(Field* field, int center_chunk_x, int center_chunk_y){
    for(int i = -WORLD_SIZE; i <= WORLD_SIZE; i++)
    for(int j = -WORLD_SIZE; j <= WORLD_SIZE; j++){
        Chunk chunk;
        LoadChunk(&chunk, center_chunk_x + j*CHUNK_SIZE, center_chunk_y + i*CHUNK_SIZE);
        field->chunks[i + WORLD_SIZE][j + WORLD_SIZE] = chunk;
    }
}


void CreateField(Field *field){
    for(int i = 0; i < FIELD_HEIGHT; i++)
    for(int j = 0; j < FIELD_WIDTH; j++){
      CreateChunk(&field->chunks[i][j], j * CHUNK_SIZE, i * CHUNK_SIZE);
    }
}


void DrawChunkInGivenCoords(Window *window, Chunk *chunk, int x, int y){
    int endx = x + CHUNK_SIZE;
    int endy = y + CHUNK_SIZE;
    for(int i = y; i < endy; i++)
    for(int j = x; j < endx; j++){
        Part p = chunk->parts[i - y][j - x];
        SDL_Rect r = {
            .x = j * PART_SIZE,
            .y = i * PART_SIZE,
            .w = PART_SIZE,
            .h = PART_SIZE
        };
        SDL_SetRenderDrawColor(
            window->renderer,
            p.color.r,
            p.color.g,
            p.color.b,
            p.color.a
        );
        SDL_RenderFillRect(window->renderer, &r);
    }
}

void DrawChunk(Window *window, Chunk *chunk, Player* player){
    int start_x = abs(chunk->chunk_x - player->chunk_x);
    int start_y = abs(chunk->chunk_y - player->chunk_y);
    int endx = abs(start_x + CHUNK_SIZE);
    int endy = abs(start_y + CHUNK_SIZE);
    for(int i = start_y; i < endy; i++)
    for(int j = start_x; j < endx; j++){
        Part p = chunk->parts[i - start_y][j - start_x];
        SDL_Rect r = {
            .x = j * PART_SIZE + FIELD_DRAW_WIDTH_OFFSET - ((int)player->x - player->chunk_x),
            .y = i * PART_SIZE + FIELD_DRAW_HEIGHT_OFFSET - ((int)player->y - player->chunk_y),
            .w = PART_SIZE,
            .h = PART_SIZE
        };
        SDL_SetRenderDrawColor(
            window->renderer,
            p.color.r,
            p.color.g,
            p.color.b,
            p.color.a
        );
        SDL_RenderFillRect(window->renderer, &r);
    }
}

void DrawField(Window *window, Field *field, Player* player){
    for(int i = 0; i < FIELD_HEIGHT; i++)
    for(int j = 0; j < FIELD_WIDTH; j++){
        Chunk chunk = field->chunks[i][j];
        DrawChunk(window, &chunk, player);
    }
}

void DrawFieldFromCenter(Window* window, Field* field){
    for(int i = 0; i < FIELD_HEIGHT; i++)
    for(int j = 0; j < FIELD_WIDTH; j++){
        Chunk chunk = field->chunks[i][j];
        //DrawChunk(window, &chunk);
    }
}

void DrawScene(Window* window, Field* field, Player* player){
    for(int i = 0; i < FIELD_HEIGHT; i++)
    for(int j = 0; j < FIELD_WIDTH; j++){
        // Chunk chunk = field->chunks[i][j];
        DrawChunk(window, &field->chunks[i][j], player);
    }    
}


void CreatePlayer(Player *p, int x, int y, int rgba){
    p->x = (double)x;
    p->y = (double)y;
    p->chunk_x = (int)p->x / PART_SIZE - ((int)p->x/PART_SIZE % (CHUNK_SIZE));
    p->chunk_y = (int)p->y / PART_SIZE - ((int)p->y/PART_SIZE % (CHUNK_SIZE));
    p->camera_x = WINDOW_WIDTH/2;
    p->camera_y = WINDOW_HEIGHT/2;
    p->color.rgba = rgba;
}

void DrawPlayer(Window *window, Player *p){
    SDL_SetRenderDrawColor(
        window->renderer,
        p->color.r,
        p->color.g,
        p->color.b,
        p->color.a
    );
    SDL_Rect r = {
        .x = p->camera_x - PLAYER_WIDTH/2,
        .y = p->camera_y - PLAYER_HEIGHT/2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT
    };
    SDL_RenderFillRect(window->renderer, &r);
}

void PlayerUpdate(Player* player){
    // int multx = player->x >= 0.0f ? 1 : -1;
    // int multy = player->y >= 0.0f ? 1 : -1;
    player->chunk_x = (int)player->x/PART_SIZE - (((int)player->x/PART_SIZE) % (CHUNK_SIZE));
    player->chunk_y = (int)player->y/PART_SIZE - (((int)player->y/PART_SIZE) % (CHUNK_SIZE));
    // printf("x:%d y:%d\n", (int)player->x, (int)player->y);
    // printf("cx:%d cy:%d\n", (int)player->chunk_x, (int)player->chunk_y);
}

#define DELAY 0

int main(int argc, char* argv[]){

    make_permutation(permutation);

    Window* window;
    CreateWindow(&window, WINDOW_WIDTH, WINDOW_HEIGHT, WIN_TITLE);
    srand(time(NULL));


    Chunk chunk;
    CreateChunk(&chunk, -10, -10);

    Field field;
    CreateField(&field);

    Player player;
    CreatePlayer(&player, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0xFFFF0000);


    SDL_Event e;
    int x = CHUNK_SIZE, y = CHUNK_SIZE, mov = 15;

    clock_t deltaTime = 0, sumTime = 0;
    clock_t start = 0, end = 1;
    int fps;

    while(window->isrunning){

        deltaTime = end - start;
        sumTime += deltaTime;

        if(sumTime > 500){
            fps = 1000 / (end - start);
            sumTime = 0;
            sprintf(window->title, "Particles: %d FPS: %d Overall: %d Delay: %d", 
                    0, 
                    fps, 
                    end - start,
                    DELAY
                );
            SDL_SetWindowTitle(window->window, window->title);
        }
        start = clock();

        vec2f movementVector = {.x = 0, .y = 0};
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                window->isrunning = FALSE;
            }
            if(e.key.keysym.sym == SDLK_ESCAPE){
                window->isrunning = FALSE;
            }
            if(e.key.keysym.sym == SDLK_w){
                movementVector.y -= 100.0f * (float)deltaTime / (float)1e+2;
            }
            if(e.key.keysym.sym == SDLK_s){
                movementVector.y += 100.0f * (float)deltaTime / (float)1e+2;
            }
            if(e.key.keysym.sym == SDLK_d){
                movementVector.x += 100.0f * (float)deltaTime / (float)1e+2;
            }
            if(e.key.keysym.sym == SDLK_a){
                movementVector.x -= 100.0f * (float)deltaTime / (float)1e+2;
            }
        }
        player.x += movementVector.x;
        player.y += movementVector.y;
        
        SDL_SetRenderDrawColor(window->renderer, 100, 100, 100, 255);
        SDL_RenderClear(window->renderer);
        
        
        int playerChunkChanged = FALSE;
        int player_chunk_x = player.chunk_x;
        int player_chunk_y = player.chunk_y;
        PlayerUpdate(&player);
        if(player_chunk_x != player.chunk_x || player_chunk_y != player.chunk_y){
            playerChunkChanged = TRUE;
            LoadChunks(&field, player.chunk_x, player.chunk_y);
        }
        
        
        // DrawChunk(window, &chunk, -10, -10);
        DrawField(window, &field, &player);
        DrawPlayer(window, &player);

        SDL_RenderPresent(window->renderer);

        SDL_Delay(DELAY);

        end = clock();
    }

    DestroyWindow(&window);
    return 0;
}