#pragma once

#include "body.h"

#include "raylib.h"

typedef struct World {
    Vector2    size;
    BodyVector vec;
    Camera2D   camera;
} World;

extern World world;

void    add_random_body(void);
void    add_random_body_fn(Vector2(*positionFunc)(void));

Color   get_random_color(void);

Vector2 get_random_circlepos(void);
Vector2 get_random_worldpos(void);
