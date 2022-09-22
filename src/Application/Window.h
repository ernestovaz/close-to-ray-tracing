#pragma once

#include <string>

#include <GLFW/glfw3.h>

using std::string;

class Window {
public:
    int width;
    int height;
    string name;

    Window(int width, int height, string name);
    ~Window();

    bool shouldClose();
    void pollEvents();

private:
    GLFWwindow* window;
    void init();
};
