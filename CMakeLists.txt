cmake_minimum_required(VERSION 3.10)

set(NAME CrookedEditor)
project(${NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CURSES_NEED_WIDE TRUE)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# export CMAKE_INCLUDE_PATH="$(brew --prefix ncurses)/include:$CMAKE_INCLUDE_PATH"
# export CMAKE_LIBRARY_PATH="$(brew --prefix ncurses)/lib:$CMAKE_LIBRARY_PATH"

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/source/*.cpp")
add_executable(${NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${NAME} PRIVATE ${CURSES_LIBRARIES})

target_include_directories(${NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs")

