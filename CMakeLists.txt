cmake_minimum_required(VERSION 3.5)
project(cg_from_scratch)

set(CMAKE_C_STANDARD 99)              # TODO CHECK
set(CMAKE_C_STANDARD_REQUIRED TRUE)   # TODO CHECK
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Create your game executable target as usual
add_executable(rasterizer main.c raster.c raster.h)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(rasterizer PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(rasterizer PRIVATE SDL2::SDL2 m)
