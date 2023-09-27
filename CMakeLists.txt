include(FetchContent)
include(CMakePrintHelpers)

set(CMAKE_BUILD_TYPE Release)

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
        ./external/tinyobjloader/*.h
    )

include_directories(
    ./external/imgui
    ./external/imgui/backends
    ./external/spng
    ./external/tinyobjloader
    ./src/core
    ./src/core/objects
    ./src/core/objects/primitives
    ./src/core/objects/meshes
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


# cmake_print_variables(${CMAKE_SOURCE_DIR})

FetchContent_Declare (
    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable (tracy)
target_link_libraries (raytracer PUBLIC TracyClient)
if(EXISTS ${CMAKE_SOURCE_DIR}/build/_deps/tracy-src/public/tracy/Tracy.hpp)
    target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/build/_deps/tracy-src/public/tracy)
else()
    message(FATAL_ERROR "Tracy.hpp not found. Check the path to the Tracy source directory.")
endif()

find_package(glfw3 REQUIRED)
target_link_libraries(raytracer PUBLIC glfw)

find_package(GLEW REQUIRED)
target_link_libraries(raytracer PUBLIC GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(raytracer PUBLIC OpenGL)
