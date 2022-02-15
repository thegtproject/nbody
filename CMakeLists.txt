cmake_minimum_required(VERSION 3.16.0)

project(nbody VERSION 0.1.0 LANGUAGES C)

find_package(raylib CONFIG REQUIRED)
include_directories(${RAYLIB_INCLUDE_DIRS})

add_executable(
    nbody
    "src/main.c"

    "src/nbody.c"
    "src/body.c"
)
if(WIN32)
    target_link_libraries(nbody ${RAYLIB_LIBRARIES})
else()
    target_link_libraries(nbody raylib m)
endif(WIN32)
