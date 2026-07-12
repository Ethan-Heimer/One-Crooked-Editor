cmake_minimum_required(VERSION 3.10)

set(NAME CrookedEditor)
project(${NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CURSES_NEED_WIDE TRUE)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

add_subdirectory("${CMAKE_SOURCE_DIR}/libs/editorlibs/buffer")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/editorlibs/editor")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/source/*.cpp")
add_executable(${NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${NAME} PRIVATE ${CURSES_LIBRARIES})

target_link_libraries(${NAME} PRIVATE buffers)
target_link_libraries(${NAME} PRIVATE editor)

target_include_directories(${NAME} PRIVATE ${CURSES_INCLUDE_DIRS})

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/filehandling/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/gapbuffer/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/linkedlist/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/statemachine/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/undoing/")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/headerlibs/trees/")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/editorlibs/states/include")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/editorlibs/editor/include")

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/systems/input/include")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/editor/editor/include")
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/editor/state/include")

