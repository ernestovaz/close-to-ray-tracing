cmake_minimum_required(VERSION 3.24)
project(ray_tracing_engine)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(ray_tracing_engine src/Main.cpp src/Application/Window.cpp src/Application/Window.h include/imgui/imgui.cpp include/imgui/imgui_impl_vulkan.cpp include/imgui/imgui_impl_glfw.cpp include/imgui/imgui_widgets.cpp include/imgui/imgui_demo.cpp include/imgui/imgui_draw.cpp include/imgui/imgui_tables.cpp)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(ray_tracing_engine glfw)
target_link_libraries(ray_tracing_engine ${Vulkan_LIBRARIES})
