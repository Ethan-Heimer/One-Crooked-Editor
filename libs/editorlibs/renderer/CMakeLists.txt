cmake_minimum_required(VERSION 3.10)

set(LIBNAME tuirenderer)
project(${LIBNAME} VERSION 3.0 LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_library(${LIBNAME} SHARED ${SOURCE_FILES})

target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/terminal/")
target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/safequeue/")

target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/commands")
