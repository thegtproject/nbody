#define GRAPHICS_API_OPENGL_33

#include "body.h"
#include "nbody.h"

#include "raylib.h"
#include "raymath.h"

#include <stdio.h>

static const int initialBodyCount = 5000;

static const int screenWidth  = 1200;
static const int screenHeight = 650;

static Color clear_color = { 25, 25, 25, 255 };
static bool  paused      = true;
static bool  showhelp    = true;

static void update(void);
static void text(void);

static void draw(void)
{
    BeginDrawing();
    BeginMode2D(world.camera);
    ClearBackground(clear_color);

    for (int i = 0; i < world.vec.count; i++) {
        DrawCircleV(world.vec.bodies[i].position, world.vec.bodies[i].mass, world.vec.bodies[i].color);
    }

    EndMode2D();
    if (showhelp) text();
    EndDrawing();
}

static void setup(void)
{
    world.size.x = 1000;
    world.size.y = 1000;

    world.camera.rotation = 0.0f;
    world.camera.zoom = 1.0f;
    world.camera.target.x = world.size.x / 2.0f;
    world.camera.target.y = world.size.y / 2.0f;
    world.camera.offset.x = screenWidth / 2.0f;
    world.camera.offset.y = screenHeight / 2.0f;

    InitBodyVector(&world.vec, initialBodyCount);

    for (int i = 0; i < initialBodyCount; i++) {
        add_random_body_fn(get_random_worldpos);
    }

    setup_threads();
}

static void randomize_state(void)
{
    world.vec.count = 0;

    int bodyCount = 0;

    switch (GetRandomValue(1, 5)) {
        case 1: bodyCount = 5; break;
        case 2: bodyCount = 25; break;
        case 3: bodyCount = 125; break;
        case 4: bodyCount = 300; break;
        case 5: bodyCount = GetRandomValue(300, 1000); break;
    }

    Vector2 (*spawnPositionFn)(void) = NULL;

    switch (GetRandomValue(1, 2)) {
        case 1: spawnPositionFn = get_random_worldpos; break;
        case 2: spawnPositionFn = get_random_circlepos; break;
    }

    for (int i = 0; i < bodyCount; i++) {
        add_random_body_fn(spawnPositionFn);
    }

    adjust_tasks();
}

static void input(void)
{
    if (IsKeyDown(KEY_ONE)) remove_last_body();
    if (IsKeyDown(KEY_TWO)) add_random_body();
    if (IsKeyPressed(KEY_R)) randomize_state();

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) add_random_body_at_mouse();

    if (IsKeyDown(KEY_A)) world.camera.target.x -= 35.0;
    if (IsKeyDown(KEY_D)) world.camera.target.x += 35.0;
    if (IsKeyDown(KEY_W)) world.camera.target.y -= 35.0;
    if (IsKeyDown(KEY_S)) world.camera.target.y += 35.0;

    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    if (IsKeyPressed(KEY_H)) showhelp = !showhelp;

    if (IsKeyDown(KEY_Q)) world.vec.bodies[0].mass = Clamp(world.vec.bodies[0].mass - 3, 1, 300);
    if (IsKeyDown(KEY_E)) world.vec.bodies[0].mass = Clamp(world.vec.bodies[0].mass + 3, 1, 300);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        world.vec.bodies[0].position = GetScreenToWorld2D(GetMousePosition(), world.camera);
        world.vec.bodies[0].acceleration = (Vector2) { 0, 0 };
        world.vec.bodies[0].velocity = (Vector2) { 0, 0 };
    }

    world.camera.zoom += GetMouseWheelMove() * 0.05;
    world.camera.zoom = Clamp(world.camera.zoom, 0.05, 5.0);
}

int main(int argc, char const *argv[])
{
    setup();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "nbody sim");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        input();
        update();
        draw();
    }
    CloseWindow();
    return 0;
}

void text(void)
{
    DrawText(TextFormat("FPS: %i, Count: %i, Capacity: %i", GetFPS(), world.vec.count, world.vec.capacity), 4, 0, 16, WHITE);
    DrawText("Space - pause, H - hide help, R - randomize", 4, 16, 16, WHITE);
    DrawText("W,A,S,D - move, Wheel - zoom", 4, 32, 16, WHITE);
    DrawText("LMB - move first body under mouse", 4, 48, 16, WHITE);
    DrawText("Q/E - modify first body's mass", 4, 64, 16, WHITE);
    DrawText("RMB - add random body at mouse", 4, 80, 16, WHITE);
    DrawText("1 - remove last body, 2 - add random body", 4, 96, 16, WHITE);
}

void update(void)
{
    if (!paused) {
        UpdateBodies(&world.vec);
    }
}
