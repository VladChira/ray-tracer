# CMakeLists.txt for SPNG
 
# Add all the source files of SPNG
file(GLOB_RECURSE SPNG_SOURCES "*.c")

# Create the SPNG library
add_library(spng ${SPNG_SOURCES})

# Include the directory with SPNG headers
target_include_directories(spng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})