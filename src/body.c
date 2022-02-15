#include "body.h"
#include "nbody.h"

#include "raylib.h"
#include "raymath.h"
#include "threadpool.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static float GravityConstant = 0.04;

static void    ApplyForce(Body* b, Vector2 force);
static Vector2 Attract(Body* b1, Body* b2);

#define COLOR_RED "\x1b[38;5;1m"
#define COLOR_RESET "\x1b[0m"

#define PRINT(s) printf(COLOR_RED s COLOR_RESET "\n");
#define PRINTF(fmt, ...) printf(COLOR_RED fmt COLOR_RESET "\n", __VA_ARGS__);

#define THREAD_COUNT 5
#define SIZE         8192
#define QUEUE_SIZE   MAX_QUEUE

threadpool_t*   threadpool;
pthread_mutex_t pool_lock;
int             left;
int             error;
int             tasks_bodies_start[THREAD_COUNT];
int             tasks_bodies_count[THREAD_COUNT];
int             tasks_thread_id[THREAD_COUNT];

int finished;

void force_task(void *arg)
{
    PRINT("force_task()");
    int* pi = (int*)arg;
    int id = *pi;
    // *pi += 1;

    int start = tasks_bodies_start[id];
    int last  = tasks_bodies_start[id] + tasks_bodies_count[id];

    PRINTF("force_task[%i] start: %i, last %i", id, start, last);
    while (1) {
        // update attraction force on all bodies
        for (int i = start; i < last; i++) {
            for (int j = start; j < i; j++) {
                Vector2 force = Attract(&world.vec.bodies[i], &world.vec.bodies[j]);
                ApplyForce(&world.vec.bodies[i], force);
                ApplyForce(&world.vec.bodies[j], Vector2Negate(force));
            }
        }
    }
}

void adjust_tasks()
{
    PRINT("adjust_tasks()");
    pthread_mutex_lock(&pool_lock);

    int slice = world.vec.count / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        tasks_thread_id[i] = i;
        tasks_bodies_start[i] = i * slice;
        tasks_bodies_count[i] = slice;
        PRINTF("tasks_thread_id %i - tasks_bodies_start: %i, tasks_bodies_count %i", tasks_thread_id[i], tasks_bodies_start[i], tasks_bodies_count[i]);
    }

    pthread_mutex_unlock(&pool_lock);
}

void run_tasks()
{
    for (int i = 0; i < THREAD_COUNT; i++) {
        int result = threadpool_add(threadpool, force_task, &tasks_thread_id[i], 0);
        if (result != 0) {
            PRINTF("threadpool_add failed: %i", result);
        }
    }
}

void setup_threads()
{
    PRINT("setup_threads()");

    pthread_mutex_init(&pool_lock, NULL);

    adjust_tasks();
    threadpool = threadpool_create(THREAD_COUNT, QUEUE_SIZE, 0);

    PRINTF("pool started with %d threads and queue size of %d\n", THREAD_COUNT, QUEUE_SIZE);
}

void UpdateBodies(BodyVector* vec)
{
    run_tasks();

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
    float distance = Vector2LengthSqr(force);
    distance = Clamp(distance, 0.1, 45*45);
    force = Vector2Normalize(force);
    float strength = (GravityConstant * b2->mass * b1->mass) / distance;

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
