cmake_minimum_required(VERSION 3.24)
project(ray_tracing_renderer)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include/)

add_executable(ray_tracing_renderer src/main.cpp src/application/Application.cpp src/application/Application.h src/image/Image.cpp src/image/Image.h src/renderer/Renderer.cpp src/renderer/Renderer.h src/image/Color.cpp src/image/Color.h src/scene/object/Sphere.cpp src/scene/object/Sphere.h src/scene/Camera.cpp src/scene/Camera.h src/math/Ray.cpp src/math/Ray.h src/scene/object/Object.h src/scene/object/Plane.cpp src/scene/object/Plane.h include/imgui/imgui.cpp include/imgui/imgui_impl_sdl.cpp include/imgui/imgui_widgets.cpp include/imgui/imgui_draw.cpp include/imgui/imgui_tables.cpp include/imgui/imgui_impl_sdlrenderer.cpp)

add_dependencies(ray_tracing_renderer OpenGL::OpenGL)

target_link_libraries(ray_tracing_renderer ${SDL2_LIBRARIES})
target_link_libraries(ray_tracing_renderer OpenGL::OpenGL)
