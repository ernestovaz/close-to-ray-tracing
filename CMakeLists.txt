cmake_minimum_required(VERSION 3.24)
project(ray_tracing_engine)

set(CMAKE_CXX_STANDARD 14)

add_executable(ray_tracing_engine src/main.cpp src/Application/Window.cpp src/Application/Window.h)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(ray_tracing_engine glfw)
target_link_libraries(ray_tracing_engine ${Vulkan_LIBRARIES})