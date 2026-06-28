cmake_minimum_required(VERSION 3.10)
project(buffers VERSION 3.0 LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_library(buffers SHARED ${SOURCE_FILES})

#target_link_libraries(buffer PUBLIC tui)
target_link_libraries(buffers PRIVATE editor)

message("${CMAKE_SOURCE_DIR}")

target_include_directories(buffers PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/filehandling/")
target_include_directories(buffers PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/gapbuffer/")
target_include_directories(buffers PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/linkedlist/")

target_include_directories(buffers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
