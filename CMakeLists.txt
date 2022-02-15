cmake_minimum_required(VERSION 3.16.0)

project(nbody VERSION 0.1.0 LANGUAGES C)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(pthreads REQUIRED)
find_package(raylib CONFIG REQUIRED)

include_directories(${RAYLIB_INCLUDE_DIRS})

add_executable(
    nbody
    "src/main.c"

    "src/nbody.c"
    "src/body.c"
    "src/threadpool.c"
)

target_link_libraries(nbody ${PThreads4W_LIBRARY} ${RAYLIB_LIBRARIES})
