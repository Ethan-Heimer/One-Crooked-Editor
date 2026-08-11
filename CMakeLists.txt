cmake_minimum_required(VERSION 3.10)

set(NAME CrookedEditor)
project(${NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#   add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
#   add_link_options(-fsanitize=address)
#endif()

#add_compile_options(-O3)
add_compile_options(-Wall -Wextra)

add_subdirectory("${CMAKE_SOURCE_DIR}/libs/editorlibs/editor")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/editorlibs/renderer")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/client/*.cpp")
add_executable(${NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${NAME} PRIVATE ${CURSES_LIBRARIES})

target_link_libraries(${NAME} PRIVATE editor)
target_link_libraries(${NAME} PRIVATE tuirenderer)

target_include_directories(${NAME} PRIVATE ${CURSES_INCLUDE_DIRS})

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/gapbuffer/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/linkedlist/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/statemachine/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/safequeue/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/trees/")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/editorlibs/states/include")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/editorlibs/editor/include")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/client/systems/input/include")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/client/buffer/include")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/client/editor/include")

