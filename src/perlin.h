#ifndef PERLIN_H
#define PERLIN_H

#define PERM_SIZE 256
#define PERM_TOTAL 512

/* ---------------- Vector2 ---------------- */

typedef struct {
    double x;
    double y;
} Vector2;

double dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

/* ---------------- Utilities ---------------- */

void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int index = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[index];
        array[index] = temp;
    }
}

void make_permutation(int *permutation) {
    for (int i = 0; i < PERM_SIZE; i++) {
        permutation[i] = i;
    }

    shuffle(permutation, PERM_SIZE);

    /* Duplicate the permutation */
    for (int i = 0; i < PERM_SIZE; i++) {
        permutation[i + PERM_SIZE] = permutation[i];
    }
}

/* ---------------- Perlin helpers ---------------- */

Vector2 get_constant_vector(int v) {
    int h = v & 3;
    switch (h) {
        case 0: return (Vector2){ 1.0,  1.0};
        case 1: return (Vector2){-1.0,  1.0};
        case 2: return (Vector2){-1.0, -1.0};
        default:return (Vector2){ 1.0, -1.0};
    }
}

double fade(double t) {
    return ((6*t - 15)*t + 10)*t*t*t;
}

double lerp(double t, double a1, double a2) {
    return a1 + t * (a2 - a1);
}

/* ---------------- Noise ---------------- */

double noise2D(double x, double y, int *permutation) {
    int X = ((int)floor(x)) & 255;
    int Y = ((int)floor(y)) & 255;

    double xf = x - floor(x);
    double yf = y - floor(y);

    Vector2 topRight    = { xf - 1.0, yf - 1.0 };
    Vector2 topLeft     = { xf,       yf - 1.0 };
    Vector2 bottomRight = { xf - 1.0, yf       };
    Vector2 bottomLeft  = { xf,       yf       };

    int valueTopRight    = permutation[permutation[X + 1] + Y + 1];
    int valueTopLeft     = permutation[permutation[X]     + Y + 1];
    int valueBottomRight = permutation[permutation[X + 1] + Y];
    int valueBottomLeft  = permutation[permutation[X]     + Y];

    double dotTopRight =
        dot(topRight, get_constant_vector(valueTopRight));
    double dotTopLeft =
        dot(topLeft, get_constant_vector(valueTopLeft));
    double dotBottomRight =
        dot(bottomRight, get_constant_vector(valueBottomRight));
    double dotBottomLeft =
        dot(bottomLeft, get_constant_vector(valueBottomLeft));

    double u = fade(xf);
    double v = fade(yf);

    return lerp(u,
        lerp(v, dotBottomLeft, dotTopLeft),
        lerp(v, dotBottomRight, dotTopRight)
    );
}


#endif