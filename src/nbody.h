#pragma once

#include "body.h"

#include "raylib.h"

typedef struct World {
    Vector2    size;
    BodyVector vec;
    Camera2D   camera;
} World;

extern World world;

void    add_random_body();
void    add_random_body_fn(Vector2(*positionFunc)());

Color   get_random_color();

Vector2 get_random_circlepos();
Vector2 get_random_worldpos();
