#include "body.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>

void UpdateBodies(BodyVector* vec)
{
    // update attraction force on all bodies
    for (int i = 0; i < vec->count; i++) {
        for (int j = 0; j < vec->count; j++) {
            if (i == j) continue;

            Vector2 force = Attract(&vec->bodies[i], &vec->bodies[j]);
            ApplyForce(&vec->bodies[i], force);
        }
    }

    // update positions
    for (int i = 0; i < vec->count; i++) {
        vec->bodies[i].velocity = Vector2Add(vec->bodies[i].velocity, vec->bodies[i].acceleration);
        vec->bodies[i].position = Vector2Add(vec->bodies[i].position, vec->bodies[i].velocity);
        vec->bodies[i].acceleration.x = 0;
        vec->bodies[i].acceleration.y = 0;
    }
}

Vector2 Attract(Body* b1, Body* b2)
{
    Vector2 force = Vector2Subtract(b2->position, b1->position);
    float distance = Vector2Length(force);
    distance = Clamp(distance, 1, 25);
    force = Vector2Normalize(force);
    float strength = (GravityConstant * b2->mass * b1->mass) / (distance * distance);
    force.x = force.x * strength;
    force.y = force.y * strength;

    return force;
}

void ApplyForce(Body* b, Vector2 force)
{
    force.x /= b->mass * 3;
    force.y /= b->mass * 3;
    b->acceleration.x += force.x;
    b->acceleration.y += force.y;
}

void AddBody(BodyVector* vec, Body b)
{
    int index = vec->count++;
    if (index >= vec->capacity) {
        vec->capacity = vec->capacity * 2;
        vec->bodies = (Body*)realloc(vec->bodies, vec->capacity * sizeof(Body));
    }
    vec->bodies[index] = b;
}

void InitBodyVector(BodyVector* vec, int capacity)
{
    vec->capacity = capacity;
    vec->count = 0;
    vec->bodies = (Body*)calloc(capacity, sizeof(Body));
}
