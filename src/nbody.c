#include "nbody.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdint.h>

World world;

Color get_random_color(void)
{
    return (Color) {
        .r = GetRandomValue(0, 255),
        .g = GetRandomValue(0, 255),
        .b = GetRandomValue(0, 255),
        .a = 255,
    };
}

Vector2 get_random_worldpos(void)
{
    return (Vector2) {
        .x = GetRandomValue(0, world.size.x),
        .y = GetRandomValue(0, world.size.y),
    };
}

Vector2 get_random_circlepos(void)
{
    float a = (float)rand();
    float b = (float)rand();
    float R = 0.02;

    if (b < a) {
        float tmp = a;
        a = b;
        b = tmp;
    }

    Vector2 v = (Vector2) {
        .x = b * R * cosf(2 * PI * a / b),
        .y = b * R * sinf(2 * PI * a / b),
    };

    v.x += world.size.x / 2;
    v.y += world.size.y / 2;

    return v;
}

Body get_random_body(void)
{
    return (Body) {
        .acceleration = { 0, 0 },
        .velocity     = { 0, 0 },
        .color    = get_random_color(),
        .mass     = GetRandomValue(1, 15),
        .position = { 0, 0 },
    };
}

void add_random_body_fn(Vector2(*positionFunc)(void))
{
    Body body = get_random_body();
    body.position = positionFunc();
    AddBody(&world.vec, body);
}

void add_random_body(void)
{
    Body body = get_random_body();
    body.position = get_random_circlepos();
    AddBody(&world.vec, body);
}

void add_random_body_at_mouse(void)
{
    Body body = get_random_body();
    body.position = GetScreenToWorld2D(GetMousePosition(), world.camera);
    AddBody(&world.vec, body);
}

void remove_last_body(void)
{
    if (world.vec.count > 1) {
        world.vec.count--;
    }
}
