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

extern void AddBody(BodyVector* vec, Body p);
extern void InitBodyVector(BodyVector* vec, int capacity);
extern void UpdateBodies(BodyVector* vec);
