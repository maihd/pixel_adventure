cmake_minimum_required(VERSION 3.18)
project(pixel_adventure)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(libs/SDL3_image EXCLUDE_FROM_ALL)

file(GLOB HEADERS
    
    libs/imgui/imconfig.h
)

file(GLOB SOURCES
    src_sdl3/main.c

    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp

    libs/imgui/backends/sdl3.cpp
    libs/imgui/backends/sdlgpu3.cpp
)

add_executable(pixel_adventure ${SOURCES})
target_include_directories(pixel_adventure PRIVATE libs/vectormath/include libs/imgui libs/imgui/backends)
target_link_directories(pixel_adventure PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)