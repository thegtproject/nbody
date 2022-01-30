# nbody
A simple nbody sim using raylib

![](demo.gif)

## Dependencies
raylib 4.0

## Build
This project should build with no problems on Linux and Windows with cmake. You will need raylib to be discoverable by cmake. For example using vcpkg on windows:

    mkdir build && cd build
    cmake --toolchain C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake ..
    cmake --build . --config Release
