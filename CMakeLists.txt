cmake_minimum_required(VERSION 3.18)
project(pixel_adventure)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src_sdl3)
set(LIBS_DIR libs)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

set(SDLIMAGE_VENDORED ON)

set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(libs/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(libs/SDL3_image EXCLUDE_FROM_ALL)

file(GLOB HEADERS
    
    libs/imgui/imconfig.h
)

file(GLOB SOURCES
    ${SRC_DIR}/main.c

    ${SRC_DIR}/Framework/*.cpp
    ${SRC_DIR}/Misc/*.c
    ${SRC_DIR}/Misc/*.cpp
    ${SRC_DIR}/Text/*.cpp
    ${SRC_DIR}/Native/*.cpp
    ${SRC_DIR}/Functional/*.cpp

    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp

    libs/imgui/backends/sdl3.cpp
    libs/imgui/backends/sdlgpu3.cpp
)

add_executable(pixel_adventure ${SOURCES})
target_include_directories(pixel_adventure PRIVATE ${SRC_DIR} libs/vectormath/include libs libs/imgui/backends)
target_link_directories(pixel_adventure PRIVATE SDL3::SDL3-static SDL3_image::SDL3_image)