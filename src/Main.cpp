#include <iostream>

#include "Application/Window.h"

int main() {
    Window window;
    while(!window.shouldClose()){
        window.draw();
    }
}
