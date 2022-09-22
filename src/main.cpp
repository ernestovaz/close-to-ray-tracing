#include <iostream>

#include "Application/Window.h"

int main() {
    Window window(360, 360, "render");
    while(!window.shouldClose()){
        window.pollEvents();
    }
}
