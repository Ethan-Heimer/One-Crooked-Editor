cmake_minimum_required(VERSION 3.10)

set(LIBNAME editor)
project(${LIBNAME} VERSION 3.0 LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# add_subdirectory("${CMAKE_SOURCE_DIR}/libs/editorlibs/buffer" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
add_library(${LIBNAME} SHARED ${SOURCE_FILES})

target_link_libraries(${LIBNAME} PRIVATE buffers)

target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/filehandling/")
target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/gapbuffer/")
target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/linkedlist/")
target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/statemachine/")

target_include_directories(${LIBNAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/editorlibs/states/include/")

target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/editorstatecontext/")
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/editorstates/")
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
