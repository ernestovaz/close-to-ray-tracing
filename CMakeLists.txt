cmake_minimum_required(VERSION 3.24)
project(ray_tracing_renderer)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include/)

add_executable(ray_tracing_renderer src/main.cpp src/application/Application.cpp src/application/Application.h src/image/Image.cpp src/image/Image.h src/renderer/Renderer.cpp src/renderer/Renderer.h src/image/Color.cpp src/image/Color.h)
target_link_libraries(ray_tracing_renderer ${SDL2_LIBRARIES})

