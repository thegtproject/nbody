#pragma once

#include "raylib.h"
#include "raymath.h"

typedef struct Body {
    Vector2 position;
    Vector2 acceleration;
    Vector2 velocity;
    Color   color;
    float   mass;
} Body;

typedef struct BodyVector {
    int   capacity;
    int   count;
    Body* bodies;
} BodyVector;

static float GravityConstant = 0.4;

void    AddBody(BodyVector* vec, Body p);
void    InitBodyVector(BodyVector* vec, int capacity);
void    UpdateBodies(BodyVector* vec);
void    ApplyForce(Body* b, Vector2 force);
Vector2 Attract(Body* b1, Body* b2);
