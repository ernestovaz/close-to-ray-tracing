#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using std::string;

class Window {
public:

    Window();
    ~Window();

    bool shouldClose();
    void draw();

private:
    GLFWwindow* window;
    void initWindow();
    void swapChainResize();
    void render();
    void cleanup();
};
