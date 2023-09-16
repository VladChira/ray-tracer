cmake_minimum_required(VERSION 3.12)
project(raytracer)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g -D SPNG_USE_MINIZ)

# Create a build directory
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
file(MAKE_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})


file(GLOB_RECURSE SRC_FILES
    ./src/*.h
    ./src/*.cpp
)

file(GLOB_RECURSE EXT_FILES
        ./external/imgui/*.h
        ./external/imgui/*.cpp
        ./external/imgui/backends/*.h
        ./external/spng/*.h
        ./external/spng/*.c
    )

include_directories(
    ./external/imgui
    ./external/imgui/backends
    ./external/spng
    ./src/core
    ./src/core/objects
    ./src/core/samplers
    ./src/core/cameras
    ./src/core/materials
    ./src/core/BRDFs
    ./src/math
    ./src/wrappers
    ./src/gui
    ./src/core/tracers
    ./src/core/lights
    )
    

# Add the source files
set(SRCS ${SRC_FILES} ${EXT_FILES})

# Add the executable
add_executable(raytracer ${SRCS})

find_package(glfw3 REQUIRED)
target_link_libraries(raytracer glfw)

find_package(GLEW REQUIRED)
target_link_libraries(raytracer GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(raytracer OpenGL)
