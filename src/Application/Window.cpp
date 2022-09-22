#include "Window.h"


Window::Window(int width, int height, string name)
: width(width), height(height), name(name) {
    init();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if(!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

bool Window::shouldClose() {
   return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}


