#pragma once

#include "body.h"

#include "raylib.h"

typedef struct World {
    Vector2    size;
    BodyVector vec;
    Camera2D   camera;
} World;

extern World world;

extern void    add_random_body(void);
extern void    add_random_body_fn(Vector2(*positionFunc)(void));
extern void    add_random_body_at_mouse(void);
extern void    remove_last_body(void);
extern Vector2 get_random_circlepos(void);
extern Vector2 get_random_worldpos(void);
